/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: eyjian@qq.com or eyjian@gmail.com
 */
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include "sys/event.h"
SYS_NAMESPACE_BEGIN

/** �¼����У������̰߳�ȫ
  * ����1: �������Ϊ�գ���ɵȴ�����������ʱ
  * ����2: ���������������ɵȴ�����Ϊ����ʱ
  * RawQueueClassΪԭʼ������������util::CArrayQueue
  */
template <class RawQueueClass>
class CEventQueue
{       
public:
    /** �����е�Ԫ���������� */
    typedef typename RawQueueClass::_DataType DataType;

    /***
      * ����һ���¼�����
      * @pop_milliseconds: pop_frontʱ�ȴ�����Ϊ�ǿ�ʱ�ĺ����������Ϊ0���ʾ���ȴ���
      *                ����������������Ϊ�գ���pop_front��������false
      * @push_milliseconds: push_backʱ�ȴ�����Ϊ����ʱ�ĺ����������Ϊ0���ʾ���ȴ���
      *                ������������������������push_back��������false
      * @queue_max: ��Ҫ����Ķ��д�С
      */
	CEventQueue(uint32_t queue_max, uint32_t pop_milliseconds, uint32_t push_milliseconds)
		:_raw_queue(queue_max)
        ,_pop_milliseconds(pop_milliseconds)
        ,_push_milliseconds(push_milliseconds)
        ,_pop_waiter_number(0)
        ,_push_waiter_number(0)
    {
    }

    /** �ж϶����Ƿ����� */
    bool is_full() const 
	{
        CLockHelper<CLock> lock(_lock);
        return _raw_queue.is_full();
    }
    
    /** �ж϶����Ƿ�Ϊ�� */
    bool is_empty() const 
	{
        CLockHelper<CLock> lock(_lock);
        return _raw_queue.is_empty();
    }
    
    /***
      * ���ض���Ԫ��
      * @elem: �洢����Ԫ��
      * @return: ������в�Ϊ���򷵻�true�����򷵻�false
      */
    bool front(DataType& elem) const 
	{
        CLockHelper<CLock> lock(_lock);
        if (_raw_queue.is_empty()) return false;
        
        elem = _raw_queue.front();
        return true;
    }

	/***
      * ��������Ԫ��
      * @elem: �洢�������Ķ���Ԫ��
      * @return: ����ɹ��Ӷ��е��������򷵻�true�����򷵻�false
      * @exception: �����׳�CSyscallExceptoin�쳣�������쳣��RawQueue�й�
      */
    bool pop_front(DataType& elem) 
	{
        CLockHelper<CLock> lock(_lock);
        while (_raw_queue.is_empty())
        {
            // ������ȴ�������������
            if (0 == _pop_milliseconds) return false;
            // ʹ������������������Ϊtimed_wait�������쳣
            util::CountHelper<volatile int> ch(_pop_waiter_number); 
            
            // ��ʱ����������
            if (!_event.timed_wait(_lock, _pop_milliseconds)) return false;
        }

        elem = _raw_queue.pop_front();
        if (_push_waiter_number > 0) _event.signal();
        return true;
    }
    
	/***
      * ����β����һ��Ԫ��
      * @elem: ��Ҫ�����β������
      * @return: ����ɹ�����β���������ݣ��򷵻�true�����򷵻�false
      * @exception: �����׳�CSyscallExceptoin�쳣�������쳣��RawQueue�й�
      */
    bool push_back(DataType elem) 
	{
        CLockHelper<CLock> lock(_lock);
        while (_raw_queue.is_full())
        {
            // ������ȴ�������������
            if (0 == _push_milliseconds) return false;  
            // ʹ������������������Ϊtimed_wait�������쳣
            util::CountHelper<volatile int> ch(_push_waiter_number); 

            // ��ʱ����������
            if (!_event.timed_wait(_lock, _push_milliseconds)) return false;
        }

        _raw_queue.push_back(elem);
        if (_pop_waiter_number > 0) _event.signal();
        return true;
    }

    /** �õ������д洢��Ԫ�ظ��� */
    uint32_t size() const 
	{ 
        CLockHelper<CLock> lock(_lock);
		return _raw_queue.size(); 
	}

private:        
    CEvent _event;
    mutable CLock _lock;    
    RawQueueClass _raw_queue;      /** ԭʼ���� */       

private:
    uint32_t _pop_milliseconds;    /** ����ʱ�ȴ���ʱ������ */
    uint32_t _push_milliseconds;   /** ���ʱ�ȴ���ʱ������ */
    volatile int _pop_waiter_number;  /** �ȴ����������ݵ��̸߳��� */
    volatile int _push_waiter_number; /** �ȴ������п�λ�õ��̸߳��� */
};

SYS_NAMESPACE_END
#endif // EVENT_QUEUE_H

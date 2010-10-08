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
#ifndef EPOLLABLE_QUEUE_H
#define EPOLLABLE_QUEUE_H
#include "sys/event.h"
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/** ���Է���Epoll��صĶ���
  * RawQueueClassΪԭʼ������������util::CArrayQueue
  * Ϊ�̰߳�ȫ��
  */
template <class RawQueueClass>
class CEpollableQueue: public CEpollable
{
    typedef typename RawQueueClass::_DataType DataType;
    
public:
    /** ����һ����Epoll�Ķ��У�ע��ֻ�ɼ�ض��¼���Ҳ���Ƕ������Ƿ�������
      * @queue_max: �����������ɵ�Ԫ�ظ���
      * @exception: ����������׳�CSyscallException�쳣
      */
    CEpollableQueue(uint32_t queue_max)
        :_raw_queue(queue_max)
        ,_push_waiter_number(0)
    {
        if (-1 == pipe(_pipefd)) throw sys::CSyscallException(errno, __FILE__, __LINE__);
        set_fd(_pipefd[0]);
    }
    
    ~CEpollableQueue()
    {
        close();
    }

    /** �رն��� */
    virtual void close()
    {
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        if (_pipefd[0] != -1)
        {        
            close_fd(_pipefd[0]);
            _pipefd[0] = -1;
        }
        if (_pipefd[1] != -1)
        {        
            close_fd(_pipefd[1]);
            _pipefd[1] = -1;
        }
    }

    /** �ж϶����Ƿ����� */
    bool is_full() const 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        return _raw_queue.is_full();
    }
    
    /** �ж϶����Ƿ�Ϊ�� */
    bool is_empty() const 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        return _raw_queue.is_empty();
    }

    /***
      * ȡ����Ԫ��
      * @elem: �洢ȡ���Ķ���Ԫ��
      * @return: �������Ϊ�գ��򷵻�false�����򷵻�true
      */
    bool front(DataType& elem) const 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        if (_raw_queue.is_empty()) return false;

        elem = _raw_queue.front();
        return true;
    }
    
	/***
      * ��������Ԫ��
      * @elem: �洢�����Ķ���Ԫ��
      * @return: �������Ϊ�գ��򷵻�false������ȡ��Ԫ�ز�����true
      * @exception: ����������׳�CSyscallException�쳣
      */
    bool pop_front(DataType& elem) 
	{
        // û�����ݣ�Ҳ�������������Ҫ������Ӧ��ʹ���¼�����CEventQueue
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        if (_raw_queue.is_empty()) return false;

        char c;
        // read�����൱��CEvent::wait������        
        while (-1 == read(_pipefd[0], &c, sizeof(c)))
        {
            if (errno != EINTR)
                throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        elem = _raw_queue.pop_front();
        // ����еȴ��ţ���������һ��
        if (_push_waiter_number > 0) _event.signal();
        
        return true;
    }
    
	/***
      * ���β����һԪ��
      * @elem: �����뵽��β��Ԫ��
      * @millisecond: ������������ȴ����з����ĺ����������Ϊ0�򲻵ȴ���ֱ�ӷ���false
      * @return: ��������Ѿ������򷵻�false���������ɹ�������true
      * @exception: ����������׳�CSyscallException�쳣
      */
    bool push_back(DataType elem, uint32_t millisecond=0) 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        while (_raw_queue.is_full())
        {
            // ��������
            if (0 == millisecond) return false;

            // ��ʱ�ȴ�
            util::CountHelper<volatile int32_t> ch(_push_waiter_number);            
            if (!_event.timed_wait(_lock, millisecond)) 
            {
                return false;
            }
        }        

        char c = 'x';
        _raw_queue.push_back(elem);
        // write�����൱��signal������        
        while (-1 == write(_pipefd[1], &c, sizeof(c)))
        {
            if (errno != EINTR)
                throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        return true;
    }

    /** �õ������е�ǰ�洢��Ԫ�ظ��� */
    uint32_t size() const 
	{ 
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        return _raw_queue.size(); 
	}

private:
    int _pipefd[2]; /** �ܵ���� */    
    sys::CLock _lock;
    sys::CEvent _event;
    RawQueueClass _raw_queue; /** ��ͨ����ʵ�� */
    volatile int32_t _push_waiter_number; /** �ȴ����з������̸߳��� */
};

NET_NAMESPACE_END
#endif // EPOLLABLE_QUEUE_H

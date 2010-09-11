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
#include "sys/lock.h"
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/** ���Է���Epoll��صĶ���
  * GeneralQueueClassΪ��ͨ��������
  */
template <class GeneralQueueClass>
class CEpollableQueue: public CEpollable
{
    typedef typename GeneralQueueClass::_DataType DataType;
    
public:
    /** ����һ����Epoll�Ķ��У�ע��ֻ�ɼ�ض��¼���Ҳ���Ƕ������Ƿ�������
      * @queue_max: �����������ɵ�Ԫ�ظ���
      */
    CEpollableQueue(uint32_t queue_max)
        :_general_queue(queue_max)
    {
        _notify[0] = 'X';
        if (-1 == pipe(_pipefd)) throw sys::CSyscallException(errno, __FILE__, __LINE__);
        set_fd(_pipefd[0]);
    }
    
    ~CEpollableQueue()
    {
        close();
    }

    virtual void close()
    {
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        close_fd(_pipefd[0]);
        close_fd(_pipefd[1]);
        _pipefd[0] = -1;
        _pipefd[1] = -1;
    }

    bool is_full() const 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        return _general_queue.is_full();
    }
    
    bool is_empty() const 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        return _general_queue.is_empty();
    }

    bool front(DataType& elem) const 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        if (_general_queue.is_empty()) return false;

        elem = _general_queue.front();
        return true;
    }
    
	/** ����pop֮ǰӦ����ʹ��is_empty�ж�һ�� */
    bool pop_front(DataType& elem) 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        if (_general_queue.is_empty()) return false; // û�����ݣ�Ҳ������
        
        // ���û�����ݣ�������������������ݶ��У���ʵ���Բ�����
        while (-1 == read(_pipefd[0], _notify, sizeof(_notify)))
        {
            if (errno != EINTR)
                throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        elem = _general_queue.pop_front();
        return true;
    }
    
	/** ����pop֮ǰӦ����ʹ��is_full�ж�һ�� */
    bool push_back(DataType elem) 
	{
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        if (_general_queue.is_full()) return false;

        _general_queue.push_back(elem);
        while (-1 == write(_pipefd[1], _notify, sizeof(_notify)))
        {
            if (errno != EINTR)
                throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        return true;
    }

    uint32_t size() const 
	{ 
        sys::CLockHelper<sys::CLock> lock_helper(_lock);
        return _general_queue.size(); 
	}

private:
    int _pipefd[2]; /** �ܵ���� */
    char _notify[1];
    sys::CLock _lock;
    GeneralQueueClass _general_queue; /** ��ͨ����ʵ�� */
};

NET_NAMESPACE_END
#endif // EPOLLABLE_QUEUE_H

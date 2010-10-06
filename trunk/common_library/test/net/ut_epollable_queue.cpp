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
#include "sys/thread.h"
#include "net/epoller.h"
#include "sys/sys_util.h"
#include "util/array_queue.h"
#include "sys/datetime_util.h"
#include "net/epollable_queue.h"

#define QUEUE_SIZE  10000 // ���д�С
#define LOOP_NUMBER 10000 // ѭ������

// ������ȡ���������ݵ��̣߳������ݴӶ����ж�����Ȼ���������׼���
class CUTEpollableQueueThread: public sys::CThread
{
public:
    CUTEpollableQueueThread(net::CEpollableQueue<util::CArrayQueue<int> >* queue)
        :_queue(queue)
    {
        uint32_t epoll_size = 10;
        _epoller.create(epoll_size); // ����Epoll
        _epoller.set_events(queue, EPOLLIN); // �����з���Epoll��
    }

    ~CUTEpollableQueueThread()
    {
        _epoller.destroy();
    }

private:
    virtual void run()
    {
        while (!is_stop())
        {
            try
            {
                // Epoll���������Ƿ�������
                if (0 == _epoller.timed_wait(1000))
                    continue; // ��ʱ������ȴ�

                int m = 0;
                if (_queue->pop_front(m)) // ������������
                    fprintf(stdout, "<%s> pop %d from queue.\n", sys::CDatetimeUtil::get_current_datetime().c_str(), m);
                else
                    fprintf(stderr, "<%s> get nothing from queue.\n", sys::CDatetimeUtil::get_current_datetime().c_str());
            }
            catch (sys::CSyscallException& ex)
            {
                fprintf(stderr, "CUTEpollableQueueThread exception: %s at %s:%d.\n"
                    ,sys::CSysUtil::get_error_message(ex.get_errcode()).c_str()
                    ,ex.get_filename(), ex.get_linenumber());
            }
        }
    }

private:
    net::CEpoller _epoller;
    net::CEpollableQueue<util::CArrayQueue<int> >* _queue; // ʹ��CArrayQueue��Ϊ��������
};

int main()
{
    try
    {
        uint32_t queue_size = QUEUE_SIZE;
        net::CEpollableQueue<util::CArrayQueue<int> > queue(queue_size);
        CUTEpollableQueueThread* thread = new CUTEpollableQueueThread(&queue);

        thread->inc_refcount(); // �߳����ü�����һ
        thread->start(); // �����߳�

				// ѭ���������в�������
        for (int i=1; i<LOOP_NUMBER; ++i)
        {
            if (queue.push_back(i))
                fprintf(stdout, "<%s> push %d to queue.\n", sys::CDatetimeUtil::get_current_datetime().c_str(), i);
            else
                fprintf(stderr, "<%s> failed to push %d to queue.\n", sys::CDatetimeUtil::get_current_datetime().c_str(), i);

						// ���߳�sleepһ����
            sys::CSysUtil::millisleep(1000);
        }

				thread->stop(); // ֹͣ�߳�
        thread->dec_refcount(); // �߳����ü�����һ�����������thread->stop();����֮��
    }
    catch (sys::CSyscallException& ex)
    {
    		// �쳣����
        fprintf(stderr, "main exception: %s at %s:%d.\n"
            ,sys::CSysUtil::get_error_message(ex.get_errcode()).c_str()
            ,ex.get_filename(), ex.get_linenumber());
    }

    return 0;
}

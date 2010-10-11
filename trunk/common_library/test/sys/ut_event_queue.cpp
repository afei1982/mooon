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
#include <sys/thread.h>
#include <sys/sys_util.h>
#include <sys/event_queue.h>
#include <util/array_queue.h>
#include <sys/datetime_util.h>

// ���崦����Ϣ�̣߳������߳���������Ϣ
class CMyThread: public sys::CThread
{
public:
    // ���ö��д�СΪ100
    // pop�ȴ�ʱ��Ϊ10000����
    // push�ȴ�ʱ��Ϊ0����
    CMyThread()
        :_queue(100, 3000, 0)
    {
    }

    void push_message(int m)
    {
        if (_queue.push_back(m))
            printf("push %d SUCCESS by thread %u\n", m
                , sys::CThread::get_current_thread_id());
        else
            printf("push %d FAILURE by thread %u\n", m
                , sys::CThread::get_current_thread_id());
    }
    
private:
    virtual void run()
    {
        for (;;)
        {
            // ���жϣ��Ա�֤������Ϣ����������˳�
            if (is_stop() && _queue.is_empty()) break;
            
            int m;
            printf("before pop ==> %s\n"
                    , sys::CDatetimeUtil::get_current_time().c_str());
            if (_queue.pop_front(m))
                printf("pop %d ==> %s\n", m
                    , sys::CDatetimeUtil::get_current_time().c_str());
            else
                printf("pop NONE ==> %s\n"
                    , sys::CDatetimeUtil::get_current_time().c_str());
        }
    }

private:
    // ʹ���������͵��������
    sys::CEventQueue<util::CArrayQueue<int> > _queue;
};

int main()
{
    CMyThread* thread = new CMyThread;
    // ʹ�����ü��������࣬��Э���Զ�����thread
    sys::CRefCountHelper<CMyThread> ch(thread);

    try
    {
        thread->start(); // �����߳�

        // ���̷߳���Ϣ
        for (int i=0; i<10; ++i)
        {
            sys::CSysUtil::millisleep(2000);
            thread->push_message(i);
        }        

        // ��CMyThread��ʱ
        sys::CSysUtil::millisleep(3000);

        // ֹͣ�߳�
        thread->stop();
    }
    catch (sys::CSyscallException& ex)
    {
        printf("Main exception: %s at %s:%d\n"
            , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str()
            , ex.get_filename()
            , ex.get_linenumber());
    }
    
    return 0;
}
/*
��������Ĵ��룬��������ݴ��£��̺߳ź�ʱ��᲻ͬ������pop��push��˳����ܻ���Щ��ͬ����Ϊ���߳�Ϊ���еģ�����������һ�����ݶ�Ӧ��Ϊpop NONE������:
pop 0 ==> 21:06:59
before pop ==> 21:06:59
push 0 SUCCESS by thread 782374640
push 1 SUCCESS by thread 782374640
pop 1 ==> 21:07:01
before pop ==> 21:07:01
push 2 SUCCESS by thread 782374640
pop 2 ==> 21:07:03
before pop ==> 21:07:03
push 3 SUCCESS by thread 782374640
pop 3 ==> 21:07:05
before pop ==> 21:07:05
push 4 SUCCESS by thread 782374640
pop 4 ==> 21:07:07
before pop ==> 21:07:07
push 5 SUCCESS by thread 782374640
pop 5 ==> 21:07:09
before pop ==> 21:07:09
push 6 SUCCESS by thread 782374640
pop 6 ==> 21:07:11
before pop ==> 21:07:11
push 7 SUCCESS by thread 782374640
pop 7 ==> 21:07:13
before pop ==> 21:07:13
push 8 SUCCESS by thread 782374640
pop 8 ==> 21:07:15
before pop ==> 21:07:15
push 9 SUCCESS by thread 782374640
pop 9 ==> 21:07:17
before pop ==> 21:07:17
pop NONE ==> 21:07:20
*/

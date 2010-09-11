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
 * Author: jian yi, eyjian@qq.com
 */
#ifndef POOL_THREAD_H
#define POOL_THREAD_H
#include "sys/event.h"
#include "sys/thread.h"
SYS_NAMESPACE_BEGIN

// �����̳߳ص��̳߳������
class CPoolThread: public CRefCountable
{
private:
    friend class CPoolThreadHelper;
    template <class ThreadClass> friend class CThreadPool;
    class CPoolThreadHelper: public CThread
    {
        typedef enum
        {
            sf_init,     /** �̳߳�ʼ״̬ */
            sf_waiting,  /** �߳̽���ȴ�����״̬������֮�����wakeup */
            sf_wakeuped  /** �߳̽��뻽��״̬������֮��Ӧ����wait */
        }sync_flag_t;

    public:
        CPoolThreadHelper(CPoolThread* pool_thread);        
        void wakeup();

    private:
        virtual void run();
        virtual bool before_start();

    private:		
        CPoolThread* _pool_thread;
        volatile sync_flag_t _sync_flag;        
    };

protected:
    CPoolThread();
    virtual ~CPoolThread();

private:    
    virtual void run() = 0;
    virtual bool before_start() { return true; }
    void set_index(uint16_t index) { _index = index; }

public:    
    void wakeup();
    uint16_t get_index() const { return _index; }

private:
    bool start();  /** ����CThreadPool���� */
    void stop();   /** ����CThreadPool���� */
	
private:	
    uint16_t _index; /** ���߳��ڳ��е�λ�� */
    CPoolThreadHelper* _pool_thread_helper;	
};


SYS_NAMESPACE_END
#endif // POOL_THREAD_H

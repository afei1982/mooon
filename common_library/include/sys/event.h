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
#ifndef EVENT_H
#define EVENT_H
#include "sys/lock.h"
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/***
  * ֪ͨ�¼���
  * �����ڶ��е�ʵ�֣�����Ϊ��ʱ��ȴ����������ݣ����������push������
  * �����ѵȴ��߳�ȥ����ȡ����
  */
class CEvent
{
public:
    /***
      * ����֪ͨ�¼�ʵ��
      * @exception: �����׳�CSyscallException�쳣��ͨ���ɲ�������쳣
      */
    CEvent();
    ~CEvent();

    /***
      * �õ����߽���ȴ�״̬��ֱ�ӵ�������
      * @exception: �����׳�CSyscallException�쳣��ͨ���ɲ�������쳣
      */
    void wait(CLock& lock);

    /***
      * �õ����߽���ȴ�״̬��ֱ�ӵ������ѣ���ȴ���ʱ������ָ���ĺ�����
      * @return: �����ָ���ĺ�����֮ǰ�����ѣ��򷵻�true�����򷵻�false
      * @exception: �����׳�CSyscallException�쳣��ͨ���ɲ�������쳣
      */
    bool timed_wait(CLock& mutex, uint32_t millisecond);

    /***
      * ����һ������ȴ�״̬���̣߳����û���߳������ڵȴ�״̬�����Ѷ�������
      * ֻ�е�signal���÷�����wait����֮�����Ч
      * @exception: �����׳�CSyscallException�쳣��ͨ���ɲ�������쳣
      */
    void signal();

    /***
      * �㲥�����źţ������н���ȴ�״̬���߳�ȫ�����ѣ�
      * ���û���߳������ڵȴ�״̬�����Ѷ�������
      * ֻ�е�broadcast���÷�����wait����֮�����Ч
      * @exception: �����׳�CSyscallException�쳣��ͨ���ɲ�������쳣
      */
    void broadcast();
    
private:
    pthread_cond_t _cond;
};

SYS_NAMESPACE_END
#endif // EVENT_H

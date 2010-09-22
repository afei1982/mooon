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
#ifndef LOCK_H
#define LOCK_H
#include <pthread.h>
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/***
  * ��������
  * ���ڷǵݹ�����ͬһ�߳���δ�ͷ���һ�μӵ���֮ǰ��
  * ������������ȥ��ͬһ���������ݹ�������ͬһ�߳�������ͬһ�����Ӷ��
  */
class CLock
{
    friend class CEvent; // CEvent��Ҫ����CLock��_mutex��Ա

public:
    /***
      * ����һ��������
      * @recursive: �Ƿ���Ϊ�ݹ���
      * @exception: �����׳�CSyscallException�쳣
      */
    CLock(bool recursive = false);
    ~CLock();

    /***
      * ����������������ܻ�ȡ��������һֱ�ȴ�����ȡ����Ϊֹ
      * @exception: �����׳�CSyscallException�쳣
      */
    void lock();

    /***
      * ��������
      * ��ע������Ѿ�������lock���������ܵ���unlock����
      * @exception: �����׳�CSyscallException�쳣
      */
    void unlock();

    /***
      * �����Ե�ȥ��ȡ��������ò�����������������
      * @return: �����ȡ���������򷵻�true�����򷵻�false
      * @exception: �����׳�CSyscallException�쳣
      */
    bool try_lock(); 

    /***
      * �Գ�ʱ��ʽȥ��ȡ�������ָ���ĺ���ʱ���ڲ��ܻ�ȡ��������һֱ�ȴ�ֱ����ʱ
      * @return: �����ָ���ĺ���ʱ���ڻ�ȡ���������򷵻�true�����������ʱ�򷵻�false
      * @exception: �����׳�CSyscallException�쳣
      */
	bool timed_lock(uint32_t millisecond);

private:
    pthread_mutex_t _mutex;
    pthread_mutexattr_t _attr;
};

/***
  *�ݹ�����
  * ���ڷǵݹ�����ͬһ�߳���δ�ͷ���һ�μӵ���֮ǰ��
  * ������������ȥ��ͬһ���������ݹ�������ͬһ�߳�������ͬһ�����Ӷ��
  */
class CRecLock: public CLock
{
public:
    /***
      * ����һ���ݹ���
      * @exception: �����׳�CSyscallException�쳣
      */
    CRecLock()
        :CLock(true)
    {
    }
};

/***
  * �������࣬�����Զ�����
  */
template <class LockClass>
class CLockHelper
{
public:
    /***
      * ���������������
      * @exception: �����׳�CSyscallException�쳣
      */
    CLockHelper(LockClass& lock)
        :_lock(lock)
    {
        lock.lock();
    }

    /***
      * �������࣬�����Զ�����
      * @exception: ���������������׳��κ��쳣
      */
    ~CLockHelper()
    {
        try
        {
            _lock.unlock();
        }
        catch (...)
        {
        }
    }

private:
    LockClass& _lock;
};

SYS_NAMESPACE_END
#endif // LOCK_H

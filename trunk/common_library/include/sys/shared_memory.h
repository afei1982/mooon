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
#ifndef SYS_SHARED_MEMORY_H
#define SYS_SHARED_MEMORY_H
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/** System V�����ڴ�C++��װ�� */
class CSysVSharedMemory
{
public:
    CSysVSharedMemory();
    /***
      * ��һ���Ѿ����ڵĹ����ڴ�
      * @path: �������������ڴ��·��(�����ļ���)������ΪNULL
      * @exception: ����������׳�CSyscallException�쳣
      */
    void open(const char* path);

    /***
      * ����һ�������ڴ�
      * @path: �������������ڴ��·��(�����ļ���)�����ΪNULL����ϵͳѡ���һ�޶���
      * @mode: Ȩ��ģʽ����ֵΪS_IRWXU��S_IXUSR��
      * @return: ����Ѿ������򷵻�false�����򷵻�true
      * @exception: ����������׳�CSyscallException�쳣
      */
    bool create(const char* path, mode_t mode=IPC_DEFAULT_PERM);
    
    /***
      * �ر��Ѿ�������򿪵Ĺ����ڴ棬
      * ����Ѿ�û�н��̹������˹����ڴ棬��ɾ����
      * @exception: ����������׳�CSyscallException�쳣
      */
    void close();

    /***
      * ����͹����ڴ�Ĺ���
      * @exception: ����������׳�CSyscallException�쳣
      */
    void detach();

    /***
      * �����������ڴ�
      * @exception: ����������׳�CSyscallException�쳣
      */
    void* attach(int flag); 

    /***
      * �õ������ڴ�ĵ�ַ
      * @return: ����Ѿ������򷵻�ָ�����ڴ�ĵ�ַ�����򷵻�NULL
      */
    void* get_shared_memoty_address();
    void* get_shared_memoty_address() const;

private:
    int _shmid;
    void* _shmaddr; /** attach�Ĺ����ڴ��ַ */
};

SYS_NAMESPACE_END
#endif // SYS_SHARED_MEMORY_H

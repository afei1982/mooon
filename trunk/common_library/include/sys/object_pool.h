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
#ifndef _OBJECT_POOL_H
#define _OBJECT_POOL_H
#include "sys/lock.h"
SYS_NAMESPACE_BEGIN

/***
  * ������ʵ�֣����ܸߵ����̰߳�ȫ
  */
class CRawObjectPool
{
public:
    CRawObjectPool();
    ~CRawObjectPool();

    /** ������create�����Ķ���� */
    void destroy();

    /***
      * ���������
      * @object_size: �����С
      * @object_number: �������
      * @use_heap: ����ز���ʱ���Ƿ�Ӷ��Ϸ���
      * @guard_size: �����С
      * @guard_flag: �����ʶ
      */
    void create(uint16_t object_size, uint32_t object_number, bool use_heap=true, uint8_t guard_size=1, char guard_flag='m');

    /***
      * ��������ڴ�
      * @return: �������ز������������˴Ӷ��Ϸ����ڴ棬�򷵻شӶ��Ϸ�����ڴ棬
      *          �����������ز���ʱ����NULL�����򷵻شӶ�����з�����ڴ�
      */
    void* allocate();

    /***
      * ���ն����ڴ�
      * @object: ��Ҫ�����յ�������е��ڴ棬��������ڴ���е��ڴ棬
      *          ��createʱ����Ӷѷ��䣬��ֱ���ͷŸ��ڴ棬��������ڳ��ڴ淶Χ�ڣ�
      *          �����Ƿ�Ϊ��ȷ�ĳ��ڴ棬���������ղ�����true�������������false
      * @return: ��������ջ�ɾ������true�����򷵻�false
      */
    bool reclaim(void* object);

    /** ���ص�����ز�����ʱ���Ƿ�Ӷ��Ϸ����ڴ� */
    bool use_heap() const;

    /** �õ�����ֵ��С */
    uint8_t get_guard_size() const;

    /** �õ��ش�С��Ҳ���ǳ��пɷ���Ķ������ */
    uint32_t get_pool_size() const;        

    /** �õ�����ؿɷ���Ķ����С */
    uint16_t get_object_size() const;

    /** �õ�������У���ǰ�����Է���Ķ������ */
    uint32_t get_available_number() const;

private:    
    bool _use_heap;             /** ����ز���ʱ���Ƿ�Ӷ��Ϸ��� */
    uint8_t _guard_size;        /** �����С��ʵ����Ҫ���ڴ��СΪ: (_guard_size+_object_size)*_object_number */
    uint16_t _object_size;      /** �����С������_guard_size���֣�����ʵ�ʶ����СӦ���ټ�ȥ_guard_size */
    uint32_t _object_number;    /** ������� */    
    volatile uint32_t _stack_top_index;  /** ջ������ */
    volatile uint32_t _available_number; /** ���л����Է���Ķ������ */

private:
    char* _stack_top;    
    char* _stack_bottom;
    char** _object_stack;
    char* _bucket_bitmap; /** Ͱ״̬��������¼��ǰ״̬���Է�ֹ�ظ����� */    
};

/***
  * �̰߳�ȫ�Ķ���أ����ܽ�CRawObjectPoolҪ��
  */
class CThreadObjectPool
{
public:
    /** ������create�����Ķ���� */
    void destroy();

    /***
      * ���������
      * @object_size: �����С
      * @object_number: �������
      * @use_heap: ����ز���ʱ���Ƿ�Ӷ��Ϸ���
      * @guard_size: �����С
      * @guard_flag: �����ʶ
      */
    void create(uint16_t object_size, uint32_t object_number, bool use_heap=true, uint8_t guard_size=1, char guard_flag='m');

    /***
      * ��������ڴ�
      * @return: �������ز������������˴Ӷ��Ϸ����ڴ棬�򷵻شӶ��Ϸ�����ڴ棬
      *          �����������ز���ʱ����NULL�����򷵻شӶ�����з�����ڴ�
      */
    void* allocate();

    /***
      * ���ն����ڴ�
      * @object: ��Ҫ�����յ�������е��ڴ棬��������ڴ���е��ڴ棬
      *          ��createʱ����Ӷѷ��䣬��ֱ���ͷŸ��ڴ棬��������ڳ��ڴ淶Χ�ڣ�
      *          �����Ƿ�Ϊ��ȷ�ĳ��ڴ棬���������ղ�����true�������������false
      * @return: ��������ջ�ɾ������true�����򷵻�false
      */
    bool reclaim(void* object);

    /** ���ص�����ز�����ʱ���Ƿ�Ӷ��Ϸ����ڴ� */
    bool use_heap() const;

    /** �õ�����ֵ��С */
    uint8_t get_guard_size() const;

    /** �õ��ش�С��Ҳ���ǳ��пɷ���Ķ������ */
    uint32_t get_pool_size() const;        

    /** �õ�����ؿɷ���Ķ����С */
    uint16_t get_object_size() const;

    /** �õ�������У���ǰ�����Է���Ķ������ */
    uint32_t get_available_number() const;
    
private:
    CLock _lock;
    CRawObjectPool _raw_object_pool;
};

SYS_NAMESPACE_END
#endif // _OBJECT_POOL_H

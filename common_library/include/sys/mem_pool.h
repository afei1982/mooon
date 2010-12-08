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
#ifndef _MEM_POOL_H
#define _MEM_POOL_H
#include "sys/lock.h"
SYS_NAMESPACE_BEGIN

/***
  * ���ڴ��ʵ�֣����ܸߵ����̰߳�ȫ
  */
class CRawMemPool
{
public:
    CRawMemPool();
    ~CRawMemPool();

    /** ������create�������ڴ�� */
    void destroy();

    /***
      * �����ڴ��
      * @bucket_size: �ڴ��С
      * @bucket_number: �ڴ����
      * @use_heap: �ڴ�ز���ʱ���Ƿ�Ӷ��Ϸ���
      * @guard_size: �����С
      * @guard_flag: �����ʶ
      */
    void create(uint16_t bucket_size, uint32_t bucket_number, bool use_heap=true, uint8_t guard_size=1, char guard_flag='m');

    /***
      * �����ڴ��ڴ�
      * @return: ����ڴ�ز������������˴Ӷ��Ϸ����ڴ棬�򷵻شӶ��Ϸ�����ڴ棬
      *          ��������ڴ�ز���ʱ����NULL�����򷵻ش��ڴ���з�����ڴ�
      */
    void* allocate();

    /***
      * �����ڴ��ڴ�
      * @bucket: ��Ҫ�����յ��ڴ���е��ڴ棬��������ڴ���е��ڴ棬
      *          ��createʱ����Ӷѷ��䣬��ֱ���ͷŸ��ڴ棬��������ڳ��ڴ淶Χ�ڣ�
      *          �����Ƿ�Ϊ��ȷ�ĳ��ڴ棬���������ղ�����true�������������false
      * @return: ��������ջ�ɾ������true�����򷵻�false
      */
    bool reclaim(void* bucket);

    /** ���ص��ڴ�ز�����ʱ���Ƿ�Ӷ��Ϸ����ڴ� */
    bool use_heap() const;

    /** �õ�����ֵ��С */
    uint8_t get_guard_size() const;

    /** �õ��ش�С��Ҳ���ǳ��пɷ�����ڴ���� */
    uint32_t get_pool_size() const;        

    /** �õ��ڴ�ؿɷ�����ڴ��С */
    uint16_t get_bucket_size() const;

    /** �õ��ڴ���У���ǰ�����Է�����ڴ���� */
    uint32_t get_available_number() const;

private:    
    bool _use_heap;             /** �ڴ�ز���ʱ���Ƿ�Ӷ��Ϸ��� */
    uint8_t _guard_size;        /** �����С��ʵ����Ҫ���ڴ��СΪ: (_guard_size+_bucket_size)*_bucket_number */
    uint16_t _bucket_size;      /** �ڴ��С������_guard_size���֣�����ʵ���ڴ��СӦ���ټ�ȥ_guard_size */
    uint32_t _bucket_number;    /** �ڴ���� */    
    volatile uint32_t _stack_top_index;  /** ջ������ */
    volatile uint32_t _available_number; /** ���л����Է�����ڴ���� */

private:
    char* _stack_top;    
    char* _stack_bottom;
    char** _bucket_stack;
    char* _bucket_bitmap; /** Ͱ״̬��������¼��ǰ״̬���Է�ֹ�ظ����� */    
};

/***
  * �̰߳�ȫ���ڴ�أ����ܽ�CRawMemPoolҪ��
  */
class CThreadMemPool
{
public:
    /** ������create�������ڴ�� */
    void destroy();

    /***
      * �����ڴ��
      * @bucket_size: �ڴ��С
      * @bucket_number: �ڴ����
      * @use_heap: �ڴ�ز���ʱ���Ƿ�Ӷ��Ϸ���
      * @guard_size: �����С
      * @guard_flag: �����ʶ
      */
    void create(uint16_t bucket_size, uint32_t bucket_number, bool use_heap=true, uint8_t guard_size=1, char guard_flag='m');

    /***
      * �����ڴ��ڴ�
      * @return: ����ڴ�ز������������˴Ӷ��Ϸ����ڴ棬�򷵻شӶ��Ϸ�����ڴ棬
      *          ��������ڴ�ز���ʱ����NULL�����򷵻ش��ڴ���з�����ڴ�
      */
    void* allocate();

    /***
      * �����ڴ��ڴ�
      * @bucket: ��Ҫ�����յ��ڴ���е��ڴ棬��������ڴ���е��ڴ棬
      *          ��createʱ����Ӷѷ��䣬��ֱ���ͷŸ��ڴ棬��������ڳ��ڴ淶Χ�ڣ�
      *          �����Ƿ�Ϊ��ȷ�ĳ��ڴ棬���������ղ�����true�������������false
      * @return: ��������ջ�ɾ������true�����򷵻�false
      */
    bool reclaim(void* bucket);

    /** ���ص��ڴ�ز�����ʱ���Ƿ�Ӷ��Ϸ����ڴ� */
    bool use_heap() const;

    /** �õ�����ֵ��С */
    uint8_t get_guard_size() const;

    /** �õ��ش�С��Ҳ���ǳ��пɷ�����ڴ���� */
    uint32_t get_pool_size() const;        

    /** �õ��ڴ�ؿɷ�����ڴ��С */
    uint16_t get_bucket_size() const;

    /** �õ��ڴ���У���ǰ�����Է�����ڴ���� */
    uint32_t get_available_number() const;
    
private:
    CLock _lock;
    CRawMemPool _raw_mem_pool;
};

SYS_NAMESPACE_END
#endif // _MEM_POOL_H

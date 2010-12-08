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
#include "util/array_queue.h"
SYS_NAMESPACE_BEGIN

/***
  * ������ʵ�֣����ܸߵ����̰߳�ȫ
  */
template <class ObjectClass>
class CRawObjectPool
{
public:
    /***
      * ����һ�����̰߳�ȫ��������
      * @use_heap: ����������޶���ʱ���Ƿ�Ӷ��д�������
      */
    CRawObjectPool(bool use_heap)
        :_use_heap(use_heap)
        ,_object_number(0)
        ,_object_array(NULL)
        ,_object_queue(NULL)
    {
    }

    /** ���������� */
    ~CRawObjectPool()
    {     
        destroy();
    }

    /***
      * ���������
      * @object_number: ��Ҫ�����Ķ������
      */
    void create(uint32_t object_number)
    {
        _object_number = object_number;
        _object_array = new ObjectClass[_object_number];
        _object_queue = new util::CArrayQueue<ObjectClass*>(_object_number);
        
        for (uint32_t i=0; i<_object_number; ++i)
        {
            ObjectClass* object = &_object_array[i];
            object->set_index(i+1); // Index���Ǵ���0��0��Ϊ��Ч��ʶ
            object->set_in_queue(true);

            _object_queue->push_back(object);
        }
    }

    /** ���ٶ���� */
    void destroy()
    {
        delete []_object_queue;        
        delete []_object_array;

        _object_queue = NULL;
        _object_array = NULL;
    }

    /***
      * �Ӷ�����н���һ�����󣬲��������Ƿ��ڳ��е�״̬����Ϊfalse
      * @return: ��������Ϊ�գ�������Ӷ��д���������Ӷ��ϴ���һ���¶��󣬲���������
      *          ��������Ϊ�գ��Ҳ�����Ӷ��д��������򷵻�NULL��
      *          �������ز�Ϊ�գ���ӳ���ȡһ�����󣬲�����ָ����������ָ��
      */
    ObjectClass* borrow()
    {
        ObjectClass* object = NULL;
        
        // �������Ϊ�գ����Ƿ�Ӷ��д����¶�����������ԣ��򷵻�NULL
        if (_object_queue->is_empty()) 
        {
            if (_use_heap)
            {
                object = new ObjectClass;
                object->set_index(0); // indexΪ0����ʾ���Ƕ�����еĶ���
            }
        }
        else
        {
            object = _object_queue->pop_front();
            object->set_in_queue(false);
        }        

        return object;
    }

    /***
      * ��һ������黹�������      
      * @object: ָ����黹������صĶ���ָ�룬������󲢲��Ƕ�����еĶ�����delete����
      *          �������Żض���أ������Ƿ��ڶ�����е�״̬����Ϊtrue
      */
    void pay_back(ObjectClass* object)
    {
        // ����Ƕ�����еĶ���
        if (0 == object->get_index())
        {       
            delete object;
        }
        else
        {
            // ������ڶ�����
            if (!object->is_in_queue())
            {
                _object_queue->reset();
                _object_queue->set_in_queue(true);
                _object_queue->push_back(object);
            }
        }
    }

private:
    bool _use_heap;
    uint32_t _object_number;
    ObjectClass* _object_array;
    util::CArrayQueue<ObjectClass*>* _object_queue;
};

/***
  * �̰߳�ȫ�Ķ���أ����ܽ�CRawObjectPool��
  */
template <class ObjectClass>
class CThreadObjectPool
{
public: 
    /***
      * ����һ���̰߳�ȫ��������
      * @use_heap: ����������޶���ʱ���Ƿ�Ӷ��д�������
      */
    CThreadObjectPool(bool use_heap)
        :_raw_object_pool(use_heap)
    {        
    }   
    
    /***
      * ���������
      * @object_number: ��Ҫ�����Ķ������
      */
    void create(uint32_t object_number)
    {
        CLockHelper<CLock> lock_helper(_lock);
        _raw_object_pool.create(object_number);
    }

    /** ���ٶ���� */
    void destroy()
    {
        CLockHelper<CLock> lock_helper(_lock);
        _raw_object_pool.destroy();
    }

    /** �����ؽ���һ������ */
    ObjectClass* borrow()
    {
        CLockHelper<CLock> lock_helper(_lock);
        return _raw_object_pool.borrow();
    }

    /** ��һ������黹������� */
    void pay_back(ObjectClass* object)
    {
        CLockHelper<CLock> lock_helper(_lock);
        _raw_object_pool.pay_back(object);
    }
    
private:
    CLock _lock;
    CRawObjectPool _raw_object_pool;
};

SYS_NAMESPACE_END
#endif // _OBJECT_POOL_H

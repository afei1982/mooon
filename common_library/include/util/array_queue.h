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
 * Author: JianYi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef UTIL_ARRAY_QUEUE_H
#define UTIL_ARRAY_QUEUE_H
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/** ������ʵ�ֵĶ���, ���̰߳�ȫ */
template <typename DataType>
class CArrayQueue
{       
public:
    /** �����е�Ԫ���������� */
    typedef DataType _DataType;

    /***
      * ����һ���������
      * @queue_max: ��Ҫ����Ķ��д�С
      */
	CArrayQueue(uint32_t queue_max)
		:_tail(0)
		,_head(0)
		,_queue_size(0)
    {
        _queue_max = queue_max+1;
		_elem_array = new DataType[queue_max];        
        memset(_elem_array, 0, queue_max);
    }
	
    ~CArrayQueue()
    {
        delete []_elem_array;
    }
        
    /** �ж϶����Ƿ����� */
    bool is_full() const 
	{
        return ((_tail+1) % _queue_max == _head);
    }
    
    /** �ж϶����Ƿ�Ϊ�� */
    bool is_empty() const 
	{
        return (_head == _tail);
    }

    /** ���ض���Ԫ�� */
    DataType front() const 
	{
        return _elem_array[_head];
    }
    
	/***
      * ��������Ԫ��
      * ע��: ����pop֮ǰӦ����ʹ��is_empty�ж�һ��
      * @return: ���ض���Ԫ��
      */
    DataType pop_front() 
	{
        DataType elem = _elem_array[_head];
        _head = (_head+1) % _queue_max;
        --_queue_size;
        return elem;
    }
    
	/***
      * ����β����һ��Ԫ��
      * ע��: ����pop֮ǰӦ����ʹ��is_full�ж�һ��
      */
    void push_back(DataType elem) 
	{
        _elem_array[_tail] = elem;
        _tail = (_tail+1) % _queue_max; 
        ++_queue_size;
    }

    /** �õ������д洢��Ԫ�ظ��� */
    uint32_t size() const 
	{ 
		return _queue_size; 
	}
    
private:        
    uint32_t _tail;       /** ���� */
    uint32_t _head;       /** ��β */    
    uint32_t _queue_size; /** ���е�ǰ���ɵ�Ԫ�ظ��� */	
	uint32_t _queue_max;  /** �����������ɵ�Ԫ�ظ��� */	
	DataType* _elem_array; /** ����ʵ�ֶ��е����� */
};

UTIL_NAMESPACE_END
#endif // UTIL_ARRAY_QUEUE_H

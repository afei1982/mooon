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
#ifndef _HISTOGRAM_ARRAY_H
#define _HISTOGRAM_ARRAY_H
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/***
  * ֱ��ͼ����
  */
template <typename DataType>
class CHistogramArray
{
public:
    /** ����洢��Ԫ������ */
    typedef DataType _DataType;
    
    /***
      * ����һ��ֱ��ͼ����
      * @array_size: ֱ��ͼ�����С
      */
    CHistogramArray(uint32_t array_size)
        :_array_size(array_size)
    {
        _histogram_size_array = new uint32_t[array_size];
        _elem_array = new DataType*[array_size];

        for (uint32_t i=0; i<array_size; ++i)
        {
            _elem_array[i] = NULL;
            _histogram_size_array[i] = 0; // ��ʼ��ÿ������СΪ0
        }
    }

    /** ����ֱ��ͼ���� */
    ~CHistogramArray()
    {
        for (uint32_t i=0; i<_array_size; ++i)
            delete []_elem_array[i];
        delete [](DataType**)_elem_array;
    }

    /***
      * ����Ԫ�ص�ָ����ֱ��ͼ��
      * @position: ֱ��ͼ�������е�λ��
      * @elem: ��Ҫ�����Ԫ��
      * @unique: �Ƿ���ȥ�ؼ�飬����ǣ����ظ�������ͬ��Ԫ��
      * @return: ����ɹ�����true�����򷵻�false
      */
    bool insert(uint32_t position, DataType elem, bool unique=true)
    {
        if (!histogram_exist(position))
        {
            _elem_array[position] = new DataType[1];
            _elem_array[position][0] = elem;

            ++_histogram_size_array[position];
            return true;
        }
        else
        {                    
            uint32_t histogram_size = get_histogram_size(position);
            if (unique)
            {
                for (uint32_t i=0; i<histogram_size; ++i)
                    if (_elem_array[position][i] == elem)
                        return false;
            }

            DataType* elem_array = new DataType[histogram_size+1];
            memcpy(&elem_array[0], _elem_array[position], histogram_size);
        
            delete []_elem_array[position];
            _elem_array[position] = elem_array;

            _elem_array[position][histogram_size] = elem;
            ++_histogram_size_array[position];
        
            return true;
        }
    }

    /***
      * ��ֱ��ͼ��ɾ��һ��Ԫ��
      * @position: ֱ��ͼ�������е�λ��
      * @elem: ��Ҫɾ����Ԫ��
      * @return: ���Ԫ����ֱ��ͼ�У��򷵻�true�����򷵻�false
      */
    bool remove(uint32_t position, DataType elem)
    {
        /** ֱ��ͼ������ */
        DataType* histogram = get_histogram(position);
        if (NULL == histogram) return false;

        uint32_t histogram_size = get_histogram_size(position);
        for (uint32_t i=0; i<histogram_size; ++i)
        {
            if (_elem_array[position][i] == elem)
            {
                DataType* elem_array = NULL;
                if (histogram_size > 1)
                {
                    elem_array = new DataType[histogram_size-1];
                    memcpy(&elem_array[0], &_elem_array[position][0], i);
                    memcpy(&elem_array[i], &_elem_array[position][i+1], histogram_size-i-1);
                }                                              

                delete []_elem_array[position];
                _elem_array[position] = elem_array;
                --_histogram_size_array[position];

                return true;
            }
        }

        return false;
    }

    /** �õ������ɵ�ֱ��ͼ���� */
    uint32_t get_capacity() const
    {
        return _array_size;
    }

    /** ���һ��ֱ��ͼ�Ƿ���� */
    bool histogram_exist(uint32_t position) const
    {        
        return get_histogram(position) != NULL;
    }

    /** �õ�ֱ��ͼ��С */
    uint32_t get_histogram_size(uint32_t position) const
    {
        return (position < get_capacity())? _histogram_size_array[position]: 0;
    }

    /** �õ�ֱ��ͼ */
    DataType* get_histogram(uint32_t position) const
    {
        return (position < get_capacity())? _elem_array[position]: NULL;
    }
    
private:
    uint32_t _array_size;            /** ֱ��ͼ�����С */    
    DataType** _elem_array;          /** ֱ��ͼԪ������ */
    uint32_t* _histogram_size_array; /** ֱ��ͼ��С���� */
};

UTIL_NAMESPACE_END
#endif // _HISTOGRAM_ARRAY_H

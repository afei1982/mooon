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
#ifndef AGENT_RESOURCE_PROVIDER_H
#define AGENT_RESOURCE_PROVIDER_H
#include <util/util_config.h>
MOOON_NAMESPACE_BEGIN

/***
  * ϵͳ��Դ�ṩ��
  */
class IResourceProvider
{
public:
    /** �����������������α��������� */
    virtual ~IResourceProvider() {}

    /** �õ�����CPU�ٷֱ� */
    virtual uint32_t get_cpu() const = 0;

    /** �õ��ܵ������ڴ���(��λ: MB) */
    virtual uint32_t get_mem_total() const = 0;

    /** �õ����������ڴ���(��λ: MB) */
    virtual uint32_t get_mem_free() const = 0;

    /** �õ�����Buffer�������ڴ���(��λ: MB) */
    virtual uint32_t get_mem_buffer() const = 0;

    /** �õ�����Cache�������ڴ���(��λ: MB) */
    virtual uint32_t get_mem_cache() const = 0;

    /** �õ������ռ��ܴ�С(��λ: MB) */
    virtual uint32_t get_swap_tatol() const = 0;

    /** �õ����еĽ����ռ��С(��λ: MB) */
    virtual uint32_t get_swap_free() const = 0;

    /** �õ���ǰ��������(��λ: KB) */
    virtual uint32_t get_net_traffic() const = 0;

    /** �õ���ǰ����ռ�õ�CPU�ٷֱ� */
    virtual uint32_t get_process_cpu() const = 0;

    /** �õ���ǰ������ռ�õ������ڴ� */
    virtual uint32_t get_process_mem() const = 0;
};

MOOON_NAMESPACE_END
#endif // AGENT_RESOURCE_PROVIDER_H

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
#ifndef AGENT_H
#define AGENT_H
#include "agent/config_observer.h"
#include "agent/resource_provider.h"
MOOON_NAMESPACE_BEGIN

/***
  * Agent�ӿڣ����Ⱪ¶Agent������
  */
class IAgent
{
public:
    /** �����⺯��Ӧ���������ĸ澯 */
    virtual ~IAgent() {}    

    /** �õ���Դ�ṩ�� */
    virtual IResourceProvider* get_resource_provider() const = 0;
    
    /** �ϱ�״̬
      * @data: ���ϱ�������
      * @data_size: ���ϱ����ݵ��ֽ�����С
      */
    virtual void report(const char* data, size_t data_size) = 0;

    /***
      * ֧�ֶ�center��һ��center���Ӳ���ʱ���Զ��л�
      */
    virtual void add_center(const net::ip_address_t& ip_address) = 0;    

    /***
      * ע�����ù۲���
      * @config_name: ��������
      * @config_observer: ���ù۲���
      * @return: �����ͬ����ע���򷵻�false�����򷵻�true
      */
    virtual bool register_config_observer(const char* config_name, IConfigObserver* config_observer) = 0;

    /***
      * ע�����ù۲���
      * @config_name: ��������
      */
    virtual void deregister_config_observer(const char* config_name, IConfigObserver* config_observer) = 0;    

    /***
      * ע���������
      * @command: �������
      * @command_processor: ��Ҫע�����������
      */
    virtual void deregister_commoand_processor(uint16_t command, ICommandProcessor* command_processor) = 0;

    /***
      * ע���������
      * @command: �������
      * @command_processor: �������
      * @exclusive: �Ƿ��ռ�����������commandֻ����һ���������
      * @return: ����Ѿ����ڶ�ռ�ģ���ע��ʧ�ܷ���false�����򷵻�true
      */
    virtual bool register_commoand_processor(uint16_t command, ICommandProcessor* command_processor, bool exclusive) = 0;
};

/***
  * �õ�ָ��Agent��ָ��
  */
extern "C" IAgent* get_agent();

/***
  * ����ɾ�������õ�Agent����create_agent����ɶԵ��ã����Ҿ�Ϊ���̰߳�ȫ��
  * ������������ʱ�������߳��е���create_agent���ڽ����˳�ʱ����destroy_agent
  */
extern "C" void destroy_agent();

/***
  * ����Agentȫ��Ψһʵ����ע��÷������̰߳�ȫ����destroy_agent����ɶԵ���
  * @logger: ����Agent����־��
  * @return: ��������ɹ�����ָ��Agent��ָ�򣬷��򷵻�NULL
  */
extern "C" IAgent* create_agent(sys::ILogger* logger);

MOOON_NAMESPACE_END
#endif // AGENT_H

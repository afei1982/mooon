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
 * Author: JianYi, eyjian@qq.com
 */
#ifndef GENERAL_SERVER_H
#define GENERAL_SERVER_H
#include <sys/log.h>
#include <net/ip_address.h>
#include "server/packet_handler.h"
MOOON_NAMESPACE_BEGIN

/***
  * ��ܹ����ص��ӿڣ������������Ľ������ͱ��Ĵ�����
  */
class CALLBACK_INTERFACE IServerFactory
{
public:    
    /** ���������������������α������澯 */
    virtual ~IServerFactory() {}
    
    /** ������������ */
    virtual IPacketHandler* create_packet_handler() = 0;

    /** ����Э������� */
    virtual IProtocolParser* create_protocol_parser() = 0;    

    /** ����������Ӧ */
    virtual IRequestResponsor* create_request_responsor(IProtocolParser* parser) = 0;
};

/***
  * ������ûص��ӿ�
  */
class CALLBACK_INTERFACE IServerConfig
{
public:
    /** ���������������������α������澯 */
    virtual ~IServerConfig() {}

    /** �õ�epoll��С */
    virtual uint32_t get_epoll_size() const = 0;
        
    /** �õ�epool�ȴ���ʱ������ */
    virtual uint32_t get_epoll_timeout() const = 0;

    /** �õ���ܵĹ����̸߳��� */
    virtual uint16_t get_thread_number() const = 0;

    /** �õ����ӳش�С */
    virtual uint32_t get_connection_pool_size() const = 0;

    /** ���ӳ�ʱ���� */
    virtual uint32_t get_connection_timeout_seconds() const = 0;

    /** �õ��������� */    
    virtual const net::ip_port_pair_array_t& get_listen_parameter() const = 0;
};

/** ͨ�÷��������
  */
class IServer
{ 
public:
    /** ���������������������α������澯 */
    virtual ~IServer() {}

    /** ֹͣServer */
    virtual void stop() = 0;

    /** ����Server */
    virtual bool start() = 0;
};

/** ����Server */
extern "C" void destroy_server(IServer* server);

/***
  * ����Server
  * @logger: ��־��
  * @config: Server����
  * @factory: Server����
  */
extern "C" IServer* create_server(sys::ILogger* logger, IServerConfig* config, IServerFactory* factory);

MOOON_NAMESPACE_END
#endif // GENERAL_SERVER_H

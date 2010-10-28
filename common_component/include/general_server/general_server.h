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
#include "general_server/packet_handler.h"
MOOON_NAMESPACE_BEGIN

/***
  * ��ܹ����ص��ӿڣ������������Ľ������ͱ��Ĵ�����
  */
class CALLBACK_INTERFACE IFrameFactory
{
public:    
    /** ���������������������α������澯 */
    virtual ~IGtfFactory() {}
    
    virtual IPacketHandler* create_packet_handler() = 0;
    virtual IProtocolParser* create_protocol_parser() = 0;    
    virtual IResponsor* create_responsor(IProtocolParser* parser) = 0;
};

/***
  * ������ûص��ӿ�
  */
class CALLBACK_INTERFACE IFrameConfig
{
public:
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
    const net::ip_port_pair_array_t& get_listen_parameter() const = 0;
};

/** ͨ�÷��������
  */
class IGeneralServer
{ 
public:
    /** ���������������������α������澯 */
    virtual ~IGeneralServer() {}

    /** ֹͣServer */
    virtual void stop() = 0;

    /** ����Server */
    virtual bool start() = 0;
};

// ȫ�ֵ���C����
extern "C" void destroy_general_server(IGeneralServer* general_server);
extern "C" IGeneralServer* create_general_server(sys::ILogger* logger, IFrameConfig* frame_config, IFrameFactory* frame_factory);

MOOON_NAMESPACE_END
#endif // GENERAL_SERVER_H

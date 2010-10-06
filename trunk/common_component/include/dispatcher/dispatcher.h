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
#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "util/util_config.h"
MY_NAMESPACE_BEGIN

/***
  * �ַ���Ϣ�ṹ
  */
typedef struct
{
    uint32_t length;   /** content���ֽ��� */
    char content[1];   /** ��Ϣ���� */
}dispach_message_t;

/***
  * �����߽ӿ�
  */
class ISender
{
public:    
    // ����������Ӧ��������
    virtual ~ISender() {}

    /***
      * ������Ϣ
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      */
    virtual bool send_message(dispach_message_t* message) = 0;
};

/***
  * Ӧ����Ϣ������
  */
class CALLBACK_INTERFACE IReplyHandler
{
public:
    // ����������Ӧ��������
    virtual ~IReplyHandler() {}    

    /** �õ��洢Ӧ����Ϣ��buffer */
    virtual char* get_buffer() const = 0;

    /** �õ��洢Ӧ����Ϣ��buffer��С */
    virtual size_t get_buffer_length() const = 0;    

    /** ����Ӧ����Ϣ */
    virtual bool handle_reply(size_t data_size) = 0;
};

/***
  * Ӧ����Ϣ��������
  * ÿ���߳�ͨ�����������Լ���Ӧ����Ϣ�����������һ��Ӧ����Ϣ��������ֻ�ᱻһ���߳�ʹ��
  */
class CALLBACK_INTERFACE IReplyHandlerFactory
{
public:
    // ����������Ӧ��������
    virtual ~IReplyHandlerFactory() {}

    virtual IReplyHandler* create_reply_handler() = 0;
    virtual void destroy_reply_handler(IReplyHandler* reply_handler) = 0;
};

/***
  * ��Ϣ�ַ����ӿ�
  */
class IDispatcher
{
public:    
    // ����������Ӧ��������
    virtual ~IDispatcher() {}

    /***
      * �ͷ�һ�������ߣ������get_sender�ɶԵ���
      */
    virtual void release_sender(ISender* sender) = 0;

    /***
      * ����һ��IPV4��ַ�õ�һ�������ߣ������release_sender�ɶԵ���
      * @node_ip: �������͵�IPV4��ַ
      */
    virtual ISender* get_sender(uint32_t node_ip) = 0;

    /***
      * ����һ��IPV6��ַ�õ�һ�������ߣ������release_sender�ɶԵ���
      * @node_ip: 16�ֽ����͵�IPV6��ַ
      */
    virtual ISender* get_sender(uint8_t* node_ip) = 0;    
    
    /** �����̳߳��е��̸߳��� */
    virtual void set_thread_count(uint16_t thread_count);

    /** ����Ӧ����Ϣ������ */
    virtual void set_reply_handler_factory(IReplyHandlerFactory* reply_handler_factory) = 0;
    
    /***
      * ������Ϣ
      * @node_id: �ڵ�ID
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      */
    virtual bool send_message(uint16_t node_id, dispach_message_t* message) = 0; 
    
    /***
      * ������Ϣ
      * @node_ipv: �ڵ��IPV4��ַ
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      */
    virtual bool send_message(uint32_t node_ip, dispach_message_t* message) = 0;
    
    /***
      * ������Ϣ
      * @node_ipv: �ڵ��IPV6��ַ
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      */
    virtual bool send_message(uint8_t* node_ip, dispach_message_t* message) = 0; 
};

//////////////////////////////////////////////////////////////////////////
// ȫ��C��������

/** ���ٷַ��������̰߳�ȫ */
extern "C" void destroy_dispatcher();

/** �õ��ַ��������̰߳�ȫ */
extern "C" IDispatcher* get_dispatcher();

MY_NAMESPACE_END
#endif // DISPATCHER_H

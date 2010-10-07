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
#include "net/ip_node.h"
MY_NAMESPACE_BEGIN


/***
  * ���ʽ���
  *
  * @Dispatcher: ��Ϣ�ַ������ṩ����Ϣ����Ŀ��IP�Ͷ˿ڵ�����
  * @Sender: ִ�н���Ϣ����Ŀ��IP�Ͷ˿�
  * @SenderThread: ��Ϣ���ͳ��̣߳�����Sender����Ϣ����Ŀ��IP�Ͷ˿�
  * @ReplyHandler: ��ϢӦ������������Զ˵�Ӧ�𣬺�SenderThreadһһ��Ӧ��
  *                ��һ��ReplyHandlerֻ��һ��SenderThreadΨһ����
  * @ReplyHandlerFactory: ��ϢӦ��������������SenderThread�����������Լ�����ϢӦ����
  */

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
      * ��ʼ����Ϣ�ַ���
      * @queue_size: ÿ��Sender�Ķ��д�С
      * @thread_count: ��Ϣ�����̸߳���
      */
    virtual bool create(uint32_t queue_size, uint16_t thread_count) = 0;

    /** ������Ϣ�ַ���������create�ɶԵ��� */
	virtual void destroy() = 0;        

    /***
      * �ͷ�һ�������ߣ������get_sender�ɶԵ���
      */
    virtual void release_sender(ISender* sender) = 0;

    /** �ر�Sender */
    virtual void close_sender(const net::ipv4_node_t& ip_node) = 0;
    virtual void close_sender(const net::ipv6_node_t& ip_node) = 0;
    
    /***
      * ����IP�Ͷ˿ڵõ�һ��Sender�������release_sender�ɶԵ���
      * @ip: ��Ϣ������IP��ַ
      */
    virtual ISender* get_sender(const net::ipv4_node_t& ip_node) = 0;      
    virtual ISender* get_sender(const net::ipv6_node_t& ip_node) = 0;        

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
      * @ip: ��Ϣ�����͵�IP��ַ
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      */
    virtual bool send_message(const net::ipv4_node_t& ip_node, dispach_message_t* message) = 0; 
    virtual bool send_message(const net::ipv6_node_t& ip_node, dispach_message_t* message) = 0; 
};

//////////////////////////////////////////////////////////////////////////
// ȫ��C��������

/** ������Ϣ�ַ��������̰߳�ȫ */
extern "C" void destroy_dispatcher();

/***
  * �õ�һ��Ψһ����Ϣ�ַ�������һ�ε��÷��̰߳�ȫ��
  * ���Ա��뱣֤��һ��ֻ��һ���̵߳������� ͨ�����������߳��е��ã�����ɳ�ʼ��
  */
extern "C" IDispatcher* get_dispatcher();

MY_NAMESPACE_END
#endif // DISPATCHER_H

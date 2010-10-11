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
#include <sys/log.h>
#include <net/ip_node.h>
#include <net/ip_address.h>

/**
  * ���ܿ��ƺ�
  */
#define ENABLE_CONFIG_UPDATE     0  /** �Ƿ�������ʵʱ���¹��ܣ���ҪAgent֧�� */
#define ENABLE_LOG_STATE_DATA    0  /** �Ƿ�����¼״̬���ݹ��ܣ���ҪObserver֧�� */
#define ENABLE_REPORT_STATE_DATA 0  /** �Ƿ����ϱ�״̬���ݹ��ܣ���ҪAgent֧�� */

MY_NAMESPACE_BEGIN

/***
  * ���ʽ���
  *
  * @Dispatcher: ��Ϣ�ַ������ṩ����Ϣ����Ŀ��IP�Ͷ˿ڵ�����
  * @Sender: ִ�н���Ϣ����Ŀ��IP�Ͷ˿�
  * @SenderThread: ��Ϣ���ͳ��̣߳�����Sender����Ϣ����Ŀ��IP�Ͷ˿�
  * @ReplyHandler: ��ϢӦ������������Զ˵�Ӧ�𣬺�Senderһһ��Ӧ��
  *                ��һ��ReplyHandlerֻ��һ��SenderΨһ����
  * @ReplyHandlerFactory: ��ϢӦ������������������Ϊÿ��Sender������ϢӦ����
  */

/** �������� */
enum
{
    DEFAULT_RECONNECT_TIMES       = 10,  /** Ĭ�ϵ�������������Ӵ��� */
    DEFAULT_MESSAGE_MERGED_NUMBER = 10,  /** Ĭ�ϵ���ཫ���ٸ���Ϣ�ϲ���һ������Ϣ */
    MAX_MESSAGE_MERGED_NUMBER     = 30   /** �����Խ����ٸ���Ϣ�ϲ���һ������Ϣ */
};

/** Ӧ������ֵ */
typedef enum
{
    reply_error,    /** ����Ӧ����Ϣ���� */
    reply_finish,   /** Ӧ����Ϣ�Ѿ����� */
    reply_continue  /** Ӧ����Ϣδ��������Ҫ���� */
}reply_return_t;

/***
  * �ַ���Ϣ�ṹ
  * Ҫ��length��contentλ��ͬһ���������ڴ�
  */
typedef struct
{
    uint32_t length;   /** content���ֽ��� */
    char content[0];   /** ��Ϣ���� */
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
  * Ӧ����Ϣ��������ÿ��Sender�����Ӧһ��Ӧ����Ϣ������
  */
class CALLBACK_INTERFACE IReplyHandler
{
public:
    // ����������Ӧ��������
    virtual ~IReplyHandler() {}    

    /** �õ��洢Ӧ����Ϣ��buffer */
    virtual char* get_buffer() = 0;

    /** �õ��洢Ӧ����Ϣ��buffer��С */
    virtual uint32_t get_buffer_length() const = 0;    

    /** �����߱��ر��ˣ�ֻ�з����ڴ���Ӧ����Ϣ�����вŻᴦ�� */
    virtual void sender_closed(int32_t node_id, const net::ip_address_t& peer_ip, uint16_t peer_port) {}

    /** ����Ӧ����Ϣ */
    virtual reply_return_t handle_reply(int32_t node_id, const net::ip_address_t& peer_ip, uint16_t peer_port, uint32_t data_size) = 0;
};

/***
  * Ӧ����Ϣ��������������
  */
class CALLBACK_INTERFACE IReplyHandlerFactory
{
public:
    // ����������Ӧ��������
    virtual ~IReplyHandlerFactory() {}

    /** ����Ӧ����Ϣ������ */
    virtual IReplyHandler* create_reply_handler() = 0;

    /** ����Ӧ����Ϣ������ */
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

    /** �ر���Ϣ�ַ���������open�ɶԵ��� */
	virtual void close() = 0;  

    /***
      * ��ʼ����Ϣ�ַ���
      * @dispatch_table: �ַ����ļ���
      * @queue_size: ÿ��Sender�Ķ��д�С
      * @thread_count: ��Ϣ�����̸߳���
      * @message_merged_number: �ϲ��ɴ���Ϣ���͵ĵ���Ϣ����
      * @reply_handler_factory: Ӧ����Ϣ��������������
      */
    virtual bool open(const char* dispatch_table, uint32_t queue_size, uint16_t thread_count, uint16_t message_merged_number, IReplyHandlerFactory* reply_handler_factory=NULL) = 0;     

    /***
      * �ͷ�һ�������ߣ������get_sender�ɶԵ��ã���ֻ��UnmanagedSender��Ч
      */
    virtual void release_sender(ISender* sender) = 0;

    /** �ر�Sender��ֻ��UnmanagedSender��Ч */
    virtual void close_sender(const net::ipv4_node_t& ip_node) = 0;
    virtual void close_sender(const net::ipv6_node_t& ip_node) = 0;
    
    /***
      * ����IP�Ͷ˿ڵõ�һ��Sender�������release_sender�ɶԵ��ã�
      * ֻ��UnmanagedSender��Ч
      * @ip: ��Ϣ������IP��ַ
      */
    virtual ISender* get_sender(const net::ipv4_node_t& ip_node) = 0;      
    virtual ISender* get_sender(const net::ipv6_node_t& ip_node) = 0;        

    /** �õ��ɹ����Sender���� */
    virtual uint16_t get_managed_sender_number() const = 0;

    /** ��������������� */
    virtual void set_reconnect_times(uint32_t reconnect_times) = 0;          
    
    /***
      * ������Ϣ
      * @node_id: �ڵ�ID
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      * @ע������: �������false���������Ӧ��ɾ����Ϣ����free(message)��
      *            ������Ϣ����Dispatcher��ɾ����
      *            ������Ϣ�ڴ������malloc��calloc��realloc�����ġ�
      *            
      */
    virtual bool send_message(uint16_t node_id, dispach_message_t* message) = 0; 
    
    /***
      * ������Ϣ
      * @ip: ��Ϣ�����͵�IP��ַ
      * @message: ��Ҫ���͵���Ϣ
      * @return: ������Ͷ���������false�����򷵻�true
      * @ע������: �������false���������Ӧ��ɾ����Ϣ����free(message)��
      *            ������Ϣ����Dispatcher��ɾ����
      *            ������Ϣ�ڴ������malloc��calloc��realloc�����ġ�
      */
    virtual bool send_message(const net::ipv4_node_t& ip_node, dispach_message_t* message) = 0; 
    virtual bool send_message(const net::ipv6_node_t& ip_node, dispach_message_t* message) = 0; 
};

//////////////////////////////////////////////////////////////////////////
// ȫ��C��������

extern "C" void destroy_dispatcher();      /** ������Ϣ�ַ������ */
extern "C" IDispatcher* get_dispatcher();  /** �����Ϣ�ַ������ */
extern "C" IDispatcher* create_dispatcher(sys::ILogger* logger); /** ������Ϣ�ַ������ */

MY_NAMESPACE_END
#endif // DISPATCHER_H

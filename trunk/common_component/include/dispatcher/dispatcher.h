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

MOOON_NAMESPACE_BEGIN

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
    DEFAULT_RECONNECT_TIMES = 10  /** Ĭ�ϵ�������������Ӵ��� */    
};

/***
  * �ַ���Ϣ����
  */
typedef enum
{
    dispatch_file,   /** ��Ҫ���͵���һ���ļ� */
    dispatch_buffer  /** ��Ҫ���͵���һ��Buffer */
}dispatch_message_type_t;

/***
  * �ַ���Ϣͷ
  */
typedef struct
{
    dispatch_message_type_t type; /** �ַ���Ϣ���� */
    uint32_t length;              /** �ļ���С��content���ֽ��� */    
}dispatch_message_t;

/***
  * �ַ��ļ�������Ϣ�ṹ
  */
typedef struct 
{
    dispatch_message_t header; /** �ַ���Ϣͷ */
    int32_t fd;                /** ��Ҫ���͵��ļ������� */
    uint32_t offset;           /** �ļ�ƫ�ƣ����ļ��ĸ�λ�ÿ�ʼ���� */
}dispatch_file_message_t;

/***
  * �ַ�Buffer������Ϣ�ṹ
  */
typedef struct
{
    dispatch_message_t header; /** �ַ���Ϣͷ */
    char content[0];           /** ��Ҫ���͵���Ϣ���� */
}dispatch_buffer_message_t;

/***
  * �����߽ӿ�
  */
class ISender
{
public:    
    // ����������Ӧ��������
    virtual ~ISender() {}
    
    /***
      * ������Ϣ�ط�����
      * @resend_times: �ط����������Ϊ-1��ʾһֱ�ط�ֱ���ɹ����ͳ�ȥ��
      *                ���Ϊ0��ʾ���ط��������ط�ָ������
      */
    virtual void set_resend_times(int8_t resend_times) = 0;

    /***
      * ������Ϣ
      * @message: ��Ҫ���͵���Ϣ
      * @milliseconds: �ȴ����ͳ�ʱ�����������Ϊ0��ʾ���ȴ��������أ�����
      *                �ȴ���Ϣ�ɴ�����У�ֱ����ʱ����
      * @return: �����Ϣ������У��򷵻�true�����򷵻�false
      */
    virtual bool send_message(dispatch_message_t* message, uint32_t milliseconds=0) = 0;
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

    /***
      * ÿһ����Ϣ������ǰ����
      * @route_id: ��Ϣ��·��ID�����ΪUnmanagedSender����Ϊ-1
      * @peer_ip: ��Ϣ���͵�Ŀ��IP��ַ
      * @peer_port: ��Ϣ������Ŀ��˿ں�
      */
    virtual void before_send(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port) {}
    
    /***
      * ��ǰ��Ϣ�Ѿ��ɹ��������
      * @route_id: ��Ϣ��·��ID�����ΪUnmanagedSender����Ϊ-1
      * @peer_ip: ��Ϣ���͵�Ŀ��IP��ַ
      * @peer_port: ��Ϣ������Ŀ��˿ں�
      */
    virtual void send_finish(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port) {}

    /***
      * ��Ŀ������ӶϿ�
      * @route_id: ��Ϣ��·��ID�����ΪUnmanagedSender����Ϊ-1
      * @peer_ip: ��Ϣ���͵�Ŀ��IP��ַ
      * @peer_port: ��Ϣ������Ŀ��˿ں�
      */
    virtual void sender_closed(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port) {}

    /***
      * ��Ŀ��ɹ���������
      * @route_id: ��Ϣ��·��ID�����ΪUnmanagedSender����Ϊ-1
      * @peer_ip: ��Ϣ���͵�Ŀ��IP��ַ
      * @peer_port: ��Ϣ������Ŀ��˿ں�
      */
    virtual void sender_connected(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port) {}

    /***
      * ���ӵ�Ŀ��ʧ��
      * @route_id: ��Ϣ��·��ID�����ΪUnmanagedSender����Ϊ-1
      * @peer_ip: ��Ϣ���͵�Ŀ��IP��ַ
      * @peer_port: ��Ϣ������Ŀ��˿ں�
      */
    virtual void sender_connect_failure(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port) {}

    /***
      * �յ���Ӧ�����ݣ�����Ӧ����
      * @route_id: ��Ϣ��·��ID�����ΪUnmanagedSender����Ϊ-1
      * @peer_ip: ��Ϣ���͵�Ŀ��IP��ַ
      * @peer_port: ��Ϣ������Ŀ��˿ں�
      * @data_size: �����յ��������ֽ���
      */
    virtual util::handle_result_t handle_reply(int32_t route_id, const net::ip_address_t& peer_ip, uint16_t peer_port, uint32_t data_size) = 0;
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
      * @route_table: ·�ɱ��ļ���
      * @queue_size: ÿ��Sender�Ķ��д�С
      * @thread_count: ��Ϣ�����̸߳���
      * @reply_handler_factory: Ӧ����Ϣ��������������
      */
    virtual bool open(const char* route_table, uint32_t queue_size, uint16_t thread_count, IReplyHandlerFactory* reply_handler_factory=NULL) = 0;     

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

    /** �õ��ɹ����Sender��ID���� */
    virtual const uint16_t* get_managed_sender_array() const = 0;

    /** ��������������� */
    virtual void set_reconnect_times(uint32_t reconnect_times) = 0;          
    
    /***
      * ������Ϣ�ط�����
      * @resend_times: �ط����������Ϊ-1��ʾһֱ�ط�ֱ���ɹ����ͳ�ȥ��
      *                ���Ϊ0��ʾ���ط��������ط�ָ������
      */
    virtual void set_resend_times(uint16_t route_id, int8_t resend_times) = 0;
    virtual void set_resend_times(const net::ipv4_node_t& ip_node, int8_t resend_times) = 0;
    virtual void set_resend_times(const net::ipv6_node_t& ip_node, int8_t resend_times) = 0;

    /***
      * ������Ϣ
      * @route_id: ·��ID
      * @message: ��Ҫ���͵���Ϣ
      * @milliseconds: �ȴ����ͳ�ʱ�����������Ϊ0��ʾ���ȴ��������أ�����
      *                �ȴ���Ϣ�ɴ�����У�ֱ����ʱ����
      * @return: �����Ϣ������У��򷵻�true�����򷵻�false
      * @ע������: �������false���������Ӧ��ɾ����Ϣ����free(message)��
      *            ������Ϣ����Dispatcher��ɾ����
      *            ������Ϣ�ڴ������malloc��calloc��realloc�����ġ�
      *            
      */
    virtual bool send_message(uint16_t route_id, dispatch_message_t* message, uint32_t milliseconds=0) = 0; 
    
    /***
      * ������Ϣ
      * @ip: ��Ϣ�����͵�IP��ַ
      * @message: ��Ҫ���͵���Ϣ
      * @milliseconds: �ȴ����ͳ�ʱ�����������Ϊ0��ʾ���ȴ��������أ�����
      *                �ȴ���Ϣ�ɴ�����У�ֱ����ʱ����
      * @return: �����Ϣ������У��򷵻�true�����򷵻�false
      * @ע������: �������false���������Ӧ��ɾ����Ϣ����free(message)��
      *            ������Ϣ����Dispatcher��ɾ����
      *            ������Ϣ�ڴ������malloc��calloc��realloc�����ġ�
      */
    virtual bool send_message(const net::ipv4_node_t& ip_node, dispatch_message_t* message, uint32_t milliseconds=0) = 0; 
    virtual bool send_message(const net::ipv6_node_t& ip_node, dispatch_message_t* message, uint32_t milliseconds=0) = 0; 
};

//////////////////////////////////////////////////////////////////////////
// ȫ��C��������

extern "C" void destroy_dispatcher();      /** ������Ϣ�ַ������ */
extern "C" IDispatcher* get_dispatcher();  /** �����Ϣ�ַ������ */
extern "C" IDispatcher* create_dispatcher(sys::ILogger* logger); /** ������Ϣ�ַ������ */

MOOON_NAMESPACE_END
#endif // DISPATCHER_H

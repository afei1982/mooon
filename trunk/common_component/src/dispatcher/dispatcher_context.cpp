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
#include "sys/sys_util.h"
#include "dispatcher_context.h"
MY_NAMESPACE_BEGIN

CDispatcherContext::CDispatcherContext()
    :_reply_handler_factory(NULL)
    ,_sender_table_managed(NULL)
    ,_sender_table_unmanaged(NULL)
{            
}

CDispatcherContext::~CDispatcherContext()
{
    delete _sender_table_managed;
    delete _sender_table_unmanaged;
}

bool CDispatcherContext::create(uint32_t queue_size, uint16_t thread_count)
{   
    // !��ע���������Ⱥ�ʱ���ϵ
    // !����SenderTable�����ڴ���ThreadPool֮��
    if (!create_thread_pool(thread_count)) return false;
    if (!create_sender_table_managed(queue_size)) return false;
    if (!create_sender_table_unmanaged(queue_size)) return false;
    
    // �����̳߳أ������г��߳̿�ʼ����
    thread_pool.activate();
    
    return true;
}

void CDispatcherContext::destroy()
{
    thread_pool.destroy();
}

void CDispatcherContext::release_sender(ISender* sender)
{
    _sender_table_unmanaged->release_sender(sender);
}

ISender* CDispatcherContext::get_sender(const net::ipv4_node_t& ip_node)
{
    return _sender_table_unmanaged->get_sender(ip_node);
}

ISender* CDispatcherContext::get_sender(const net::ipv6_node_t& ip_node)
{
    return _sender_table_unmanaged->get_sender(ip_node);
}

void CDispatcherContext::set_reply_handler_factory(IReplyHandlerFactory* reply_handler_factory)
{
    _reply_handler_factory = reply_handler_factory;
}

bool CDispatcherContext::send_message(uint16_t node_id, dispach_message_t* message)
{
    // �������ø��£��������_sender_table�����ؽ���
    sys::CReadLockHelper read_lock_helper(_sender_table_managed_read_write_lock);
    return _sender_table_managed->send_message(node_id, message);
}

bool CDispatcherContext::send_message(const net::ipv4_node_t& ip_node, dispach_message_t* message)
{    
    return _sender_table_unmanaged->send_message(ip_node, message);
}

bool CDispatcherContext::send_message(const net::ipv6_node_t& ip_node, dispach_message_t* message)
{   
    return _sender_table_unmanaged->send_message(ip_node, message);
}

bool CDispatcherContext::create_thread_pool(uint16_t thread_count)
{
    do
    {            
        try
        {
            // ���û�������߳�������ȡĬ�ϵ��̸߳���
            if (0 == thread_count)
                thread_count = get_default_thread_count();
            
            // �����̳߳�
            // ֻ��CThread::before_start����false��create�Ż᷵��false
            if (!thread_pool.create(thread_count)) break;                        
            return true;
        }
        catch (sys::CSyscallException& ex)
        {
            MYLOG_ERROR("Failed to create thread pool, error is %s at %s:%d.\n"
                , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str()
                , ex.get_filename(), ex.get_linenumber());
            break; 
        }
    } while (false);

    return false;
}

bool CDispatcherContext::create_sender_table_managed(uint32_t queue_size)
{
    _sender_table_managed = new CSenderTableManaged(queue_size, &thread_pool);
    if (!_sender_table_managed->load()) 
    {
        delete _sender_table_managed;
        _sender_table_managed = NULL;
    }
    
    return _sender_table_managed != NULL;    
}

bool CDispatcherContext::create_sender_table_unmanaged(uint32_t queue_size)
{
    _sender_table_unmanaged = new CSenderTableUnmanaged(queue_size, &thread_pool);
    return true;
}

uint16_t CDispatcherContext::get_default_thread_count() const
{
    // ����Ĭ�ϵ��̳߳����̸߳���ΪCPU�˸�����1�������ȡ����CPU�˸�������ȡ1
    uint16_t thread_count = sys::CSysUtil::get_cpu_number();
    return (thread_count < 2)? 1: thread_count-1;
}

//////////////////////////////////////////////////////////////////////////
static CDispatcherContext* g_dispatcher = NULL;

void destroy_dispatcher()
{
    delete g_dispatcher;
    g_dispatcher = NULL;
}

IDispatcher* get_dispatcher()
{
    if (NULL == g_dispatcher) g_dispatcher = new CDispatcherContext;
    return g_dispatcher;
}

MY_NAMESPACE_END

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
#include "sender.h"
#include "send_thread.h"
#include "sender_table.h"
MY_NAMESPACE_BEGIN

CSender::~CSender()
{
    clear_message();

    // ɾ��_reply_handler�������ڴ�й©
    IReplyHandlerFactory* reply_handler_factory = _thread_pool->get_reply_handler_factory();
    if (reply_handler_factory != NULL)
        reply_handler_factory->destroy_reply_handler(_reply_handler);
}

CSender::CSender(CSendThreadPool* thread_pool, int32_t node_id, uint32_t queue_max, IReplyHandler* reply_handler)
    :_thread_pool(thread_pool)
    ,_object(NULL)
    ,_node_id(node_id)
    ,_send_queue(queue_max, this)
    ,_reply_handler(reply_handler)
    ,_current_offset(0)
    ,_current_message(NULL)
{       
}

bool CSender::push_message(dispach_message_t* message)
{
    return _send_queue.push_back(message);
}

void CSender::before_close()
{
    _reply_handler->sender_closed(_object, _node_id, get_peer_ip(), get_peer_port());
}

void CSender::clear_message()
{
    // ɾ���ж��е�������Ϣ
    dispach_message_t* message;
    while (_send_queue.pop_front(message))
    {              
        delete message;
    }
}

bool CSender::do_handle_reply()
{
    size_t buffer_length = _reply_handler->get_buffer_length();
    char* buffer = _reply_handler->get_buffer();

    // �ر�����
    if ((0 == buffer_length) || (NULL == buffer)) return false;
    ssize_t data_size = this->receive(buffer, buffer_length);
    if (0 == data_size) return false; // ���ӱ��ر�

    // ����Ӧ���������ʧ����ر�����
    return _reply_handler->handle_reply(_object, _node_id, get_peer_ip(), get_peer_port(), (uint32_t)data_size);
}

dispach_message_t* CSender::get_current_message()
{
    // ��ǰ��Ϣ��δ�������
    if (_current_message != NULL) return _current_message;

    // �Ӷ�����ȡһ����Ϣ
    return (_send_queue.pop_front(_current_message))? _current_message: NULL;
}

void CSender::reset_current_message(bool delete_message)
{
    // ���õ�ǰ��Ϣ״̬ 
    _current_offset = 0;
    if (delete_message)
    {    
        delete [](char*)_current_message;
        _current_message = NULL;        
    }
}

net::epoll_event_t CSender::do_send_message(void* ptr, uint32_t events)
{
    CSendThread* thread = static_cast<CSendThread*>(ptr);
    net::CEpoller& epoller = thread->get_epoller();

    try
    {       
        // ���ȴ����걾�����е�������Ϣ
        for (;;)
        {
            dispach_message_t* message = get_current_message();
            if (NULL == message)
            {
                if (1 == this->get_refcount())
                {
                    // ������Ҫ������
                    return net::epoll_destroy;
                }
                else
                {
                    // �������Ѿ�û����Ϣ�ˣ���Ҫ��ض����Ƿ������ݣ�������Ҫ�����м���Epoll��            
                    epoller.set_events(&_send_queue, EPOLLIN);
                    return net::epoll_read;
                }
            }
            else
            {
                ssize_t retval = this->send(message->content+_current_offset, message->length-_current_offset);
                _current_offset += retval;

                if (_current_offset == message->length)
                {
                    // ��ǰ��Ϣ�Ѿ�ȫ������
                    reset_current_message(true);
                    //return net::epoll_read; // ����������һ����Ϣ
                }
                else
                {
                    // һ��δ�����꣬��Ҫ����
                    return net::epoll_write;
                }
            }
        }
    }
    catch (sys::CSyscallException& ex)
    {
        // �����쳣
        reset_current_message(false);
        DISPATCHER_LOG_DEBUG("Dispatcher send error for %s.\n", sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
        thread->add_sender(this); // ����������
        return net::epoll_close;   
    }    
}

int32_t CSender::get_node_id() const
{
    return _node_id;
}

void CSender::do_set_object(void* object)
{
    _object = object;    
}

net::epoll_event_t CSender::do_handle_epoll_event(void* ptr, uint32_t events)
{
    CSendThread* thread = static_cast<CSendThread*>(ptr);
    
    do
    {
        if ((EPOLLHUP & events) || (EPOLLERR & events))
        {
            reset_current_message(false);
            thread->add_sender(this); // ����������
            break;
        }
        else if (EPOLLOUT & events)
        {
            // ������������ӣ����л�״̬
            if (is_connect_establishing()) set_connected_state();
            return do_send_message(ptr, events);
        }
        else if (EPOLLIN & events)
        {
            if (!do_handle_reply()) break;            
            return net::epoll_none;
        }    
        else // Unknown events
        {
            break;
        }
    } while (false);

    reset_current_message(false);
    thread->add_sender(this); // ����������
    return net::epoll_close;
}

MY_NAMESPACE_END

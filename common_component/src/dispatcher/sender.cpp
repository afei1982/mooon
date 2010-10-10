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
    ,_node_id(node_id)
    ,_send_queue(queue_max, this)
    ,_reply_handler(reply_handler)
    ,_total_size(0)
    ,_current_count(0)
    ,_current_offset(0)
    ,_current_message_iovec(NULL)
{       
}

bool CSender::push_message(dispach_message_t* message)
{
    return _send_queue.push_back(message);
}

void CSender::before_close()
{
    _reply_handler->sender_closed(_node_id, get_peer_ip(), get_peer_port());
}

void CSender::clear_message()
{
    // ɾ���ж��е�������Ϣ
    dispach_message_t* message;
    while (_send_queue.pop_front(message))
    {              
        free(message);
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
    return _reply_handler->handle_reply(_node_id, get_peer_ip(), get_peer_port(), (uint32_t)data_size);
}

struct iovec* CSender::get_current_message_iovec()
{
    // ��ǰ��Ϣ��δ�������
    if (_current_count > 0)
    {
        return _current_message_iovec;
    }

    _current_count = _thread_pool->get_message_merged_number();
    dispach_message_t* message_array[_thread_pool->get_message_merged_number()];

    _send_queue.pop_front(message_array, _current_count);
    if (0 == _current_count) return NULL; // ����Ϊ��
    
    _current_message_iovec = &_message_iovec[0];
    for (uint32_t i=0; i<_current_count; ++i)
    {        
        // ���÷��Ͳ���
        _current_message_iovec[i].iov_len = message_array[i]->length;
        _current_message_iovec[i].iov_base = message_array[i]->content;

        // ���÷��ͳ�ʼ״ֵ̬
        _total_size += message_array[i]->length;      
    }
    
    // �Ӷ�����ȡһ����Ϣ
    return _current_message_iovec;
}

// ���õ�ǰ��Ϣ״̬ 
void CSender::reset_current_message_iovec(reset_action_t reset_action)
{      
    uint32_t i;
    uint32_t current_count = _current_count;

    if (ra_finish == reset_action)
    {    
        // ��ǰ��Ϣ��������                
        _total_size = 0;
        _current_count = 0;
        _current_offset = 0;
        _current_message_iovec = NULL;

        // ע�������ѭ����Ҫʹ��_current_count����Ϊ���Ѿ�������Ϊ0
        for (i=0; i<current_count; ++i)  
        {
            free((char*)_current_message_iovec[i].iov_base - sizeof(uint32_t));
        }
    }
    else
    {
        uint32_t current_size = 0;        
        
        // ������Щ��Ϣ�Ѿ��������ͳ�ȥ��
        for (i=0; i<current_count; ++i)
        {
            current_size += _current_message_iovec[i].iov_len;
            if (_current_offset >= current_size)
            {
                // ����Ϣ�Ѿ����ͳ�ȥ
                --_current_count;
                _total_size -= _current_message_iovec[i].iov_len;
                free((char*)_current_message_iovec[i].iov_base - sizeof(uint32_t));            
            }
            else
            {
                // ���˵��ϵ㣬��Ҫ������������ͣ��Ա�֤δ���������Ϣ�����ٴδ�ͷ��
                _current_message_iovec = &_current_message_iovec[i];
                _current_offset = (ra_error == reset_action)? 0: (_current_offset-current_size);
                break;
            }
        }
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
            struct iovec* current_message_iovec = get_current_message_iovec();
            if (NULL == current_message_iovec)
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
                ssize_t retval = this->writev(current_message_iovec, _current_count);
                _current_offset += retval;

                if (_current_offset == _total_size)
                {
                    // ��ǰ��Ϣ�Ѿ�ȫ������
                    reset_current_message_iovec(ra_finish);
                    //return net::epoll_read; // ����������һ����Ϣ
                }
                else
                {
                    // һ��δ�����꣬��Ҫ����
                    reset_current_message_iovec(ra_continue);
                    return net::epoll_write;
                }
            }
        }
    }
    catch (sys::CSyscallException& ex)
    {
        // �����쳣        
        DISPATCHER_LOG_DEBUG("Dispatcher send error for %s.\n", sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
        reset_current_message_iovec(ra_error);
        thread->add_sender(this); // ����������
        return net::epoll_close;   
    }    
}

int32_t CSender::get_node_id() const
{
    return _node_id;
}

net::epoll_event_t CSender::do_handle_epoll_event(void* ptr, uint32_t events)
{
    CSendThread* thread = static_cast<CSendThread*>(ptr);
    
    do
    {
        if ((EPOLLHUP & events) || (EPOLLERR & events))
        {
            reset_current_message_iovec(ra_error);
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

    reset_current_message_iovec(ra_error);
    thread->add_sender(this); // ����������
    return net::epoll_close;
}

MY_NAMESPACE_END

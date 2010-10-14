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
MOOON_NAMESPACE_BEGIN

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
    ,_is_in_reply(false)
    ,_total_size(0)
    ,_current_count(0)
    ,_current_offset(0)
    ,_current_message_iovec(NULL)
{       
}

int32_t CSender::get_node_id() const
{
    return _node_id;
}

bool CSender::push_message(dispatch_message_t* message)
{
    return _send_queue.push_back(message);
}

void CSender::before_close()
{
    // ������ڴ���Ӧ����Ϣ�����У������close���Ա�����߶�δ������Ӧ����д���
    if (_is_in_reply)
    {
        _reply_handler->sender_closed(_node_id, get_peer_ip(), get_peer_port());
        _is_in_reply = false;
    }
}

void CSender::clear_message()
{
    // ɾ���ж��е�������Ϣ
    dispatch_message_t* message;
    while (_send_queue.pop_front(message))
    {              
        free(message);
    }
}

util::handle_result_t CSender::do_handle_reply()
{
    size_t buffer_length = _reply_handler->get_buffer_length();
    char* buffer = _reply_handler->get_buffer();

    // �ر�����
    if ((0 == buffer_length) || (NULL == buffer)) 
    {
        DISPATCHER_LOG_DEBUG("Sender %d:%s:%d can not get buffer or length.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port());
        return util::handle_error;
    }
    
    ssize_t data_size = this->receive(buffer, buffer_length);
    if (0 == data_size) 
    {
        DISPATCHER_LOG_WARN("Sender %d:%s:%d closed by peer.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port());
        return util::handle_error; // ���ӱ��ر�
    }

    // ����Ӧ���������ʧ����ر�����
    util::handle_result_t retval = _reply_handler->handle_reply(_node_id, get_peer_ip(), get_peer_port(), (uint32_t)data_size);
    if (util::handle_finish == retval)
    {
        DISPATCHER_LOG_DEBUG("Sender %d:%s:%d reply finished.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port());
    }
    else if (util::handle_error == retval)
    {
        DISPATCHER_LOG_ERROR("Sender %d:%s:%d reply error.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port());
    }

    return retval;
}

struct iovec* CSender::get_current_message_iovec()
{
    // ��ǰ��Ϣ��δ�������
    if (_current_count > 0)
    {
        return _current_message_iovec;
    }

    _current_count = _thread_pool->get_message_merged_number();
    dispatch_message_t* message_array[_thread_pool->get_message_merged_number()];

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
    char* current_message;
    // �����ȱ���_current_count����Ϊ�������������޸�
    uint32_t current_count = _current_count;

    if (ra_finish == reset_action)
    {            
        // �ͷ���Ϣ�ڴ�
        for (i=0; i<current_count; ++i)  
        {
            current_message = (char*)get_struct_head_address(dispatch_message_t, content, _current_message_iovec[i].iov_base);
            free(current_message);
        }

        // ��ǰ��Ϣ��������                
        _total_size = 0;
        _current_count = 0;
        _current_offset = 0;
        _current_message_iovec = NULL;
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
                current_message = (char*)get_struct_head_address(dispatch_message_t, content, _current_message_iovec[i].iov_base);
                free(current_message);            
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
                    DISPATCHER_LOG_INFO("Sender %d:%s:%d end of life.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port());
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
        DISPATCHER_LOG_ERROR("Sender %d:%s:%d send error for %s.\n"
            , _node_id, get_peer_ip().to_string().c_str(), get_peer_port()
            , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
        return net::epoll_close;   
    }    
}

net::epoll_event_t CSender::do_handle_epoll_event(void* ptr, uint32_t events)
{
    CSendThread* thread = static_cast<CSendThread*>(ptr);
    
    do
    {
        if ((EPOLLHUP & events) || (EPOLLERR & events))
        {
            DISPATCHER_LOG_ERROR("Sender %d:%s:%d happen HUP or ERROR event.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port());
            break;
        }
        else if (EPOLLOUT & events)
        {
            // ������������ӣ����л�״̬
            if (is_connect_establishing()) set_connected_state();
            net::epoll_event_t send_retval = do_send_message(ptr, events);
            if (net::epoll_close == send_retval) break;

            return send_retval;
        }
        else if (EPOLLIN & events)
        {          
            _is_in_reply = true;
            util::handle_result_t reply_retval = do_handle_reply();
            if (util::handle_finish == reply_retval) _is_in_reply = false;
            if (util::handle_error == reply_retval) 
            {
                break;
            }

            return net::epoll_none;
        }    
        else // Unknown events
        {
            DISPATCHER_LOG_ERROR("Sender %d:%s:%d got unknown events %d.\n", _node_id, get_peer_ip().to_string().c_str(), get_peer_port(), events);
            break;
        }
    } while (false);

    reset_current_message_iovec(ra_error);
    thread->add_sender(this); // ����������
    return net::epoll_close;
}

MOOON_NAMESPACE_END

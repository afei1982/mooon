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
#include <sys/thread.h>
#include <net/net_util.h>
#include "connection.h"
#include "server_thread.h"
MOOON_NAMESPACE_BEGIN

CConnection::CConnection()
    :_is_in_pool(false) // ֻ�ܳ�ʼ��Ϊfalse
    ,_protocol_parser(NULL)
    ,_request_responsor(NULL)
{
}

void CConnection::reset()
{
    _protocol_parser->reset();
	_request_responsor->reset();	
}

net::epoll_event_t CConnection::handle_epoll_event(void* ptr, uint32_t events)
{
    net::epoll_event_t retval;
    CServerThread* thread = (CServerThread *)ptr;
    thread->update_waiter(this); // ����ʱ�������ֹ��ʱ
                    
    if (EPOLLIN & events)
    {
        retval = do_handle_epoll_read(ptr);
    }
    else if (EPOLLOUT & events)
    {
        retval = do_handle_epoll_send(ptr);
    }
    else
    {
        retval = do_handle_epoll_error();
    }

    return retval;
}

net::epoll_event_t CConnection::do_handle_epoll_error()
{
    SERVER_LOG_DEBUG("Connection %s:%d exception.\n", get_peer_ip().to_string().c_str(), get_peer_port());
    return net::epoll_close;
}

net::epoll_event_t CConnection::do_handle_epoll_send(void* ptr)
{
    ssize_t retval;
    uint32_t size = _request_responsor->get_size();  
    uint32_t offset = _request_responsor->get_offset();

    try
    {
        if (0 == size)
	    {
            // ����Ӧ������Ҫ����
            return _request_responsor->keep_alive()? net::epoll_read: net::epoll_close;
	    }
        
        if (_request_responsor->is_send_file())
        {
            // �����ļ�
            off_t file_offset = (off_t)offset;
            int file_fd = _request_responsor->get_fd();            
            retval = CTcpWaiter::send_file(file_fd, &file_offset, size);
        }
        else
        {            
            // ����Buffer
            const char* buffer = _request_responsor->get_buffer();
            retval = CTcpWaiter::send(buffer+offset, size); 
            
        }       
    }
    catch (sys::CSyscallException& ex)
    {
        SERVER_LOG_ERROR("Waiter %s:%d send error: %s at %s:%d.\n"
                 , get_peer_ip().to_string().c_str(), get_peer_port()
                 , strerror(ex.get_errcode()), ex.get_filename(), ex.get_linenumber());	
        return net::epoll_close;
    }

    if (-1 == retval)
    {
        // Would block
		SERVER_LOG_DEBUG("Send block to %s:%d.\n", get_peer_ip().to_string().c_str(), get_peer_port());
        return net::epoll_read_write;
    }

    // �����Ѿ����͵Ĵ�Сֵ
    _request_responsor->move_offset((uint32_t)retval);	
    if (_request_responsor->get_size() > _request_responsor->get_offset()) return net::epoll_read_write;        

    // ������ϣ����Ϊ�����ӣ���ֱ�ӹر�
    if (!_request_responsor->keep_alive())
    {
        // ������
        SERVER_LOG_DEBUG("Response finish with keep alive false to %s:%d.\n", get_peer_ip().to_string().c_str(), get_peer_port());
        return net::epoll_close;
    }               

	reset();
    SERVER_LOG_DEBUG("Response finish with keep alive true to %s:%d.\n", get_peer_ip().to_string().c_str(), get_peer_port());
    return net::epoll_read;
}

net::epoll_event_t CConnection::do_handle_epoll_read(void* ptr)
{
    ssize_t retval;
    CServerThread* thread = (CServerThread *)ptr;
    uint32_t buffer_offset = _protocol_parser->get_buffer_offset();
    uint32_t buffer_size = _protocol_parser->get_buffer_size();
    char* buffer = _protocol_parser->get_buffer();

    try
    {
        retval = receive(buffer+buffer_offset, buffer_size-buffer_offset);
    }
    catch (sys::CSyscallException& ex)
    {
        SERVER_LOG_ERROR("Waiter %s:%d receive error: %s at %s:%d.\n"
                 , get_peer_ip().to_string().c_str(), get_peer_port()
                 , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str()
                 , ex.get_filename(), ex.get_linenumber());		
        return net::epoll_close;
    }

    if (0 == retval)
    {
        SERVER_LOG_DEBUG("Waiter %s:%d closed by peer.\n", get_peer_ip().to_string().c_str(), get_peer_port());
        return net::epoll_close;
    }
    if (-1 == retval)
    {
        return net::epoll_none;
    }
    
    buffer[buffer_offset+retval] = '\0';
    SERVER_LOG_DEBUG("[%s:%d] %u:%.*s.\n", get_peer_ip().to_string().c_str(), get_peer_port(), (uint32_t)retval, retval, buffer+buffer_offset);
    
    util::handle_result_t handle_result = _protocol_parser->parse((uint32_t)retval);
    if (util::handle_finish == handle_result)
    {
        if (!thread->get_packet_handler()->handle(_protocol_parser, _request_responsor))
        {
            SERVER_LOG_DEBUG("Protocol translate error to %s:%d.\n", get_peer_ip().to_string().c_str(), get_peer_port());				
            return net::epoll_close;
        }

        return do_handle_epoll_send(ptr);
    }
    else if (util::handle_error == handle_result)
    {
        SERVER_LOG_DEBUG("Protocol parse error from %s:%d.\n", get_peer_ip().to_string().c_str(), get_peer_port());
        return net::epoll_close;
    }
    else
    {        
		SERVER_LOG_DEBUG("Continue to receive from %s:%d ...\n", get_peer_ip().to_string().c_str(), get_peer_port());
        return net::epoll_none;
    }
}

MOOON_NAMESPACE_END

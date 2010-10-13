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
#include <net/net_util.h>
#include "send_thread.h"
#include "unmanaged_sender_table.h"
MOOON_NAMESPACE_BEGIN

CSendThread::CSendThread()
    :_current_time(0)
    ,_reconnect_times(0)
    ,_last_connect_time(0)
    ,_unmanaged_sender_table(NULL)
{
}

time_t CSendThread::get_current_time() const
{
    return _current_time;
}

void CSendThread::add_sender(CSender* sender)
{
    sys::CLockHelper<sys::CLock> lock_helper(_unconnected_lock);
    _unconnected_queue.push_back(sender);
}

void CSendThread::set_reconnect_times(uint32_t reconnect_times)
{
    _reconnect_times = reconnect_times;
}

void CSendThread::set_unmanaged_sender_table(CUnmanagedSenderTable* unmanaged_sender_table)
{
    _unmanaged_sender_table = unmanaged_sender_table;
}

void CSendThread::run()
{
    do_connect();
    _current_time = time(NULL);
    _timeout_manager.check_timeout(_current_time);

    int events_count = _epoller.timed_wait(1000);
    if (0 == events_count)
    {
        // ��ʱ����        
    }
    else
    {
        for (int i=0; i<events_count; ++i)
        {
            net::CEpollable* epollable = _epoller.get(i);
            uint32_t events = _epoller.get_events(i);            

            net::epoll_event_t retval = epollable->handle_epoll_event(this, events);
            if (net::epoll_none == retval)
            {
                // �������κδ���
            }
            else if (net::epoll_read == retval)
            {
                _epoller.set_events(epollable, EPOLLIN);
            }
            else if (net::epoll_write == retval)
            {
                _epoller.set_events(epollable, EPOLLOUT);
            }
            else if (net::epoll_read_write == retval)
            {
                _epoller.set_events(epollable, EPOLLIN|EPOLLOUT);
            }
            else if (net::epoll_remove == retval)
            {
                _epoller.del_events(epollable);
            }
            else if (net::epoll_close == retval)
            {                
                epollable->close();
                _epoller.del_events(epollable);                
            }
            else if (net::epoll_destroy == retval)
            {                
                epollable->close();
                _epoller.del_events(epollable);
                epollable->dec_refcount(); // ��Ҫ���ٶ���
            }
        }
    }
}

bool CSendThread::before_start()
{
    _timeout_manager.set_keep_alive_second(60);
    _timeout_manager.set_timeout_handler(this);
    _epoller.create(1000);
    return true;
}

void CSendThread::on_timeout_event(CUnmanagedSender* timeoutable)
{
    remove_sender(timeoutable);
}

void CSendThread::do_connect()
{
    // ����if���Խ���do_connect�����ܵ�Ӱ��
    if (_unconnected_queue.empty()) return;
    
    // ��Ҫ���ı���
    sys::CLockHelper<sys::CLock> lock_helper(_unconnected_lock);

    // �����ȵõ�count��ֻ��Ϊδ���ӳɹ��ģ��������������_unconnected_queueβ����
    // ���ҳɹ��Ļ��_unconnected_queue���޳���������ֻ֤��Ե�ǰ�ı���һ�Σ��������ظ���Ҳ�����ٱ���һ��
    CSenderQueue::size_type count =  _unconnected_queue.size();
    for (CSenderQueue::size_type i=0; i<count; ++i)
    {
        CSender* sender = _unconnected_queue.front();
        _unconnected_queue.pop_front();

        // ��Ҫ������
        if (1 == sender->get_refcount())
        {
            sender->dec_refcount();
            continue;
        }
        else if (sender->get_reconnect_times() > _reconnect_times)
        {
            // �����������������Ӵ���
            remove_sender(sender);
            continue;
        }
        
        try
        {
            // ��������첽���ӣ���������ܵı�֤
            sender->async_connect();
            _epoller.set_events(sender, EPOLLOUT);
        }
        catch (sys::CSyscallException& ex)
        {
            // ����δ�ɹ����ٲ��뵽����β����������ѭ��count�����ƣ����Է���β�����Ա�֤���ֲ����ٱ�����
            sender->close();
            _unconnected_queue.push_back(sender);
            DISPATCHER_LOG_DEBUG("Sender connected to %s:%d failed.\n"
                , sender->get_peer_ip().to_string().c_str(), sender->get_peer_port());
        }
    }
}

void CSendThread::remove_sender(CSender* sender)
{
    sender->close();
    _epoller.del_events(sender);

    uint16_t port = sender->get_peer_port();
    const uint32_t* ip_data = sender->get_peer_ip().get_address_data();    

    if (sender->is_ipv6())
    {
        net::ipv6_node_t ipv6_node(port, ip_data);
        _unmanaged_sender_table->close_sender(ipv6_node);
    }       
    else
    {
        net::ipv4_node_t ipv4_node(port, ip_data[0]);
        _unmanaged_sender_table->close_sender(ipv4_node);
    }
}

MOOON_NAMESPACE_END

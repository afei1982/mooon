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
#include "send_thread.h"
#include "net/net_util.h"
MY_NAMESPACE_BEGIN

CSendThread::CSendThread()
    :_last_connect_time(0)
    ,_reconnect_frequency(3)
    ,_reply_handler(NULL)
{
}

void CSendThread::add_sender(CSender* sender)
{
    sys::CLockHelper<sys::CLock> lock_helper(_unconnected_lock);
    _unconnected_queue.push_back(sender);
}

void CSendThread::run()
{
    do_connect();
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
    _epoller.create(1000);
    return true;
}

void CSendThread::do_connect()
{
    // ����if���Խ���do_connect�����ܵ�Ӱ��
    if (_unconnected_queue.empty()) return;
    time_t now = time(NULL);
    if (now - _last_connect_time < _reconnect_frequency) return; // ����������Ƶ��
    _last_connect_time = now;

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
            MYLOG_DEBUG("Sender connected to %s:%d failed.\n"
                , sender->get_peer_ip().to_string().c_str(), sender->get_peer_port());
        }
    }
}

MY_NAMESPACE_END

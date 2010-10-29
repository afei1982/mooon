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
 * Author: jian yi, eyjian@qq.com
 */
#include <sys/thread.h>
#include <net/net_util.h>
#include "server_thread.h"
#include "server_listener.h"
MOOON_NAMESPACE_BEGIN

void CServerListener::handle_epoll_event(void* ptr, uint32_t events)
{   
    int newfd;
    uint16_t port;
    ip_address_t ip_address;
    
    try
    {
        
        newfd = net::CListener::accept(ip_address, port);
        
        CServerThread* waiter_thread = (CServerThread *)ptr;
        if (waiter_thread->add_waiter(newfd, ip_address, port))
        {
            // ����ĳЩserver��������Ϣ�޴���webserver
            SERVER_LOG_DEBUG("Accept a request - %s:%d.\n", ip_address.to_string().c_str(), port);
        }
    }
    catch (sys::CSyscallException& ex)
    {
		// ����ĳЩserver��������Ϣ�޴���webserver
        SERVER_LOG_DEBUG("Accept error: %s at %s:%d.\n", strerror(ex.get_errcode()), ex.get_filename(), ex.get_linenumber());
    }    
}

MOOON_NAMESPACE_END

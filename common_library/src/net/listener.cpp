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
#include <fcntl.h>
#include "net/listener.h"
#include "sys/sys_util.h"
#include "net/net_util.h"
NET_NAMESPACE_BEGIN

CListener::CListener()
    :_port(0)
{
}

void CListener::listen(const ip_address_t& ip, uint16_t port, bool enabled_address_zero)
{    
    // �Ƿ������������ַ�ϼ���
    if (!enabled_address_zero && ip.is_zero_address())
        throw sys::CSyscallException(EINVAL, __FILE__, __LINE__, "forbid listening on 0.0.0.0 address");

    // ��ֹ�����㲥��ַ
    if ( ip.is_broadcast_address())
        throw sys::CSyscallException(EINVAL, __FILE__, __LINE__, "forbid listening on broadcast address");        

    socklen_t addr_len;           // ��ַ���ȣ����ΪIPV6�����sizeof(struct sockaddr_in6)���������sizeof(struct sockaddr_in)
    struct sockaddr* addr;        // ������ַ�����ΪIPV6��ָ��addr_in6������ָ��addr_in   
    struct sockaddr_in addr_in;
    struct sockaddr_in6 addr_in6;
    
    const uint32_t* ip_data = ip.get_address_data();
    if (ip.is_ipv6())
    {
        addr_len = sizeof(struct sockaddr_in6);
        addr = (struct sockaddr*)&addr_in6;
        addr_in6.sin6_family = AF_INET6;
        addr_in6.sin6_port = htons(port);
        memcpy(&addr_in6.sin6_addr, ip_data, addr_len);
    }
    else
    {
        addr_len = sizeof(struct sockaddr_in);
        addr = (struct sockaddr*)&addr_in;
        addr_in.sin_family = AF_INET;        
        addr_in.sin_port = htons(port);
        addr_in.sin_addr.s_addr = ip_data[0];
        memset(addr_in.sin_zero, 0, sizeof(addr_in.sin_zero));
    }

    // ����һ��socket
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd) throw sys::CSyscallException(errno, __FILE__, __LINE__, "socket error");

    try
    {
        int retval; // �������淵��ֵ
        
        // IP��ַ����
        int reuse = 1;
        retval = ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
        if (-1 == retval) throw sys::CSyscallException(errno, __FILE__, __LINE__, "setsockopt error");

        // ��ֹ�ӽ��̼̳�
        retval = ::fcntl(fd, F_SETFD, FD_CLOEXEC);
        if (-1 == retval) throw sys::CSyscallException(errno, __FILE__, __LINE__, "fcntl error");

        int retry_times = 50; // ���Դ���
        for (;;)
        {
            retval = ::bind(fd, addr, addr_len);
            if (retval != -1) break;
            
            if ((EADDRINUSE == errno) && (--retry_times > 0))
                sys::CSysUtil::millisleep(100);
            else
                throw sys::CSyscallException(errno, __FILE__, __LINE__, "bind error");
        }

        // ���û��bind��������ѡһ��IP�Ͷ˿ڣ�����listen֮ǰ������bind
        retval = ::listen(fd, 10000);
        if (-1 == retval) throw sys::CSyscallException(errno, __FILE__, __LINE__, "listen error");
        
        // �洢ip��port���Ǳ���ģ������Է���gdbʱ�鿴�����ֵ
        _ip = ip;
        _port = port;
        CEpollable::set_fd(fd);
    }
    catch (...)
    {
        if (fd != -1) ::close(fd);
        throw;
    }
}

void CListener::listen(const ipv4_node_t& ip_node, bool enabled_address_zero)
{
    ip_address_t ip = ip_node.ip;
    listen(ip, ip_node.port, enabled_address_zero);
}

void CListener::listen(const ipv6_node_t& ip_node, bool enabled_address_zero)
{
    ip_address_t ip = (uint32_t*)ip_node.ip;
    listen(ip, ip_node.port, enabled_address_zero);
}

int CListener::accept(ip_address_t& peer_ip, uint16_t& peer_port)
{
    struct sockaddr_in6 peer_addr_in6;
    struct sockaddr* peer_addr = (struct sockaddr*)&peer_addr_in6;        
    socklen_t peer_addrlen = sizeof(struct sockaddr_in6); // ʹ������

    int newfd = ::accept(CEpollable::get_fd(), peer_addr, &peer_addrlen);
    if (-1 == newfd) throw sys::CSyscallException(errno, __FILE__, __LINE__, "accept error");

    // ���ܵ���һ��IPV4����
    if (AF_INET == peer_addr->sa_family)
    {
        struct sockaddr_in* peer_addr_in = (struct sockaddr_in*)peer_addr;
        peer_port = peer_addr_in->sin_port;
        peer_ip = peer_addr_in->sin_addr.s_addr;
    }
    else
    {
        // ���ܵ���һ��IPV6����
        peer_port = peer_addr_in6.sin6_port;
        peer_ip = (uint32_t*)&peer_addr_in6.sin6_addr;
    }

    return newfd;
}

NET_NAMESPACE_END

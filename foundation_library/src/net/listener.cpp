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
    _ip[0] = '\0';
}

void CListener::listen(const char* ip, uint16_t port)
{
    // ��ֹ�㲥��ַ
    if (CNetUtil::is_broadcast_address(ip))
        throw sys::CSyscallException(EINVAL, __FILE__, __LINE__, "Invalid IP address");

    // ת��Ϊ�������͵�IP��ַ��������Ƿ���Ч
    int ip_tmp = CNetUtil::convert_ipv4(ip);
    if (0 == ip_tmp)
    {        
        throw sys::CSyscallException(EINVAL, __FILE__, __LINE__, "Invalid IP address");
    }

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd) throw sys::CSyscallException(errno, __FILE__, __LINE__);

    try
    {
        int retval; // �������淵��ֵ
        
        // IP��ַ����
        int reuse = 1;
        retval = ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
        if (-1 == retval) throw sys::CSyscallException(errno, __FILE__, __LINE__);

        // ��ֹ�ӽ��̼̳�
        retval = ::fcntl(fd, F_SETFD, FD_CLOEXEC);
        if (-1 == retval) throw sys::CSyscallException(errno, __FILE__, __LINE__);

        // ���ü�����ַ�Ͷ˿�
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);          
        addr.sin_addr.s_addr = (NULL == ip) ? INADDR_ANY: ip_tmp;

        int retry_times = 50; // ���Դ���
        for (;;)
        {
            retval = ::bind(fd, (const struct sockaddr *)&addr, sizeof(struct sockaddr_in));
            if (retval != -1) break;
            
            if ((EADDRINUSE == errno) && (--retry_times > 0))
                sys::CSysUtil::millisleep(100);
            else
                throw sys::CSyscallException(errno, __FILE__, __LINE__);
        }

        // ���û��bind��������ѡһ��IP�Ͷ˿ڣ�����listen֮ǰ������bind
        retval = ::listen(fd, 10000);
        if (-1 == retval) throw sys::CSyscallException(errno, __FILE__, __LINE__);
        
        // �洢ip��port���Ǳ���ģ������Է���gdbʱ�鿴�����ֵ
        _port = port;
        strncpy(_ip, ip, sizeof(_ip)-1);
        _ip[sizeof(_ip)-1] = '\0';
        CEpollable::set_fd(fd);
    }
    catch (...)
    {
        if (fd != -1) ::close(fd);
        throw;
    }
}

int CListener::accept(uint32_t& peer_ip, uint16_t& peer_port)
{
    struct sockaddr_in peer_addr;
    socklen_t peer_addrlen = sizeof(struct sockaddr_in);

    int newfd = ::accept(CEpollable::get_fd(), (struct sockaddr *)&peer_addr, &peer_addrlen);
    if (-1 == newfd) throw sys::CSyscallException(errno, __FILE__, __LINE__);

    peer_ip = peer_addr.sin_addr.s_addr;
    peer_port = peer_addr.sin_port;

    return newfd;
}

NET_NAMESPACE_END

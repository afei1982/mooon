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
#include <signal.h>
#include "net/net_util.h"
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

// CIgnorePipeSignal�������������Զ���PIPE�źź���
static class CIgnorePipeSignal
{
public:
    CIgnorePipeSignal()
    {
        signal(SIGPIPE, SIG_IGN);
    }
}_do_not_used_for_ever; // ��Զ��Ҫʹ��_do_not_used_for_ever

//////////////////////////////////////////////////////////////////////////
// ȫ�ֺ���

/***
  * �ж�ָ��fd�Ƿ����ָ���ı�־
  * @fd: �ļ����׽��ֵȾ��
  * @flags: ָ���ı�־ֵ
  * @return: �������ָ���ı�־ֵ���򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool has_the_flags(int fd, int flags)
{
    int curr_flags = fcntl(fd, F_GETFL, 0);
    if (-1 == curr_flags)
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

    return (curr_flags & flags == flags);
}

/***
  * �ж�ָ��fd�Ƿ�Ϊ��������
  * @fd: �ļ����׽��ֵȾ��
  * @return: ���fdΪ�������ģ��򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool is_nonblock(int fd)
{
    return has_the_flags(fd, O_NONBLOCK);
}

/***
  * �ж�ָ��fd�Ƿ�Ϊ���ӳٵ�
  * @fd: �ļ����׽��ֵȾ��
  * @return: ���fdΪ���ӳٵģ��򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool is_nodelay(int fd)
{
    return has_the_flags(fd, O_NDELAY);
}

/***
  * Ϊָ����fd���ӻ�ɾ��ָ���ı�־
  * @fd: �ļ����׽��ֵȾ��
  * @yes: �Ƿ�����Ϊָ����־�����Ϊtrue��������statusָ���ı�־������ȥ��statusָ���ı�־
  * @flags: ��־ֵ
  * @exception: ��������������׳�CSyscallException�쳣
  */
void set_socket_flags(int fd, bool yes, int flags)
{
	// Get the file status flags
	int curr_flags = fcntl(fd, F_GETFL, 0);
	if (-1 == curr_flags)
        throw sys::CSyscallException(errno, __FILE__, __LINE__);

	// keep and set the file status flags
	int new_flags = yes? (curr_flags | flags): (curr_flags & ~flags);
	if (-1 == fcntl(fd, F_SETFL, new_flags))
        throw sys::CSyscallException(errno, __FILE__, __LINE__);
}

/***
  * Ϊָ��fd�����ӻ�ɾ����������־
  * @fd: �ļ����׽��ֵȾ��
  * @yes: �Ƿ�����Ϊ��������־�����Ϊtrue��������Ϊ����������������Ϊ����
  * @exception: ��������������׳�CSyscallException�쳣
  */
void set_nonblock(int fd, bool yes)
{
    set_socket_flags(fd, yes, O_NONBLOCK);
}

/***
  * Ϊָ��fd�����ӻ�ɾ�����ӳٱ�־
  * @fd: �ļ����׽��ֵȾ��
  * @yes: �Ƿ�����Ϊ���ӳٱ�־�����Ϊtrue��������Ϊ���ӳ٣���������Ϊ�ӳ�
  * @exception: ��������������׳�CSyscallException�쳣
  */
void set_nodelay(int fd, bool yes)
{
    set_socket_flags(fd, yes, O_NDELAY);
}

void close_fd(int fd)
{
    (void)::close(fd);
}

//////////////////////////////////////////////////////////////////////////
// CEpollable

CEpollable::CEpollable()
    :_fd(-1)
    ,_epoll_events(-1)
{
}

CEpollable::~CEpollable()
{
    this->close();
}

void CEpollable::close()
{
    _epoll_events = -1;
    if (_fd != -1)
    {
        before_close();
        close_fd(_fd);
        _fd = -1;
    }        
}

/***
  * �ж�ָ��fd�Ƿ�Ϊ��������
  * @return: ���fdΪ�������ģ��򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool CEpollable::is_nonblock()
{
    return net::is_nonblock(_fd);
}

/***
  * �ж�ָ��fd�Ƿ�Ϊ���ӳٵ�
  * @return: ���fdΪ���ӳٵģ��򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool CEpollable::is_nodelay()
{
    return net::is_nodelay(_fd);
}

/***
  * �޸Ķ���ķ���������
  * @yes: �Ƿ�����Ϊ��������ʶ�����Ϊtrue��������Ϊ����������������Ϊ����
  * @exception: ��������������׳�CSyscallException�쳣
  */
void CEpollable::set_nonblock(bool yes)
{
	net::set_nonblock(_fd, yes);
}

/***
  * �޸Ķ���ķ��ӳ�����
  * @yes: �Ƿ�����Ϊ���ӳٱ�ʶ�����Ϊtrue��������Ϊ���ӳ٣���������Ϊ�ӳ�
  * @exception: ��������������׳�CSyscallException�쳣
  */
void CEpollable::set_nodelay(bool yes)
{
	net::set_nodelay(_fd, yes);
}

epoll_event_t CEpollable::handle_epoll_event(void* ptr, uint32_t events)
{
    // Do nothing
    return epoll_none;
}

void CEpollable::before_close()
{
}

NET_NAMESPACE_END

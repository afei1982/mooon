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
#ifndef EPOLLABLE_H
#define EPOLLABLE_H
#include <fcntl.h>
#include <net/if.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "net/net_config.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

/***
  * handle_epoll_event�ķ���ֵ����
  */
typedef enum
{
    epoll_none,       /** ������ʲôҲ������ */
    epoll_read,       /** ��ҪEpoll����Ϊֻ���¼� */
    epoll_write,      /** ��ҪEpoll����Ϊֻд�¼� */
    epoll_read_write, /** ��ҪEpoll����Ϊ����д�¼� */
    epoll_delete      /** ��Ҫ��Epoll���޳� */
}epoll_event_t;

/***
  * �ж�ָ��fd�Ƿ����ָ���ı�־
  * @fd: �ļ����׽��ֵȾ��
  * @flags: ָ���ı�־ֵ
  * @return: �������ָ���ı�־ֵ���򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool has_the_flags(int fd, int flags);

/***
  * �ж�ָ��fd�Ƿ�Ϊ��������
  * @fd: �ļ����׽��ֵȾ��
  * @return: ���fdΪ�������ģ��򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool is_nonblock(int fd);

/***
  * �ж�ָ��fd�Ƿ�Ϊ���ӳٵ�
  * @fd: �ļ����׽��ֵȾ��
  * @return: ���fdΪ���ӳٵģ��򷵻�true�����򷵻�false
  * @exception: ��������������׳�CSyscallException�쳣
  */
bool is_nodelay(int fd);

/***
  * Ϊָ����fd���ӻ�ɾ��ָ���ı�־
  * @fd: �ļ����׽��ֵȾ��
  * @yes: �Ƿ�����Ϊָ����־�����Ϊtrue��������statusָ���ı�־������ȥ��statusָ���ı�־
  * @flags: ��־ֵ
  * @exception: ��������������׳�CSyscallException�쳣
  */
void set_socket_flags(int fd, bool yes, int flags);

/***
  * Ϊָ��fd�����ӻ�ɾ����������־
  * @fd: �ļ����׽��ֵȾ��
  * @yes: �Ƿ�����Ϊ��������־�����Ϊtrue��������Ϊ����������������Ϊ����
  * @exception: ��������������׳�CSyscallException�쳣
  */
void set_nonblock(int fd, bool yes);

/***
  * Ϊָ��fd�����ӻ�ɾ�����ӳٱ�־
  * @fd: �ļ����׽��ֵȾ��
  * @yes: �Ƿ�����Ϊ���ӳٱ�־�����Ϊtrue��������Ϊ���ӳ٣���������Ϊ�ӳ�
  * @exception: ��������������׳�CSyscallException�쳣
  */
void set_nodelay(int fd, bool yes);

/** �ر�ָ���ľ��
  */
void close_fd(int fd);

/***
  * �õ��Ѿ����͵��ļ����ֽ���
  */
long get_send_file_bytes();

/***
  * �õ��Ѿ����͵Ĳ������ļ����ڵ����ֽ���
  */
long get_send_buffer_bytes();

/***
  * �õ��Ѿ����յ��������ֽ���
  */
long get_recv_buffer_bytes();

//////////////////////////////////////////////////////////////////////////
// CEpollable

/***
  * ��Epool�࣬���п�ʹ��Epoll��ض���Ļ���
  */
class CEpollable
{
    friend class CEpoller;

public:
    CEpollable();
    virtual ~CEpollable();

    /** �رվ�� */
    virtual void close();

    /** �õ����ֵ */
    int get_fd() const { return _fd; }
    
    /** �õ����õ�Epoll�¼� */
    int get_epoll_events() const { return _epoll_events; }

    /***
      * �ж�ָ��fd�Ƿ�Ϊ��������
      * @return: ���fdΪ�������ģ��򷵻�true�����򷵻�false
      * @exception: ��������������׳�CSyscallException�쳣
      */
    bool is_nonblock();

    /***
      * �ж�ָ��fd�Ƿ�Ϊ���ӳٵ�
      * @return: ���fdΪ���ӳٵģ��򷵻�true�����򷵻�false
      * @exception: ��������������׳�CSyscallException�쳣
      */
    bool is_nodelay();

    /***
      * �޸Ķ���ķ���������
      * @yes: �Ƿ�����Ϊ��������ʶ�����Ϊtrue��������Ϊ����������������Ϊ����
      * @exception: ��������������׳�CSyscallException�쳣
      */
	void set_nonblock(bool yes);

    /***
      * �޸Ķ���ķ��ӳ�����
      * @yes: �Ƿ�����Ϊ���ӳٱ�ʶ�����Ϊtrue��������Ϊ���ӳ٣���������Ϊ�ӳ�
      * @exception: ��������������׳�CSyscallException�쳣
      */
	void set_nodelay(bool yes);

public:
    /***
      * Epoll�¼��ص�����
      * @ptr: ����ָ��
      * @events: ������Epoll�¼�
      * @return: ��μ�epoll_event_t��˵��
      * @exception: ϵͳ���ó����׳�CSyscallException�쳣
      */
    virtual epoll_event_t handle_epoll_event(void* ptr, uint32_t events);

protected: // ���̳е�����ʹ��
    /** ���þ�� */
    void set_fd(int fd) { _fd = fd; }

private:
    void set_epoll_events(int epoll_events) { _epoll_events = epoll_events;}

private:
    int _fd;
    int _epoll_events;
};

NET_NAMESPACE_END
#endif // EPOLLABLE_H

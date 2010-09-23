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
#ifndef EPOLLER_H
#define EPOLLER_H
#include <sys/epoll.h>
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/***
  * Epoll������װ��
  */
class CEpoller
{
public:
    /***
      * ����һ��Epoll����
      * �����׳��κ��쳣
      */
    CEpoller();
    ~CEpoller();

    /***
      * ����Epoll�����г�ʼ��
      * @epoll_size: ������Epoll��С
      * @exception: ��������׳�CSyscallException�쳣
      */
    void create(uint32_t epoll_size);

    /***
      * �����Ѿ�������Epoll
      * �����׳��κ��쳣
      */
    void destroy();

    /***
      * �Գ�ʱ��ʽ�ȴ�Epoll���¼������ָ����ʱ�������¼�����ʱ����
      * @milliseconds: ��ȴ��ĺ����������Ǳ�֤�ȴ����ʱ������ʹ���ж�
      * @return: ����ڳ�ʱʱ���ڣ����¼����򷵻����¼��Ķ��������
      *          ���򷵻�0��ʾ�Ѿ���ʱ��
      * @exception: ��������׳�CSyscallException�쳣
      */
    int timed_wait(uint32_t milliseconds);

    /***
      * ��һ����Epoll�Ķ���ע�ᵽEpoll�����
      * @epollable: ָ���Epoll�����ָ��
      * @events: ��Ҫ��ص�Epoll�¼���ȡֵ����Ϊ: EPOLLIN��EPOLLOUT�ȣ�
      *          ������鿴Epollϵͳ����˵���ֲ�
      *          ͨ������Ҫ��ʾ����EPOLLERR��EPOLLHUP�����¼�����Ϊ��������
      *          �ᱻ�Զ�����
      * @force: �Ƿ�ǿ����������ʽ����
      * @exception: ��������׳�CSyscallException�쳣
      */
    void set_events(CEpollable* epollable, int events, bool force=false);

    /***
      * ��һ����Epoll�����Epoll��ɾ��
      * @epollable: ָ���Epoll�����ָ��
      * @exception: ��������׳�CSyscallException�쳣
      */
    void del_events(CEpollable* epollable);

    /***
      * ���ݱ�ŵõ�һ��ָ���Epoll�����ָ��
      * @index: ��ţ���ע��index������timed_wait�ɹ��ķ���ֵ��Χ��
      * @return: ����һ��ָ���Epoll�����ָ��
      */
    CEpollable* get(uint32_t index) const { return (CEpollable *)_events[index].data.ptr; }

    /***
      * ���ݱ�ŵõ�������Epoll�¼�
      * @index: ��ţ���ע��index������timed_wait�ɹ��ķ���ֵ��Χ��
      * @return: ���ط�����Epoll�¼�
      */
    uint32_t get_events(uint32_t index) const { return _events[index].events; }

private:
    int _epfd;
    uint32_t _epoll_size;
    uint32_t _max_events;
    struct epoll_event* _events; 
};

NET_NAMESPACE_END
#endif // EPOLLER_H

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
#ifndef LISTENER_H
#define LISTENER_H
#include "net/ip_node.h"
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/***
  * TCP����˼�������
  * ����������ĳ�˿��ϵļ����ͽ�����������
  */
class CListener: public CEpollable
{
public:
    /** ����һ��TCP����� */
    CListener();

    /** �Ƿ�ΪIPV6������ */
    bool is_ipv6() const { return _ip.is_ipv6(); }
    
    /***
      * ������ָ��IP�Ͷ˿��ϵļ���
      * @ip: ����IP��ַ
      * @port: �����˿ں�
      * @enabled_address_zero: �Ƿ�������0.0.0.0�ϼ�������ȫ�����Ĭ�ϲ�����
      * @exception: ��������������׳�CSyscallException�쳣
      */
    void listen(const ip_address_t& ip, uint16_t port, bool enabled_address_zero=false);
    void listen(const ipv4_node_t& ip_node, bool enabled_address_zero=false);
    void listen(const ipv6_node_t& ip_node, bool enabled_address_zero=false);

    /***
      * ������������
      * @peer_ip: �����洢�Զ˵�IP��ַ
      * @peer_port: �����洢�Զ˶˿ں�
      * @return: �µ�SOCKET���
      * @exception: ��������������׳�CSyscallException�쳣
      */
    int accept(ip_address_t& peer_ip, uint16_t& peer_port);
    
    /** �õ�������IP��ַ */
    const ip_address_t& get_listen_ip() const { return _ip; }

    /** �õ������Ķ˿ں� */
    uint16_t get_listen_port() const { return _port; }

private:    
    uint16_t _port;
    ip_address_t _ip;
};

NET_NAMESPACE_END
#endif // LISTENER_H

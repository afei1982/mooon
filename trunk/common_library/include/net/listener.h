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
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

class CListener: public CEpollable
{
public:
    CListener();
    /***
      * ������ָ��IP�Ͷ˿ڵļ���
      * @ip: ����IP��ַ�����ΪNULL������0.0.0.0��ַ������ע��ip����ֵ����Ϊ��0.0.0.0���͹㲥��ַ
      * @port: �����˿ں�
      * @exception: ��������������׳�CSyscallException�쳣
      */
    void listen(const char* ip, uint16_t port);

    /***
      * ������������
      * @peer_ip: �Զ�IP��ַ
      * @peer_port: �Զ˶˿ں�
      * @return: �µ�SOCKET���
      * @exception: ��������������׳�CSyscallException�쳣
      */
    int accept(uint32_t& peer_ip, uint16_t& peer_port);
    
    const char* get_listen_ip() const { return _ip; }
    uint16_t get_listen_port() const { return _port; }

private:    
    uint16_t _port;
    char _ip[IP_ADDRESS_MAX];
};

NET_NAMESPACE_END
#endif // LISTENER_H

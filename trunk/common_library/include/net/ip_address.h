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
#ifndef NET_IP_ADDRESS_H
#define NET_IP_ADDRESS_H
#include "net/net_config.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

/** IP��ַ������IPV4��IPV6 */
class ip_address_t
{
public:    
    /** ����һ��127.0.0.1��ַ */
    ip_address_t();
    
    /** ����һ��IPV4��ַ */
    ip_address_t(uint32_t ipv4);

    /***
      * ����һ��IPV6��ַ
      */
    ip_address_t(uint32_t* ipv6);
    
    /***
      * ����һ��������IPV6Ҳ������IPV4�ĵ�ַ
      * @ip: �ַ���IP��ַ�����ΪNULL������һ��0.0.0.0��IPV4��ַ
      * @exception: ���Ϊ��ЧIP��ַ�����׳�CSyscallException�쳣
      */
    ip_address_t(const char* ip);

    /** �������� */
    ip_address_t(const ip_address_t& ip);

    /** �ж��Ƿ�ΪIPV6��ַ */    
    bool is_ipv6() const;

    /** ת�����ַ��� */
    std::string to_string() const;

    /** �õ���ַ���� */
    const uint32_t* get_address_data() const;

    /** �ж��Ƿ�Ϊ0.0.0.0��ַ */
    bool is_zero_address() const;

    /** �ж��Ƿ�Ϊ�㲥��ַ */
    bool is_broadcast_address() const;

public: // ��ֵ�ͱȽϲ���
    void operator =(uint32_t ipv4);
    void operator =(uint32_t* ipv6);
    void operator =(const char* ip);
    void operator =(const ip_address_t& other);
    bool operator <(const ip_address_t& other) const;
    bool operator >(const ip_address_t& other) const;
    bool operator ==(const ip_address_t& other) const;    
    
private:
    bool _is_ipv6;
    uint32_t _ip_data[4];
};

NET_NAMESPACE_END
#endif // NET_IP_ADDRESS_H

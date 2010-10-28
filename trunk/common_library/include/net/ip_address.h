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
#include <vector>
#include <utility> // std::pair
#include "net/net_config.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

/** IP��ַ������IPV4��IPV6 */
class ip_address_t
{
public:    
    /** ����һ��127.0.0.1��ַ�����׳��쳣 */
    ip_address_t();
    
    /** ����һ��IPV4��ַ�����׳��쳣 */
    ip_address_t(uint32_t ipv4);

    /***
      * ����һ��IPV6��ַ
      * @exception: �����ΪNULL�����׳�CSyscallException�쳣
      */
    ip_address_t(const uint32_t* ipv6);
    
    /***
      * ����һ��IP��ַ������ΪIPV4��Ҳ��ΪIPV6
      * @ip: �ַ���IP��ַ�����ΪNULL������һ��0.0.0.0��IPV4��ַ
      * @exception: ���Ϊ��ЧIP��ַ�����׳�CSyscallException�쳣
      */
    ip_address_t(const char* ip);

    /** �������죬���׳��쳣 */
    ip_address_t(const ip_address_t& ip);

    /** �ж��Ƿ�ΪIPV6��ַ�����׳��쳣 */    
    bool is_ipv6() const;

    /** ת�����ַ��������׳��쳣 */
    std::string to_string() const;

    /** �õ���ַ���ݵ���Ч�ֽ��� */
    size_t get_address_data_length() const;

    /** �õ���ַ���� */
    const uint32_t* get_address_data() const;    

    /** �ж��Ƿ�Ϊ0.0.0.0��ַ */
    bool is_zero_address() const;

    /** �ж��Ƿ�Ϊ�㲥��ַ */
    bool is_broadcast_address() const;

public: // ��ֵ�ͱȽϲ���
    ip_address_t& operator =(uint32_t ipv4);
    ip_address_t& operator =(const uint32_t* ipv6);
    ip_address_t& operator =(const char* ip);
    ip_address_t& operator =(const ip_address_t& other);
    bool operator <(const ip_address_t& other) const;
    bool operator >(const ip_address_t& other) const;
    bool operator ==(const ip_address_t& other) const;    
    
private:
    void from_string(const char* ip);
    
private:
    bool _is_ipv6;
    uint32_t _ip_data[4];
};

inline ip_address_t::ip_address_t()
    :_is_ipv6(false)
{
    _ip_data[0] = 0;
}

inline ip_address_t::ip_address_t(uint32_t ipv4)
    :_is_ipv6(false)
{
    _ip_data[0] = ipv4;
}

inline ip_address_t::ip_address_t(const uint32_t* ipv6)
    :_is_ipv6(true)
{
    if (NULL == ipv6) throw sys::CSyscallException(EINVAL, __FILE__, __LINE__);
    memcpy(_ip_data, ipv6, sizeof(_ip_data));
}

inline ip_address_t::ip_address_t(const char* ip)
{
    from_string(ip);
}

inline ip_address_t::ip_address_t(const ip_address_t& ip)
{
    _is_ipv6 = ip.is_ipv6();
    memcpy(_ip_data, ip.get_address_data(), sizeof(_ip_data));
}

inline ip_address_t& ip_address_t::operator =(uint32_t ipv4)
{
    _is_ipv6 = false;
    _ip_data[0] =  ipv4;
    return *this;
}

inline ip_address_t& ip_address_t::operator =(const uint32_t* ipv6)
{
    if (NULL == ipv6) throw sys::CSyscallException(EINVAL, __FILE__, __LINE__);
    memcpy(_ip_data, ipv6, sizeof(_ip_data));
    return *this;
}

inline ip_address_t& ip_address_t::operator =(const char* ip)
{
    from_string(ip);
    return *this;
}

inline ip_address_t& ip_address_t::operator =(const ip_address_t& other)
{
    _is_ipv6 = other.is_ipv6();
    memcpy(_ip_data, other.get_address_data(), sizeof(_ip_data));
    return *this;
}

inline bool ip_address_t::operator <(const ip_address_t& other) const
{
    const uint32_t* ip_data = other.get_address_data();
    
    return _is_ipv6? (-1 == memcmp(_ip_data, ip_data, sizeof(_ip_data))): (_ip_data[0] < ip_data[0]);
}

inline bool ip_address_t::operator >(const ip_address_t& other) const
{
    const uint32_t* ip_data = other.get_address_data();
    return _is_ipv6? (1 == memcmp(_ip_data, ip_data, sizeof(_ip_data))): (_ip_data[0] > ip_data[0]);
}

inline bool ip_address_t::operator ==(const ip_address_t& other) const
{
    const uint32_t* ip_data = other.get_address_data();
    if (_is_ipv6)
    {
        if (other.is_ipv6())
            return 0 == memcmp(_ip_data, ip_data, sizeof(_ip_data));
        else
            return false;
    }
    else
    {
        if (other.is_ipv6())
            return false;
        else
            return _ip_data[0] == ip_data[0];
    }
}

inline bool ip_address_t::is_ipv6() const
{
    return _is_ipv6;
}

/** ip_address_t���͵�Hash���� */
typedef struct
{
    uint64_t operator()(const ip_address_t* ip_address) const
    {
        const uint32_t* address_data = ip_address->get_address_data();
        return ip_address->is_ipv6()? (address_data[1] + address_data[3]): address_data[0];
    }
}ip_address_hasher;

/** ip_address_t���͵ıȽϺ��� */
typedef struct
{
    bool operator()(const ip_address_t* lhs, const ip_address_t* rhs) const
    {
        return *lhs == *rhs;
    }
}ip_address_comparser;

/** ip_address_t����ָ������ */
typedef std::vector<ip_address_t*> ip_address_array_t;

/** IP�Ͷ˿ڶ� */
typedef std::pair<ip_address_t, port_t> ip_port_pair_t;

/** IP�Ͷ˿ڶ�����ָ������ */
typedef std::vector<ip_port_pair_t*> ip_port_pair_array_t;

NET_NAMESPACE_END
#endif // NET_IP_ADDRESS_H

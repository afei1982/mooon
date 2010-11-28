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
#ifndef NET_UTIL_H
#define NET_UTIL_H
#include <poll.h>
#include <vector>
#include "net/net_config.h"
#include "sys/syscall_exception.h"
NET_NAMESPACE_BEGIN

/***
  * ��������صĹ�����
  */
class CNetUtil
{
public:
    typedef std::string TEthName;   /** ������ */
    typedef std::string TStringIP;  /** IP��ַ */    
    typedef std::vector<TStringIP> TStringIPArray; /** IP��ַ���� */
    typedef std::vector<std::pair<TEthName, TStringIP> > TEthIPArray; /** ��������IP������ */

    /** �ж��Ƿ�ΪС�ֽ�������Ƿ���true�����򷵻�false */
    static bool is_little_endian();

    /***
      * ��ת�ֽ�
      * @source: Դ�ֽ�
      * @result: ��ת����ֽ�
      * @length: ��Ҫ��ת�ĳ���
      */
    static void reverse_bytes(const void* source, void* result, size_t length);
    
    /***
      * ��Դ���ݴ������ֽ���ת���������ֽ���
      * @source: ��Ҫת���������ֽ���Դ����
      * @result: ���ת����������ֽ���������
      * @length: ��Ҫת�����ֽڳ���
      */    
    static void host2net(const void* source, void* result, size_t length);

    /***
      * ��Դ���ݴ������ֽ���ת���������ֽ���
      * @source: ��Ҫת���������ֽ���Դ����
      * @result: ���ת����������ֽ���������
      * @length: ��Ҫת�����ֽڳ���
      */
    static void net2host(const void* source, void* result, size_t length);

    /***
      * ��Դ���ݴ������ֽ���ת���������ֽ���
      * @source: ��Ҫת���������ֽ���Դ����
      * @result: ���ת����������ֽ���������
      */
    template <typename DataType>
    static void host2net(const DataType& source, DataType& result)
    {
        host2net(&source, &result, sizeof(DataType));        
    }

    /***
      * ��Դ���ݴ������ֽ���ת���������ֽ���
      * @source: ��Ҫת���������ֽ���Դ����
      * @result: ���ת����������ֽ���������
      */
    template <typename DataType>
    static void net2host(const DataType& source, DataType& result)
    {
        CNetUtil::host2net<DataType>(source, result);
    }

    /***
      * ��ת�ֽ�
      * @source: Դ�ֽ�
      * @result: ��ת����ֽ�
      */
    template <typename DataType>
    static void reverse_bytes(const DataType* source, DataType* result)
    {
        CNetUtil::reverse_bytes(source, result, sizeof(DataType));
    }

    /** �жϸ������ַ����Ƿ�Ϊ������������ */
    static bool is_host_name(const char* str);
    
    /** �жϸ������ַ����Ƿ�Ϊһ��IPV4��IPV6��ַ
      * @return: ����������ַ�����һ��IPV4��IPV6��ַ���򷵻�true�����򷵻�false
      */
    static bool is_valid_ip(const char* str);

    /** �жϸ������ַ����Ƿ�Ϊһ��IPV4��ַ
      * @return: ����������ַ�����һ��IPV4��ַ���򷵻�true�����򷵻�false
      */
    static bool is_valid_ipv4(const char* str);

    /** �жϸ������ַ����Ƿ�Ϊһ��IPV6��ַ
      * @return: ����������ַ�����һ��IPV6��ַ���򷵻�true�����򷵻�false
      */
    static bool is_valid_ipv6(const char* str);

    /***
      * �����������õ�һ��IP��ַ
      * @hostname: ������
      * @ip_array: ���IP������
      * @errinfo: �������������Ϣ
      * @return: ����ɹ�����true�����򷵻�false
      * @exception: ���쳣�׳�
      */
    static bool get_ip_address(const char* hostname, TStringIPArray& ip_array, std::string& errinfo);
    
    /** �õ��������Ͷ�Ӧ��IP
      * @eth_ip_array: ���ڱ������л�ȡ����IP��ַ
      * @exception: ������������׳�CSyscallException�쳣
      */
    static void get_ethx_ip(TEthIPArray& eth_ip_array);

    /** �����������õ����ڸ������ϵ�����IP��ַ
      * @ethx: ���������磺eth0�����ΪNULL�����ʾ��������
      * @ip_array: ���ڱ������л�ȡ����IP��ַ
      * @exception: ������������׳�CSyscallException�쳣
      */
    static void get_ethx_ip(const char* ethx, TStringIPArray& ip_array);    

    /** �����������͵�IP���õ��ַ������͵�IP��ַ
      * @ip: �������͵�IP
      * @return: �ַ������͵�IP��ַ
      */
    static std::string ipv4_tostring(uint32_t ipv4);
    static std::string ipv6_tostring(const uint32_t* ipv6);    

    /** ��һ���ַ���ת����IPV4��ַ����
      * @source: ��Ҫת�����ַ���
      * @ipv4: �洢ת�����IPV4��ַ
      * @return: ת���ɹ�����true�����򷵻�false
      */
    static bool string_toipv4(const char* source, uint32_t& ipv4);

    /** ��һ���ַ���ת����IPV6��ַ����
      * @source: ��Ҫת�����ַ���
      * @ipv6: �洢ת�����IPV6��ַ������Ϊ������16�ֽڣ���: uint32_t[4]
      * @return: ת���ɹ�����true�����򷵻�false
      */
    static bool string_toipv6(const char* source, uint32_t* ipv6);
    
    /** �жϴ�����ַ����Ƿ�Ϊ�ӿ������磺eth0��
      * @return: ���str�ǽӿ������򷵻�true�����򷵻�false
      */
    static bool is_ethx(const char* str);

    /** �жϴ�����ַ����Ƿ�Ϊ�㲥��ַ
      * @str: IP��ַ�ַ���������ΪNULL
      * @return: ���strΪ�㲥��ַ���򷵻�true�����򷵻�false
      */
    static bool is_broadcast_address(const char* str);

    /** ��ʱPOLL����fd����
      * @fd: ��POLL�ĵ���fd��ע�ⲻ��fd����
      * @events_requested: �����ص��¼�
      * @milliseconds: �����ĺ����������Ǳ�֤�ȴ����ʱ������ʹ���ж�
      * @events_returned: �������淵�ص��¼������ΪNULL�������¼����أ�ͨ����ⷵ���¼���������ȷ���ĸ��¼�������
      * @return: ��ʱ����false�����¼�����true
      * @exception: ����������׳�CSyscallException�쳣
      */
    static bool timed_poll(int fd, int events_requested, int milliseconds, int* events_returned=NULL);
};

NET_NAMESPACE_END
#endif // NET_UTIL_H

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
#ifndef LISTEN_MANAGER_H
#define LISTEN_MANAGER_H
#include "net/ip_address.h"
NET_NAMESPACE_BEGIN

/***
  * ����������ģ����
  */
template <class ListenClass>
class CListenManager
{
public:
    CListenManager()
        :_listener_count(0)
        ,_listener_array(NULL)
    {
    }

    /***
      * ����IP�˿ڶԣ����ﲻ���ظ���飬���Ե���ǰ���뱣֤IP�Ͷ˿ڲ��ظ�������
      * Ӧ���ڵ���create֮ǰ����add�������ú���Ҫ�����ĵ�ַ�Ͷ˿ںţ�ֻҪIP+port���ظ�����
      * @ip: ����IP��ַ
      * @port: �����˿ں�
      * �����׳��κ��쳣
      */
    void add(const char* ip, port_t port)
    {
        NET_ASSERT(ip != NULL);
        ip_port_pair_t* ip_port_pair = new ip_port_pair_t;
        ip_port_pair->first = ip;
        ip_port_pair.second = port;
        _ip_port_array.push_back(ip_port_pair);
    }

    /***
      * ����������IP�Ͷ˿ڶ��ϵļ���
      * @exception: ����������׳�CSyscallException�쳣
      */
    void create()
    {
        _listener_array = new ListenClass[_ip_port_array.size()];

        for (ip_port_pair_array_t::size_type i=0; i<_ip_port_array.size(); ++i)
        {
            try
            {                
                _listener_array[i].listen(_ip_port_array[i].first, _ip_port_array[i].second);
                ++_listener_count;
            }
            catch (...)
            {
                destroy();
                throw;
            }
        }
    }

    /***
      * ���ٺ͹ر������ж˿��ϵļ���
      * �����׳��κ��쳣
      */
    void destroy()
    {
        do_destroy_ip_port_array();
        do_destroy_listener_array();                
    }

    /** �õ������߸��� */
    uint16_t get_listener_count() const { return _listener_count; }

    /** �õ�ָ������߶�������ָ�� */
    ListenClass* get_listener_array() const { return _listener_array; }

private:    
    void do_destroy_ip_port_array()
    {
        for (ip_port_pair_array_t::size_type i=0; i<_ip_port_array.size(); ++i)
            delete _ip_port_array[i];
        _ip_port_array.clear();
    }

    void do_destroy_listener_array()
    {
        uint16_t listen_counter = _listener_count;
        for (uint16_t i=listen_counter; i>0; --i)
        {
            _listener_array[i-1].close();
            --_listener_count;
        }

        delete []_listener_array;
        _listener_array = NULL;
    }
    
private:
    uint16_t _listener_count;
    ListenClass* _listener_array;
    ip_port_pair_array_t _ip_port_array;
};

NET_NAMESPACE_END
#endif // LISTEN_MANAGER_H

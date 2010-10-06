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
#include <vector>
#include "net/net_config.h"
NET_NAMESPACE_BEGIN

/***
  * ����������ģ����
  */
template <class ListenClass>
class CListenManager
{
    typedef std::vector<std::pair<std::string, uint16_t> > TIPPortArray;

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
    void add(const char* ip, uint16_t port)
    {
        NET_ASSERT(ip != NULL);
        _ipport_array.push_back(std::pair<std::string, uint16_t>(ip, port));
    }

    /***
      * ����������IP�Ͷ˿ڶ��ϵļ���
      * @exception: ����������׳�CSyscallException�쳣
      */
    void create()
    {
        _listener_array = new ListenClass[_ipport_array.size()];

        for (TIPPortArray::size_type i=0; i<_ipport_array.size(); ++i)
        {
            try
            {
                ip_address_t ip = _ipport_array[i].first.c_str();
                _listener_array[i].listen(ip, _ipport_array[i].second);
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
        uint16_t listen_counter = _listener_count;
        for (uint16_t i=listen_counter; i>0; --i)
        {
            _listener_array[i-1].close();
            --_listener_count;
        }

        delete []_listener_array;
        _listener_array = NULL;
    }

    /** �õ������߸��� */
    uint16_t get_listener_count() const { return _listener_count; }

    /** �õ�ָ������߶�������ָ�� */
    ListenClass* get_listener_array() const { return _listener_array; }

private:
    uint16_t _listener_count;
    ListenClass* _listener_array;
    TIPPortArray _ipport_array;
};

NET_NAMESPACE_END
#endif // LISTEN_MANAGER_H

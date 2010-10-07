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
#include "net/net_util.h"
#include "net/listener.h"
#include "sys/sys_util.h"
#include "net/tcp_waiter.h"
#include "util/string_util.h"

// �޲���ʱ��
// ��0.0.0.0:5174�ϼ���

// һ������ʱ��
// ��0.0.0.0�ϼ���
// argv[1]: �˿ں�

// ��������ʱ��
// argv[1]: ������IP��ַ
// argv[2]: �����Ķ˿ں�
int main(int argc, char* argv[])
{
    uint16_t port;
    net::ip_address_t ip;
    net::CListener listener;

    if (1 == argc) // �޲���
    {
        ip = (char*)NULL;
        port = 5174; // ��(5)Ҫ(1)��(7)��(4)
    }
    else if (2 == argc) // һ������
    {
        ip = (char*)NULL;
        if (!util::CStringUtil::string2uint16(argv[2], port))
        {
            fprintf(stderr, "Invalid port: %s.\n", argv[2]);
            exit(1);
        }
    }
    else if (3 == argc) // ��������
    {            
        ip =  argv[1];
        if (!util::CStringUtil::string2uint16(argv[2], port))
        {
            fprintf(stderr, "Invalid port: %s.\n", argv[2]);
            exit(1);
        }
    }
    
    try
    {
        // ��ʼ������������0.0.0.0�ϼ���
        listener.listen(ip, port, true);
        fprintf(stdout, "Listening at %s:%d.\n", ip.to_string().c_str(), port);
        
        uint16_t peer_port;
        net::ip_address_t peer_ip;

        while (true)
        {
            // ����һ����������
            int newfd = listener.accept(peer_ip, peer_port);
            fprintf(stdout, "Accepted connect - %s:%d.\n"
                , peer_ip.to_string().c_str(), peer_port);

            // ���µ����������CTcpWaiter��
            net::CTcpWaiter waiter;
            waiter.attach(newfd);

            while (true)
            {
                // ��������
                char buffer[IO_BUFFER_MAX];
                ssize_t retval = waiter.receive(buffer, sizeof(buffer)-1);
                if (0 == retval)
                {           
                    // �Զ˹ر�������
                    fprintf(stdout, "Connect closed by peer %s:%d.\n"
                        , peer_ip.to_string().c_str(), peer_port);
                    break;
                }
                else
                {
                    // ����Ļ�ϴ�ӡ���յ�������
                    buffer[retval] = '\0';
                    fprintf(stdout, "[R] ==> %s.\n", buffer);
                }
            }
        }
    }
    catch (sys::CSyscallException& ex)
    {
        // �����������쳣
        fprintf(stderr, "exception %s at %s:%d.\n"
            , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str()
            , ex.get_filename(), ex.get_linenumber());
        exit(1);
    }

    return 0;
}

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
#include <arpa/inet.h>
#include <net/net_util.h>

int main()
{
    // ת��2�ֽ�����
    uint16_t b1;
    uint16_t a1 = 0x0103;

    net::CNetUtil::host2net<uint16_t>(a1, b1);
    printf("a1 = 0x%04x, b1=0x%04x\n", a1, b1);
    if (b1 == htons(a1)) /** ��ϵͳ�⺯���Ƚϣ�����֤�Ƿ���ȷ */
        printf("host2net success\n");
    else
        printf("host2net failure\n");

    // ת��4�ֽ�����
    printf("\n");
    uint32_t b2;
    uint32_t a2 = 0x01016070;

    net::CNetUtil::host2net<uint32_t>(a2, b2);
    printf("a2 = 0x%04x, b2=0x%04x\n", a2, b2);
    if (b2 == htonl(a2)) /** ��ϵͳ�⺯���Ƚϣ�����֤�Ƿ���ȷ */
        printf("host2net success\n");
    else
        printf("host2net failure\n");

    // ������ת����Ӧ�õ����ֽ��ַ����ϣ��൱�ڷ�ת�ַ���
    printf("\n");
    char str[] = "123456789";
    size_t length = strlen(str);
    char* dst = new char[length+1];
    util::delete_helper<char> dh(dst, true); // �Զ�����delete []dst
    net::CNetUtil::host2net(str, dst, length);
    dst[length] = '\0';
    printf("%s ==> %s\n",str, dst);    

    return 0;
}

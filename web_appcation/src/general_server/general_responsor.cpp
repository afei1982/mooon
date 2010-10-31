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
#include "general_responsor.h"
MOOON_NAMESPACE_BEGIN

static char responsor_body[1024] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static char responsor_header[] = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Length: 1024\r\n\r\n";

CGeneralResponsor::CGeneralResponsor(CGeneralParser* parser)
    :_parser(parser)
{
    reset();
}

/** ��λ״̬ */
void CGeneralResponsor::reset()
{
    _offset = 0;
    _is_send_header = true;
}

/** �Ƿ񱣳����Ӳ��Ͽ���������һ������ */
bool CGeneralResponsor::keep_alive() const
{
    return true;
}

/** �Ƿ���һ���ļ� */
bool CGeneralResponsor::is_send_file() const
{
    return false;
}

/** �õ���Ҫ���͵Ĵ�С */
uint32_t CGeneralResponsor::get_size() const
{
    return _is_send_header? strlen(responsor_header): sizeof(responsor_body);
}

/** �õ�����ƫ�ƿ�ʼ���� */
uint32_t CGeneralResponsor::get_offset() const 
{
    return _offset;
}

/** �õ��ļ���� */
int CGeneralResponsor::get_fd() const     
{
    return -1;
}     

/** �õ���Ҫ���͵����� */
const char* CGeneralResponsor::get_buffer() const
{
    return _is_send_header? responsor_header: responsor_body;
}

/***
  * �ƶ�ƫ��
  * @offset: ���η��͵��ֽ���
  */
void CGeneralResponsor::move_offset(uint32_t offset)
{
    _offset += offset;
    if (_is_send_header)
    {
        if (_offset == strlen(responsor_header))
        {
            _offset = 0;
            _is_send_header = false;
        }
    }
}

MOOON_NAMESPACE_END

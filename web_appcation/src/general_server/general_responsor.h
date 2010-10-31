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
#ifndef GENERAL_RESPONSOR_H
#define GENERAL_RESPONSOR_H
#include <server/request_responsor.h>
#include "general_parser.h"
MOOON_NAMESPACE_BEGIN

class CGeneralResponsor: public IRequestResponsor
{
public:
    CGeneralResponsor(CGeneralParser* parser);
    
private:
    /** ��λ״̬ */
    virtual void reset();
    
    /** �Ƿ񱣳����Ӳ��Ͽ���������һ������ */
    virtual bool keep_alive() const;

    /** �Ƿ���һ���ļ� */
    virtual bool is_send_file() const;
    
    /** �õ���Ҫ���͵Ĵ�С */
    virtual uint32_t get_size() const;

    /** �õ�����ƫ�ƿ�ʼ���� */
    virtual uint32_t get_offset() const; 

    /** �õ��ļ���� */
    virtual int get_fd() const;              

    /** �õ���Ҫ���͵����� */
    virtual const char* get_buffer() const;

    /***
      * �ƶ�ƫ��
      * @offset: ���η��͵��ֽ���
      */
    virtual void move_offset(uint32_t offset);

private:    
    uint32_t _offset;
    bool _is_send_header;
    CGeneralParser* _parser;    
};

MOOON_NAMESPACE_END
#endif // GENERAL_RESPONSOR_H

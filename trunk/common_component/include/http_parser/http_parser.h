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
 * Author: JianYi, eyjian@qq.com
 */
#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H
#include <util/util_config.h>
MOOON_NAMESPACE_BEGIN

/***
  * Http�¼��ص��ӿ�
  */
class CALLBACK_INTERFACE IHttpEvent
{
public:    
    /** ���������������������α������澯 */
    virtual ~IHttpEvent() {}

    /** ��λ���� */
    virtual void reset() = 0;

    /** �Ѿ���������ͷβ */
    virtual bool on_head_end() = 0;
    
    /***
      * ��������
      * @errmsg: ������Ϣ
      */
    virtual void on_error(const char* errmsg) = 0;    

    /***
      * �Ѿ���������HTTP����
      * @begin: ��������ʼλ��
      * @end: ����������λ��
      * @return: ���������ȷ����true�����򷵻�false
      */
    virtual bool on_method(const char* begin, const char* end) = 0;

    /***
      * �Ѿ���������URL
      * @begin: URL��ʼλ��
      * @end: URL����λ��
      * @return: ���URL��ȷ����true�����򷵻�false
      */
    virtual bool on_url(const char* begin, const char* end) = 0;

    /***
      * �Ѿ��������İ汾�ţ���HTTP/1.1
      * @begin: �汾�ſ�ʼλ��
      * @end: �汾�Ž���λ��
      * @return: ����汾����ȷ����true�����򷵻�false
      */
    virtual bool on_version(const char* begin, const char* end) = 0;

    /***
      * �Ѿ�����������Ӧ����
      * @begin: ��Ӧ���뿪ʼλ��
      * @end: ��Ӧ�������λ��
      * @return: �����Ӧ������ȷ����true�����򷵻�false
      */
    virtual bool on_code(const char* begin, const char* end) = 0;

    /***
      * �Ѿ�����������Ӧ������������OK
      * @begin: ��Ӧ����������ʼλ��
      * @end: ��Ӧ������������λ��
      * @return: �����Ӧ����������ȷ����true�����򷵻�false
      */
    virtual bool on_describe(const char* begin, const char* end) = 0;

    /***
      * �Ѿ�����������ֵ�ԣ��磺host: www.hadoopor.com
      * @name_begin: ���ֵĿ�ʼλ��
      * @name_end: ���ֵĽ���λ��
      * @value_begin: ֵ�Ŀ�ʼλ��
      * @value_end: ֵ�Ľ���λ��
      * @return: �����ֵ����ȷ����true�����򷵻�false
      */
    virtual bool on_name_value_pair(const char* name_begin, const char* name_end
                                   ,const char* value_begin, const char* value_end) = 0;
};

/***
  * HTTPЭ��������ӿ�
  * ������ʽ����������������Ϊ����������ǰ��������ݣ�
  * ���֧�ֽ���һ����(��СΪһ���ֽ�)���ݣ��ͽ����ⲿ�ֵĽ���
  */
class IHttpParser
{
public:    
    /** ���������������������α������澯 */
    virtual ~IHttpParser() {}

    /** ��λ����״̬ */
	virtual void reset() = 0;

    /** ��ͷ�Ѿ�������� */
    virtual bool head_finished() const = 0;

    /** �õ���ͷ���ֽ��� */
    virtual int get_head_length() const = 0;

    /** �õ�HTTP�¼� */
    virtual IHttpEvent* get_http_event() const = 0;

    /** ����HTTP�¼� */
    virtual void set_http_event(IHttpEvent* event) = 0;

    /***
      * ִ�н���
      * @buffer: ��Ҫ������Buffer
      * @return: ��ο�TReturnResult��˵��
      */
    virtual util::handle_result_t parse(const char* buffer) = 0;
};

/** ����HTTPЭ������� */
extern void destroy_http_parser(IHttpParser* parser);

/** ����HTTPЭ������� */
extern IHttpParser* create_http_parser(bool is_request);

MOOON_NAMESPACE_END
#endif // HTTP_PARSER_H

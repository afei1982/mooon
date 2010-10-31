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
#ifndef GENERAL_EVENT_H
#define GENERAL_EVENT_H
#include <http_parser/http_parser.h>
MOOON_NAMESPACE_BEGIN

class CGeneralEvent: public IHttpEvent
{
private:    
    /** ��λ���� */
    virtual void reset();

    /** �Ѿ���������ͷβ */
    virtual bool on_head_end();
    
    /***
      * ��������
      * @errmsg: ������Ϣ
      */
    virtual void on_error(const char* errmsg);    

    /***
      * �Ѿ���������HTTP����
      * @begin: ��������ʼλ��
      * @end: ����������λ��
      * @return: ���������ȷ����true�����򷵻�false
      */
    virtual bool on_method(const char* begin, const char* end);

    /***
      * �Ѿ���������URL
      * @begin: URL��ʼλ��
      * @end: URL����λ��
      * @return: ���URL��ȷ����true�����򷵻�false
      */
    virtual bool on_url(const char* begin, const char* end);

    /***
      * �Ѿ��������İ汾�ţ���HTTP/1.1
      * @begin: �汾�ſ�ʼλ��
      * @end: �汾�Ž���λ��
      * @return: ����汾����ȷ����true�����򷵻�false
      */
    virtual bool on_version(const char* begin, const char* end);

    /***
      * �Ѿ�����������Ӧ����
      * @begin: ��Ӧ���뿪ʼλ��
      * @end: ��Ӧ�������λ��
      * @return: �����Ӧ������ȷ����true�����򷵻�false
      */
    virtual bool on_code(const char* begin, const char* end);

    /***
      * �Ѿ�����������Ӧ������������OK
      * @begin: ��Ӧ����������ʼλ��
      * @end: ��Ӧ������������λ��
      * @return: �����Ӧ����������ȷ����true�����򷵻�false
      */
    virtual bool on_describe(const char* begin, const char* end);

    /***
      * �Ѿ�����������ֵ�ԣ��磺host: www.hadoopor.com
      * @name_begin: ���ֵĿ�ʼλ��
      * @name_end: ���ֵĽ���λ��
      * @value_begin: ֵ�Ŀ�ʼλ��
      * @value_end: ֵ�Ľ���λ��
      * @return: �����ֵ����ȷ����true�����򷵻�false
      */
    virtual bool on_name_value_pair(const char* name_begin, const char* name_end
                                   ,const char* value_begin, const char* value_end);
};

MOOON_NAMESPACE_END
#endif // GENERAL_EVENT_H

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
 * ���������ҵ�Ѻõ�ApacheЭ�飬�������޸ĺͷַ������뱣����Ȩ˵�����֡�
 * �����������⣬�뷢�͵��������䣬�Ա㼰ʱ�޸���лл������������Դ�� 
 *
 * ���ݿ��������ʱ����Ҫ����CDBException�쳣�ķ�ʽ����
 */
#ifndef MYSQL_CONNECTION_H
#define MYSQL_CONNECTION_H
#include "sys/db.h"
#include "plugin/plugin.h"
PLUGIN_NAMESPACE_BEGIN

/***
  * MySQL��
  */
class CMySQLRow: public sys::IRecordrow
{
public:
    CMySQLRow(char** field_array);
    
private:    
    /***
      * ͨ���ֶα��ȡ���ֶε�ֵ(��Ч��)
      */
    virtual const char* get_field(uint16_t index) const;

    /***
      * ͨ���ֶ�����ȡ���ֶε�ֵ(��Ч��)
      */
    virtual const char* get_field(const char* filed_name) const;

private:
    char** _field_array;
};

/***
  * MySQL��¼��
  */
class CMySQLRecordset: public sys::IRecordset
{
public:
    CMySQLRecordset(void* resultset);

private:
    /***
      * �õ���¼��������
      */
    virtual size_t get_row_number() const;

    /***
      * �õ��ֶθ���
      */
    virtual size_t get_field_number() const;

    /***
      * �жϼ�¼���Ƿ�Ϊ��
      */
    virtual bool is_empty() const;

    /***
      * �������������һ��
      * @return: ���û��Ҫ�������з���NULL
      */
    virtual sys::IRecordrow* get_next_recordrow() const;

    /***
      * �ͷ�get_next_recordrow�õ��ļ�¼��
      */
    virtual void release_recordrow(sys::IRecordrow* recordrow);

private:
    void* _resultset; /** �洢MySQL����� */
};

/***
  * ���ݿ����ӽӿ�
  */
class CMySQLConnection: public sys::IDBConnection
{
public:    
    CMySQLConnection();
    ~CMySQLConnection();
    
    bool is_in_pool() const;
    void set_in_pool(bool yes); 
    void open(const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password);
    void close();

private:
    /** �Ƿ������Զ��ύ */
    virtual void enable_autocommit(bool enabled);
    
    /***
      * �����ж����ݿ������Ƿ��������� 
      */
    virtual bool is_established() const;
    
    /***
      * ���ݿ��ѯ�������������select, show, describe, explain��check table��
      * @is_stored: �Ƿ����м�¼���������ش洢
      * @return: ��ɹ����ؼ�¼����ָ��
      * @exception: ������׳�CDBException�쳣
      */
    virtual sys::IRecordset* query(bool is_stored, const char* format, ...);
    
    /***
      * �ͷ�query�õ��ļ�¼��
      */
    virtual void release_recordset(sys::IRecordset* recordset);

    /***
      * ���ݿ�insert��update���²���
      * @return: ��ɹ�������Ӱ��ļ�¼����
      * @exception: ������׳�CDBException�쳣
      */
    virtual size_t update(const char* format, ...);

private:
    bool _in_pool;          /** �Ƿ������ӳ��� */
    bool _is_established;   /** �Ƿ��Ѿ������ݿ⽨�������� */
    void* _mysql_handler;   /** MySQL���, ʹ��void������Ϊ����ͷ�ļ������� */
};

PLUGIN_NAMESPACE_END
#endif // MYSQL_CONNECTION_H

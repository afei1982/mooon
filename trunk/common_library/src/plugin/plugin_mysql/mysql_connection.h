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
#include "sys/ref_countable.h"
PLUGIN_NAMESPACE_BEGIN

/***
  * MySQL��
  */
class CMySQLRow: public sys::IRecordrow
{
public:
    CMySQLRow(char** field_array, uint16_t filed_number);
    
private:    
    /***
      * ͨ���ֶα��ȡ���ֶε�ֵ(��Ч��)
      */
    virtual const char* get_field_value(uint16_t index) const;

    /***
      * ͨ���ֶ�����ȡ���ֶε�ֵ(��Ч��)
      */
    virtual const char* get_field_value(const char* filed_name) const;

private:
    char** _field_array;
    uint16_t _filed_number;
};

/***
  * MySQL��¼��
  */
class CMySQLRecordset: public sys::IRecordset
{
public:
    CMySQLRecordset(void* resultset);
    ~CMySQLRecordset();

private:
    /***
      * �õ���¼��������
      * ����MySQL�����queryʱ������is_storedΪfalse����ú������ܷ�����ȷ��ֵ��
      * ����Ӧ��ֻ����is_storedΪtrue����ʹ�øú���
      */
    virtual size_t get_row_number() const;

    /***
      * �õ��ֶθ���
      */
    virtual uint16_t get_field_number() const;

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
    virtual void free_recordrow(sys::IRecordrow* recordrow);

private:
    void* _resultset; /** �洢MySQL����� */
};

/***
  * ���ݿ����ӽӿ�
  */
class CMySQLConnection
{
public:    
    CMySQLConnection();
    ~CMySQLConnection();
     
    void open(const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password);
    void close();

    /** �Ƿ������Զ��ύ */
    void enable_autocommit(bool enabled);
    
    /***
      * �����ж����ݿ������Ƿ��������� 
      */
    bool is_established() const;
    
    /***
      * ���ݿ��ѯ�������������select, show, describe, explain��check table��
      * @is_stored: �Ƿ����м�¼���������ش洢
      * @return: ��ɹ����ؼ�¼����ָ��
      * @exception: ������׳�CDBException�쳣
      */
    sys::IRecordset* query(bool is_stored, const char* format, va_list& args);
    
    /***
      * �ͷ�query�õ��ļ�¼��
      */
    void free_recordset(sys::IRecordset* recordset);

    /***
      * ���ݿ�insert��update���²���
      * @return: ��ɹ�������Ӱ��ļ�¼����
      * @exception: ������׳�CDBException�쳣
      */
    size_t update(const char* format, va_list& args);

private:
    bool _is_established;   /** �Ƿ��Ѿ������ݿ⽨�������� */
    void* _mysql_handler;   /** MySQL���, ʹ��void������Ϊ����ͷ�ļ������� */
};

class CMySQLGeneralConnection: public sys::IDBConnection
{
public:    
    CMySQLGeneralConnection()
    {
        _ref_countable = new sys::CRefCountable;
    }
    
    void open(const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password)
    {
        _mysql_connection.open(db_ip, db_port, db_name, db_user, db_password);
    }

    void close()
    {
        _mysql_connection.close();
    }

private:
    /** �����ü���ֵ��һ */
    virtual void inc_refcount()
    {
        _ref_countable->inc_refcount();
    }

    /***
      * �����ü���ֵ��һ
      * �����ȥ֮�����ü���ֵΪ0����ִ����ɾ��
      */
    virtual bool dec_refcount()
    {
        bool deleted = _ref_countable->dec_refcount();
        if (deleted)
            delete this;

        return deleted;
    }
    
    /** �Ƿ������Զ��ύ */
    virtual void enable_autocommit(bool enabled)
    {
        _mysql_connection.enable_autocommit(enabled);
    }
    
    /***
      * �����ж����ݿ������Ƿ��������� 
      */
    virtual bool is_established() const
    {
        return _mysql_connection.is_established();
    }
    
    /***
      * ���ݿ��ѯ�������������select, show, describe, explain��check table��
      * @is_stored: �Ƿ����м�¼���������ش洢
      * @return: ��ɹ����ؼ�¼����ָ��
      * @exception: ������׳�CDBException�쳣
      */
    virtual sys::IRecordset* query(bool is_stored, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        util::va_list_helper vlh(args);

        sys::IRecordset* recordset = _mysql_connection.query(is_stored, format, args);
        return recordset;
    }
    
    /***
      * �ͷ�query�õ��ļ�¼��
      */
    virtual void free_recordset(sys::IRecordset* recordset)
    {
        _mysql_connection.free_recordset(recordset);
    }

    /***
      * ���ݿ�insert��update���²���
      * @return: ��ɹ�������Ӱ��ļ�¼����
      * @exception: ������׳�CDBException�쳣
      */
    virtual size_t update(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        util::va_list_helper vlh(args);
        
        size_t affected_rows = _mysql_connection.update(format, args);
        return affected_rows;
    }

private:    
    sys::CRefCountable* _ref_countable;
    CMySQLConnection _mysql_connection;
};

class CMySQLPoolConnection: public sys::IDBPoolConnection
{
public:    
    CMySQLPoolConnection()
        :_in_pool(false)
    {        
    }
    
    bool is_in_pool() const
    {
        return _in_pool;
    }
    
    void set_in_pool(bool yes)
    {
        _in_pool = yes;
    }
    
    void open(const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password)
    {
        _mysql_connection.open(db_ip, db_port, db_name, db_user, db_password);
    }

    void close()
    {
        _mysql_connection.close();
    }

private:
    /** �Ƿ������Զ��ύ */
    virtual void enable_autocommit(bool enabled)
    {
        _mysql_connection.enable_autocommit(enabled);
    }
    
    /***
      * �����ж����ݿ������Ƿ��������� 
      */
    virtual bool is_established() const
    {
        return _mysql_connection.is_established();
    }
    
    /***
      * ���ݿ��ѯ�������������select, show, describe, explain��check table��
      * @is_stored: �Ƿ����м�¼���������ش洢
      * @return: ��ɹ����ؼ�¼����ָ��
      * @exception: ������׳�CDBException�쳣
      */
    virtual sys::IRecordset* query(bool is_stored, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        util::va_list_helper vlh(args);

        sys::IRecordset* recordset = _mysql_connection.query(is_stored, format, args);
        return recordset;
    }
    
    /***
      * �ͷ�query�õ��ļ�¼��
      */
    virtual void free_recordset(sys::IRecordset* recordset)
    {
        _mysql_connection.free_recordset(recordset);
    }

    /***
      * ���ݿ�insert��update���²���
      * @return: ��ɹ�������Ӱ��ļ�¼����
      * @exception: ������׳�CDBException�쳣
      */
    virtual size_t update(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        util::va_list_helper vlh(args);
        
        size_t affected_rows = _mysql_connection.update(format, args);
        return affected_rows;
    }

private:
    bool _in_pool; /** �Ƿ������ӳ��� */
    CMySQLConnection _mysql_connection;
};

PLUGIN_NAMESPACE_END
#endif // MYSQL_CONNECTION_H

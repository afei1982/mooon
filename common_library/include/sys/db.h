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
#ifndef SYS_DB_H
#define SYS_DB_H
#include "sys/sys_config.h"
#define SQL_MAX 4096 /** һ��SQL����������󳤶� */
SYS_NAMESPACE_BEGIN

class CDBException
{
public:
    /***
      * ����һ���쳣����
      * ��ע�ⲻӦ����ʾ���ù��캯��
      */
    CDBException(const char* sql, const char* error_message, int error_number=0, const char* filename=__FILE__, int line_number=__LINE__)
    {
        if (sql != NULL) _sql = sql;
        if (error_message != NULL) _error_message = error_message;
        if (filename != NULL) _filename = filename;
        _error_number = error_number;
        _line_number = line_number;
    }

    /** ����ִ�г����SQL��䣬�������ִ��SQL��䣬�������һ���ַ�����β�� */
    const char* get_sql() const
    {
        return _sql.c_str();
    }
    
    /** �������ݿ�ĳ�����Ϣ */
    const char* get_error_message() const
    { 
        return _error_message.c_str(); 
    }

    /** �������ݿ�ĳ������ */
    int get_error_number() const
    {
        return _error_number;
    }

    /** ����ִ�����ݿ����ʱ������ļ��� */
    const char* get_filename() const
    {
        return _filename.c_str();
    }

    /** ����ִ�����ݿ����ʱ����Ĵ����� */
    int get_line_number() const
    {
        return _line_number;
    }
    
private:   
    std::string _sql;
    std::string _error_message; /** ������Ϣ */
    int _error_number;          /** ����� */
    std::string _filename;      /** �׳��쳣���ļ��� */
    int _line_number;                  /** �׳��쳣�Ĵ����� */
};

/***
  * ��¼�нӿ�
  */
class IRecordrow
{
public:
    /** ��������������������Ӧ���������ĸ澯 */
    virtual ~IRecordrow() {}

    /***
      * ͨ���ֶα��ȡ���ֶε�ֵ
      */
    virtual const char* get_field_value(uint16_t index) const = 0;
};

/***
  * ��¼���ӿ�
  */
class IRecordset
{
public:
    /** ��������������������Ӧ���������ĸ澯 */
    virtual ~IRecordset() {}

    /***
      * �õ���¼��������
      * ����MySQL�����queryʱ������is_storedΪfalse����ú������ܷ�����ȷ��ֵ��
      * ����Ӧ��ֻ����is_storedΪtrue����ʹ�øú���
      */
    virtual size_t get_row_number() const = 0;

    /***
      * �õ��ֶθ���
      */
    virtual uint16_t get_field_number() const = 0;

    /***
      * �жϼ�¼���Ƿ�Ϊ��
      */
    virtual bool is_empty() const = 0;

    /***
      * �������������һ��
      * @return: ���û��Ҫ�������з���NULL�����򷵻�ָ���¼�е�ָ�룬��ʱ�������release_recordrow���������ڴ�й©
      */
    virtual IRecordrow* get_next_recordrow() const = 0;

    /***
      * �ͷ�get_next_recordrow�õ��ļ�¼��
      */
    virtual void release_recordrow(IRecordrow* recordrow) = 0;
};

/***
  * ���ݿ����ӽӿ�
  */
class IDBConnection
{
public:    
    /** ��������������������Ӧ���������ĸ澯 */
    virtual ~IDBConnection() {}

    /** �Ƿ������Զ��ύ */
    virtual void enable_autocommit(bool enabled) = 0;  
    
    /***
      * �����ж����ݿ������Ƿ��������� 
      */
    virtual bool is_established() const = 0;

    /***
      * ���ݿ��ѯ�������������select, show, describe, explain��check table��
      * @is_stored: �Ƿ����м�¼���������ش洢
      * @return: ��ɹ����ؼ�¼����ָ�룬��ʱ�������release_recordset���������ڴ�й©
      * @exception: ������׳�CDBException�쳣
      */
    virtual IRecordset* query(bool is_stored, const char* format, ...) = 0;
    
    /***
      * �ͷ�query�õ��ļ�¼��
      */
    virtual void release_recordset(IRecordset* recordset) = 0;

    /***
      * ���ݿ�insert��update���²���
      * @return: ��ɹ�������Ӱ��ļ�¼����
      * @exception: ������׳�CDBException�쳣
      */
    virtual size_t update(const char* format, ...) = 0;
};

/***
  * ���ݿ����ӳؽӿ�
  */
class IDBConnectionPool
{
public:
    /** ��������������������Ӧ���������ĸ澯 */
    virtual ~IDBConnectionPool() {}

    /***
      * �õ�ȫСд��ʽ�����ݿ����������磺mysql��postgresql��
      */
    virtual const char* get_type_name() const = 0;
    
    /***
      * �̰߳�ȫ����
      * �����ݿ����ӳ��л�ȡһ������
      * @return: �����ǰ�޿��õ����ӣ��򷵻�NULL�����򷵻�ָ�����ݿ����ӵ�ָ��
      * @exception: �����׳��κ��쳣
      */
    virtual IDBConnection* get_connection() = 0;

    /***
      * �̰߳�ȫ����
      * ���Ѿ���ȡ�����ݿ����ӷŻص����ݿ����ӳ���      
      * @exception: �����׳��κ��쳣
      */
    virtual void release_connection(IDBConnection* db_connection) = 0;

    /***
      * �������ӳ�
      * @pool_size: ���ݿ����ӳ��е����ݿ����Ӹ���
      * @db_ip: ��Ҫ���ӵ����ݿ�IP��ַ
      * @db_port: ��Ҫ���ӵ����ݿ����˿ں�
      * @db_name: ��Ҫ���ӵ����ݿ��
      * @db_user: �������ݿ��õ��û���
      * @db_password: �������ݿ��õ�����
      * @exception: ������׳�CDBException�쳣
      */
    virtual void create(uint16_t pool_size, const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password) = 0;

    /***
      * �����Ѿ����������ݿ����ӳ�
      */
    virtual void destroy() = 0;

    /***
      * �õ����ӳ��е����Ӹ���
      */
    virtual uint16_t get_connection_number() const = 0;
};

//////////////////////////////////////////////////////////////////////////
// ������: DBConnectionHelper, RecordsetHelper��RecordrowHelper

/***
  * DB���������࣬�����Զ��ͷ��Ѿ���ȡ��DB����
  */
class DBConnectionHelper
{
public:
    DBConnectionHelper(IDBConnectionPool* db_connection_pool, IDBConnection*& db_connection)
        :_db_connection_pool(db_connection_pool)
        ,_db_connection(db_connection)
    {
    }
    
    ~DBConnectionHelper()
    {
        if ((_db_connection_pool != NULL) && (_db_connection != NULL))
        {
            _db_connection_pool->release_connection(_db_connection);
            _db_connection = NULL;
        }
    }

private:
    IDBConnectionPool* _db_connection_pool;
    IDBConnection*& _db_connection;
};

/***
  * ��¼�������࣬�����Զ��ͷ��Ѿ���ȡ�ļ�¼��
  */
class RecordsetHelper
{
public:
    RecordsetHelper(IDBConnection* db_connection, IRecordset* recordset)
        :_db_connection(db_connection)
        ,_recordset(recordset)
    {        
    }

    ~RecordsetHelper()
    {
        if ((_db_connection != NULL) && (_recordset != NULL))
            _db_connection->release_recordset(_recordset);
    }

private:
    IDBConnection* _db_connection;
    IRecordset* _recordset;
};

/***
  * ��¼�������࣬�����Զ��ͷ��Ѿ���ȡ�ļ�¼��
  */
class RecordrowHelper
{
public:
    RecordrowHelper(IRecordset* recordset, IRecordrow* recordrow)
        :_recordset(recordset)
        ,_recordrow(recordrow)
    {        
    }

    ~RecordrowHelper()
    {
        if ((_recordset != NULL) && (_recordrow != NULL))
            _recordset->release_recordrow(_recordrow);
    }

private:
    IRecordset* _recordset;
    IRecordrow* _recordrow;
};

SYS_NAMESPACE_END
#endif // SYS_DB_H

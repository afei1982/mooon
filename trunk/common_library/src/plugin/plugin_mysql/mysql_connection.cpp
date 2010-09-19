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
//#include <my_global.h> // ��Щ�汾��MySQL������Ҫ������ͷ�ļ�
//#include <my_sys.h>    // ��Щ�汾��MySQL������Ҫ������ͷ�ļ�
#include <mysql.h>
#include "mysql_connection.h"
#include "util/string_util.h"
PLUGIN_NAMESPACE_BEGIN

inline MYSQL_RES* get_resultset(void* resultset)
{
    return (MYSQL_RES*)resultset;
}

inline MYSQL* get_mysql_handler(void* mysql_handler)
{
    return (MYSQL*)mysql_handler;
}

//////////////////////////////////////////////////////////////////////////
// CMySQLRow

CMySQLRow::CMySQLRow(char** field_array, uint16_t filed_number)
    :_field_array(field_array)
    ,_filed_number(filed_number)
{
}

const char* CMySQLRow::get_field(uint16_t index) const
{
    return (index < _filed_number)? _field_array[index]: NULL;
}

const char* CMySQLRow::get_field(const char* filed_name) const
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CMySQLRecordset

CMySQLRecordset::CMySQLRecordset(void* resultset)
    :_resultset(resultset)
{
}

CMySQLRecordset::~CMySQLRecordset()
{
    if (_resultset != NULL)
    {
        mysql_free_result(get_resultset(_resultset));
        _resultset = NULL;
    }
}

size_t CMySQLRecordset::get_row_number() const
{
    return (size_t)mysql_num_rows(get_resultset(_resultset));
}

uint16_t CMySQLRecordset::get_field_number() const
{
    return (size_t)mysql_num_fields(get_resultset(_resultset));
}

bool CMySQLRecordset::is_empty() const
{
    return 0 == get_row_number();
}

sys::IRecordrow* CMySQLRecordset::get_next_recordrow() const
{
    MYSQL_ROW recordrow = mysql_fetch_row(get_resultset(_resultset));
    return (NULL == recordrow)? NULL: new CMySQLRow((char**)recordrow, get_field_number());
}

void CMySQLRecordset::release_recordrow(sys::IRecordrow* recordrow)
{
    delete (CMySQLRow*)recordrow;
}

//////////////////////////////////////////////////////////////////////////
// CMySQLConnection

CMySQLConnection::CMySQLConnection()
    :_in_pool(true)
    ,_is_established(false)
    ,_mysql_handler(NULL)
{
}

CMySQLConnection::~CMySQLConnection()
{
    close();
}

bool CMySQLConnection::is_in_pool() const
{
    return _in_pool;
}

void CMySQLConnection::set_in_pool(bool yes)
{
    _in_pool = yes;
}

void CMySQLConnection::open(const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password)
{
    my_bool auto_reconnect = 1; // �����Զ�������

    // ������ʼ����mysql_real_connect()����Ӧ��MYSQL�������mysql��NULLָ�룬�ú��������䡢��ʼ��������
    // ���¶��󡣷��򣬽���ʼ�����󣬲����ض���ĵ�ַ�����mysql_init()�������µĶ��󣬵�����mysql_close()���ر�
    // ����ʱ�����ͷŸö���
    _mysql_handler = mysql_init(NULL);
    
	mysql_options(get_mysql_handler(_mysql_handler), MYSQL_OPT_RECONNECT, &auto_reconnect);

    try
    {    
        if (NULL == mysql_real_connect(get_mysql_handler(_mysql_handler), db_ip, db_user, db_password, db_name, db_port, NULL, 0))
            throw sys::CDBException(NULL, mysql_error(get_mysql_handler(_mysql_handler)), mysql_errno(get_mysql_handler(_mysql_handler)), __FILE__, __LINE__);    

        _is_established = true;
    }
    catch (sys::CDBException& ex)
    {
        close();
        throw;
    }    
}

void CMySQLConnection::close()
{    
    if (_mysql_handler != NULL)
    {
        _is_established = false;
        mysql_close(get_mysql_handler(_mysql_handler));
        _mysql_handler = NULL;        
    }
}

void CMySQLConnection::enable_autocommit(bool enabled)
{
    mysql_autocommit(get_mysql_handler(_mysql_handler), enabled? 1: 0);
}

bool CMySQLConnection::is_established() const
{
    return _is_established;
}

sys::IRecordset* CMySQLConnection::query(bool is_stored, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);

    char sql[SQL_MAX];    
    int sql_length = util::CStringUtil::fix_vsnprintf(sql, sizeof(sql), format, args);

    // �����ѯ�ɹ�������0��������ִ��󣬷��ط�0ֵ
    if (mysql_real_query(get_mysql_handler(_mysql_handler), sql, (unsigned long)sql_length) != 0)
        throw sys::CDBException(sql, mysql_error(get_mysql_handler(_mysql_handler)), mysql_errno(get_mysql_handler(_mysql_handler)), __FILE__, __LINE__);
    
    MYSQL_RES* resultset = is_stored? mysql_store_result(get_mysql_handler(_mysql_handler)): mysql_use_result(get_mysql_handler(_mysql_handler));
    if (NULL == resultset)
        throw sys::CDBException(sql, mysql_error(get_mysql_handler(_mysql_handler)), mysql_errno(get_mysql_handler(_mysql_handler)), __FILE__, __LINE__);

    return new CMySQLRecordset(resultset);
}

void CMySQLConnection::release_recordset(sys::IRecordset* recordset)
{
    delete (CMySQLRecordset*)recordset;
}

size_t CMySQLConnection::update(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);

    char sql[SQL_MAX];    
    int sql_length = util::CStringUtil::fix_vsnprintf(sql, sizeof(sql), format, args);

    // �����ѯ�ɹ�������0��������ִ��󣬷��ط�0ֵ
    if (mysql_real_query(get_mysql_handler(_mysql_handler), sql, (unsigned long)sql_length) != 0)
        throw sys::CDBException(sql, mysql_error(get_mysql_handler(_mysql_handler)), mysql_errno(get_mysql_handler(_mysql_handler)), __FILE__, __LINE__);
    
    return (size_t)mysql_affected_rows(get_mysql_handler(_mysql_handler));
}

PLUGIN_NAMESPACE_END

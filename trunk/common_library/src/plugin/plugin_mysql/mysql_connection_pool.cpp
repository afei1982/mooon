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
#include "mysql_connection_pool.h"
#include "plugin/plugin_mysql/plugin_mysql.h"
PLUGIN_NAMESPACE_BEGIN

CMySQLConnectionPool::CMySQLConnectionPool()
    :_db_port(3306) // MySQLĬ�϶˿�    
    ,_connect_array(NULL)
    ,_connection_queue(NULL)
{

}

const char* CMySQLConnectionPool::get_type_name() const
{
    return "mysql";
}

sys::IDBConnection* CMySQLConnectionPool::get_connection()
{
    sys::CLockHelper<sys::CLock> lock(_lock);
    if (_connection_queue->is_empty()) return NULL;
    
    CMySQLConnection* db_connection = _connection_queue->pop_front();
    db_connection->set_in_pool(false);
    return db_connection;
}

void CMySQLConnectionPool::release_connection(sys::IDBConnection* db_connection)
{
    if (db_connection != NULL)
    {
        CMySQLConnection* mysql_connection = (CMySQLConnection*)db_connection;
        sys::CLockHelper<sys::CLock> lock(_lock);
        // ����is_in_pool��Ϊ�˷�ֹ�ظ��Ž�ȥ������ʹ���Ѷ�
        if (!_connection_queue->is_full() && !mysql_connection->is_in_pool())
        {            
            _connection_queue->push_back(mysql_connection);
            mysql_connection->set_in_pool(true);
        }
    }
}

void CMySQLConnectionPool::create(uint16_t pool_size, const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password)
{
    // �����������ݿ����Ϣ
    _db_ip = db_ip;
    _db_port = db_port;
    _db_name = db_name;
    _db_user = db_user;
    _db_password = db_password;

    // �������Ӷ���
    uint16_t db_connection_number = (0 ==pool_size)? 1: pool_size;
    _connection_queue = new util::CArrayQueue<CMySQLConnection*>(db_connection_number);    
    _connect_array = new CMySQLConnection[db_connection_number];

    try
    {    
        for (uint16_t i=0; i<db_connection_number; ++i)
        {
            CMySQLConnection* connection = &_connect_array[i];
            connection->open(db_ip, db_port, db_name, db_user, db_password);
        }
    }
    catch (sys::CDBException& ex)
    {
        destroy();
        throw;
    }
}

void CMySQLConnectionPool::destroy()
{
    if (_connection_queue != NULL)
    {
        delete _connection_queue;
        _connection_queue = NULL;
    }
    if (_connect_array != NULL)
    {
        delete []_connect_array;
        _connect_array = NULL;
    }
}

uint16_t CMySQLConnectionPool::get_connection_number() const
{
    return (uint16_t)_connection_queue->size();
}

//////////////////////////////////////////////////////////////////////////
// ���ں���
sys::IDBConnectionPool* create_mysql_connection_pool()
{
    return new CMySQLConnectionPool();
}

void destroy_mysql_connection_pool(sys::IDBConnectionPool* db_connection_pool)
{
    delete (CMySQLConnectionPool*)db_connection_pool;
}

PLUGIN_NAMESPACE_END

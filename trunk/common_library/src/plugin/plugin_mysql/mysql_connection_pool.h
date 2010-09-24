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
#ifndef MYSQL_CONNECTION_POOL_H
#define MYSQL_CONNECTION_POOL_H
#include "sys/lock.h"
#include "mysql_connection.h"
#include "util/array_queue.h"
PLUGIN_NAMESPACE_BEGIN

/***
  * ���ݿ����ӳؽӿ�
  */
class CMySQLConnectionPool: public sys::IDBConnectionPool
{
public:
    CMySQLConnectionPool();

private:
    /***
      * �õ�ȫСд��ʽ�����ݿ����������磺mysql��postgresql��
      */
    virtual const char* get_type_name() const;
    
    /***
      * �����ݿ����ӳ��л�ȡһ������
      * @return: �����ǰ�޿��õ����ӣ��򷵻�NULL�����򷵻�ָ�����ݿ����ӵ�ָ��
      * @exception: �����׳��κ��쳣
      */
    virtual sys::IDBPoolConnection* get_connection();

    /***
      * ���Ѿ���ȡ�����ݿ����ӷŻص����ݿ����ӳ���
      * @exception: �����׳��κ��쳣
      */
    virtual void put_connection(sys::IDBPoolConnection* db_connection);

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
    virtual void create(uint16_t pool_size, const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password);

    /***
      * �����Ѿ����������ݿ����ӳ�
      */
    virtual void destroy();

    /***
      * �õ����ӳ��е����Ӹ���
      */
    virtual uint16_t get_connection_number() const;

private: // �������ݿ���
    uint16_t _db_port;        /** ���ݿ����˿ں� */
    std::string _db_ip;       /** ���ݿ����IP��ַ */
    std::string _db_name;     /** ���ݿ��� */
    std::string _db_user;     /** �������ݿ���û��� */
    std::string _db_password; /** �������ݿ������ */

private:
    sys::CLock _lock;
    CMySQLPoolConnection* _connect_array;
    util::CArrayQueue<CMySQLPoolConnection*>* _connection_queue;    
};

/***
  * ���ݿ����ӹ��������ڴ���DBGeneralConnection���͵�����
  */
class CMySQLConnectionFactory: public sys::IDBConnectionFactory
{
private:
    /***
      * ����DBGeneralConnection���͵�����
      * �̰߳�ȫ
      * @db_ip: ��Ҫ���ӵ����ݿ�IP��ַ
      * @db_port: ��Ҫ���ӵ����ݿ����˿ں�
      * @db_name: ��Ҫ���ӵ����ݿ��
      * @db_user: �������ݿ��õ��û���
      * @db_password: �������ݿ��õ�����
      * @exception: ������׳�CDBException�쳣
      */
    virtual sys::IDBConnection* create_connection(const char* db_ip, uint16_t db_port, const char* db_name, const char* db_user, const char* db_password);

    /***
      * �������ݿ����ӳ�
      * @return: ����ָ�����ݿ����ӳص�ָ��
      * @exception: ������׳�CDBException�쳣
      */
    virtual sys::IDBConnectionPool* create_connection_pool();

    /***
      * �������ݿ����ӳ�
      * @db_connection_pool: ָ����Ҫ���ٵ����ݿ����ӳص�ָ��
      */
    virtual void destroy_connection_pool(sys::IDBConnectionPool*& db_connection_pool);
};

PLUGIN_NAMESPACE_END
#endif // MYSQL_CONNECTION_POOL_H

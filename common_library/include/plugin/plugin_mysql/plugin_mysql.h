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
#ifndef PLUGIN_MYSQL_H
#define PLUGIN_MYSQL_H
#include "sys/db.h"
#include "plugin/plugin.h"
PLUGIN_NAMESPACE_BEGIN

/** �õ����ݿ����ӹ��� */
extern "C" sys::IDBConnectionFactory* get_mysql_connection_factory();

PLUGIN_NAMESPACE_END
#endif // PLUGIN_MYSQL_H

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
 * Author: jian yi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef CLUSTER_NODE_TABLE_H
#define CLUSTER_NODE_TABLE_H
#include <map>
#include <set>
#include <stdexcept>
#include "cluster_util/node.h"
#include "sys/read_write_lock.h"
#include "sys/syscall_exception.h"
#include "util/file_format_exception.h"
MY_NAMESPACE_BEGIN

/** �ڵ�������͹������нڵ�
  * �̰߳�ȫ��
  */
class CNodeTable
{
public:
    /** �ڵ�ID�� */
    typedef std::set<uint32_t> node_id_set_t;

public:
    /** ����ڵ��
      * @ip_uniq: ID��IP�Ƿ�һһ��Ӧ���������Ƿ��ΪΨһ�ģ����ظ�
      */
    CNodeTable(bool ip_uniq=true);
    ~CNodeTable();
    
    /** �õ�ָ��ID�Ľڵ�
      * @node_id: �ڵ�ID
      * @inc_refcount: �Ƿ��������ü�����������������ü���������ʹ�����Ӧ�������ü���
      * @return: ����ָ��ID�Ľڵ�ָ�룬����ڵ㲢�����ڣ��򷵻�NULL
      */
    CNode* get_node(uint32_t node_id, bool inc_refcount=false);

    /** ����IP���ж��Ƿ��ж�Ӧ�Ľڵ���ڣ�
      * ��ע��һ��IP�ɶ�Ӧ�����ͬ��Node
      */
    bool node_exist(uint32_t node_ip);

    /** ͨ��IP�õ��������еĵĽڵ�ID
      */
    node_id_set_t* get_node_id_set(uint32_t node_ip);

    /** ���ݽڵ�ID��IP����һ���ڵ�
      * @node_id: �ڵ�ID
      * @node_ip: �ڵ�IP��ַ
      * @managed: �Ƿ�Ϊ�ܿؽڵ�
      * @exception: ���node_id����NODE_ID_MAX�����׳�range_error�쳣
      * @return: �ɹ�����ָ��CNode��ָ�룬��������ڵ��Ѿ����ڻ�ڵ�ID��Ч�򷵻�NULL
      */
    CNode* add_node(uint32_t node_id, uint32_t node_ip, bool managed);
    
    /** ɾ���ڵ�
      * @node: ָ����Ҫɾ���Ľڵ��ָ��
      * @exception: ���node_id����NODE_ID_MAX�����׳�range_error�쳣
      */
    void del_node(CNode* node);

    /** ���ļ��м��ؽڵ���ļ�ÿһ�и�ʽ��Ϊ��
      * �ڵ�ID	�ڵ�IP	�ڵ�����	�ڵ������Ļ���ID	�ڵ�������IDC��ID�����ֶ��Կո��TAB�ָ�
      * @1: �ڵ�ID����Ϊ��ֵ
      * @2: �ڵ�IP����Ϊ��Ч��IPV4��ַ
      * @3: �ڵ�����ֻ��Ϊ0��1������1��ʾΪ�ܿؽڵ�
      * @4: �ڵ���������IDΪ-1��ʾ������
      * @5: �ڵ�����IDC IDΪ-1��ʾ������
      * @ignore_duplicate: �Ƿ�����ظ���
      * @exception: �ļ���ʽ������׳�CFileFormatException�쳣�����ļ��������׳�CSyscallException�쳣
      */
    void load(const char* filename, bool ignore_duplicate);
    
    CNode** get_node_array() const { return _node_array; }
    uint32_t get_node_number() const { return _node_number; }

private:
    /** ɾ����������нڵ� */
    void clear_nodes();
    void do_del_node(CNode* node);
    CNode* do_add_node(uint32_t node_id, uint32_t node_ip, bool managed);

private:
    sys::CReadWriteLock _lock;   
    bool _ip_uniq;         /** ID��IP�Ƿ�һһ��Ӧ���������Ƿ��ΪΨһ�ģ����ظ� */
    CNode** _node_array;   /** ��Žڵ�ָ������� */
    volatile uint32_t _node_number; /** ʵ�ʽڵ���� */    
    typedef std::map<uint32_t, node_id_set_t* > ip_table_t;
    ip_table_t _ip_table;
};

MY_NAMESPACE_END
#endif // CLUSTER_NODE_TABLE_H

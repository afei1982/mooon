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
#ifndef CLUSTER_NODE_H
#define CLUSTER_NODE_H
#include "util/util_config.h"
#include "sys/ref_countable.h"

/** ��Ч�Ľڵ�IDֵ */
#define INVALID_NODE_ID 0xFFFFFFFF

/** �ܿؽڵ�ID��ȡ�����ֵ(������)��ͨ��Ϊ��Ⱥ��Ľڵ� */
#define MANAGED_NODE_ID_MAX   100000

/** ���ܿؽڵ����������ͨ��Ϊ��Ⱥ��ڵ㣬��ID��MANAGED_NODE_ID_MAX+1��ʼ */
#define UNMANAGED_NODE_NUMBER 1000

/** ���ڵ�ID���ڵ������ܳ������ڵ�IDֵ��NODE_ID_MAX��ֵ����С��INVALID_NODE_ID */
#define NODE_ID_MAX (MANAGED_NODE_ID_MAX+UNMANAGED_NODE_NUMBER)

/** ��Ч��IDC IDֵ */
#define INVALID_IDC_ID 0xFFFFFFFF
/** IDC ID��ȡ�����ֵ��IDC�����ܳ������IDC IDֵ��IDC_ID_MAX��ֵ����С��INVALID_IDC_ID */
#define IDC_ID_MAX 100

/** ��Ч�Ļ���IDֵ */
#define INVALID_RACK_ID 0xFFFFFFFF
/** ����ID��ȡ�����ֵ�����������ܳ������ڵ�IDֵ��RACK_ID_MAX��ֵ����С��INVALID_RACK_ID */
#define RACK_ID_MAX 1000

MY_NAMESPACE_BEGIN

/** �ж��Ƿ�Ϊ��Ч�ڵ�IDϵ�к��� */
bool is_valid_node_id(int id);
bool is_valid_node_id(uint32_t id);

/** �ж��Ƿ�Ϊ��Ч����IDϵ�к��� */
bool is_valid_rack_id(int id);
bool is_valid_rack_id(uint32_t id);

/** �ж��Ƿ�Ϊ��ЧIDC IDϵ�к��� */
bool is_valid_idc_id(int id);
bool is_valid_idc_id(uint32_t id);

class CNode: public sys::CRefCountable
{
public:
    CNode(uint32_t id, uint32_t ip, bool managed);
    uint32_t get_id() const { return _id; }
    uint32_t get_ip() const { return _ip; }
    uint32_t get_owner_idc_id() const { return _owner_idc_id; }
    uint32_t get_owner_rack_id() const { return _owner_rack_id; }
    void set_owner_idc_id(uint32_t owner_idc_id) { _owner_idc_id = owner_idc_id; }
    void set_owner_rack_id(uint32_t owner_rack_id) { _owner_rack_id = owner_rack_id; }

    /** �жϽڵ��Ƿ��ܿ� */
    bool is_managed() const { return _managed; }

    /** �жϽڵ��Ƿ���� */
    bool is_active() const { return (_boot_timestamp > _stop_timestamp); }    
    time_t get_boot_timestamp() const { return _boot_timestamp; }
    time_t get_stop_timestamp() const { return _stop_timestamp; }
    void update_boot_timestamp(time_t boot_timestamp) { _boot_timestamp = boot_timestamp; }
    void update_stop_timestamp(time_t stop_timestamp) { _stop_timestamp = stop_timestamp; }

private:
    uint32_t _id;
    uint32_t _ip;
    uint32_t _owner_idc_id;  /** �ڵ�¼����IDC */
    uint32_t _owner_rack_id; /** �ڵ�¼���Ļ��� */
    bool _managed;           /* �ڵ����ͣ��Ƿ��ܿ� */

private: // ״ֵ̬    
    volatile time_t _boot_timestamp; /** �ڵ�����ʱ�� */
    volatile time_t _stop_timestamp; /** �ڵ�ֹͣʱ�� */
};

MY_NAMESPACE_END
#endif // CLUSTER_NODE_H

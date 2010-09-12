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
#ifndef CLUSTER_RACK_TABLE_H
#define CLUSTER_RACK_TABLE_H
#include <stdexcept>
#include "cluster_util/rack.h"
#include "sys/read_write_lock.h"
MY_NAMESPACE_BEGIN

/** ���ܱ������͹������л���
  * �̰߳�ȫ��
  */
class CRackTable
{
public:
    CRackTable();
    ~CRackTable();
    
    /** �ж�ָ����ID�Ļ����Ƿ����
      * @return: ������ܴ��ڷ���true�����򷵻�false
      */
    bool rack_exist(uint32_t rack_id);

    /** ���ݻ���ID��IP����һ������
      * @exception: ���rack_id����RACK_ID_MAX�����׳�range_error�쳣
      * @return: �ɹ�����ָ��CRack��ָ�룬������������Ѿ������򷵻�NULL
      */
    CRack* add_rack(uint32_t rack_id);
    
    /** ɾ���ڵ�
      * @exception: ���rack_id����RACK_ID_MAX�����׳�range_error�쳣
      */
    void del_rack(CRack* rack);

private:
    sys::CReadWriteLock _lock;
    uint32_t _rack_number; /** ʵ�ʻ��ܸ��� */
    CRack** _rack_array;   /** ��Ż���ָ������� */
};

MY_NAMESPACE_END
#endif // CLUSTER_RACK_TABLE_H

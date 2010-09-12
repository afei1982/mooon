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
#ifndef CLUSTER_IDC_TABLE_H
#define CLUSTER_IDC_TABLE_H
#include <stdexcept>
#include "cluster_util/idc.h"
#include "sys/read_write_lock.h"
MY_NAMESPACE_BEGIN

/** IDC�������͹�������IDC
  * �̰߳�ȫ��
  */
class CIDCTable
{
public:
    CIDCTable();
    ~CIDCTable();
    
    /** �ж�ָ����ID��IDC�Ƿ����
      * @return: ���IDC���ڷ���true�����򷵻�false
      */
    bool idc_exist(uint32_t idc_id);

    /** ����IDC ID��IP����һ��IDC
      * @exception: ���idc_id����IDC_ID_MAX�����׳�range_error�쳣
      * @return: �ɹ�����ָ��CIDC��ָ�룬�������IDC�Ѿ������򷵻�NULL
      */
    CIDC* add_idc(uint32_t idc_id);
    
    /** ɾ��IDC
      * @exception: ���idc_id����IDC_ID_MAX�����׳�range_error�쳣
      */
    void del_idc(CIDC* idc);

private:
    sys::CReadWriteLock _lock;
    uint32_t _idc_number; /** ʵ��IDC���� */
    CIDC** _idc_array;    /** ���IDCָ������� */
};

MY_NAMESPACE_END
#endif // CLUSTER_IDC_TABLE_H

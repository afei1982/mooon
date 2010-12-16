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
 * Author: JianYi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef SCHED_SESSION_H
#define SCHED_SESSION_H
#include "scheduler/thread_broker.h"

#define SESSION_NUMBER_MAX 100 /** ����service�ɴ�����session������ */

MOOON_NAMESPACE_BEGIN

/** ÿ��service����һ��ΨһID��ֵ��1��ʼ����������SERVICE_NUMBER_MAX 
  * ���service��IDΪ0�����ʾ�ⲻ��һ��service��
  * service���Ƿ��̰߳�ȫ�ģ�Ҳ���������ܱ������ͬ�̵߳��ȣ�����
  * service�ڵ�session�����̰߳�ȫ�ġ�
  */
class ISession
{
public:	
    /** ���������������������α������澯 */
    virtual ~ISession() {}
    
	virtual uint32_t get_id() const = 0;
	virtual uint16_t get_sevice_id() const = 0;
	virtual util::TReturnResult handle(IThreadBroker *thread_broker, TMessage* message) = 0;
};

MOOON_NAMESPACE_END
#endif // SCHED_SESSION_H

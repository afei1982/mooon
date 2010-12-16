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
#ifndef SCHED_SERVICE_H
#define SCHED_SERVICE_H
#include "scheduler/session.h"

#define SERVICE_NUMBER_MAX 100 /** ����Scheduler�ɵ��ȵ����service���� */

MOOON_NAMESPACE_BEGIN

/** ÿ��service����һ��ΨһID��ֵ��1��ʼ����������SERVICE_NUMBER_MAX 
  * ���service��IDΪ0�����ʾ�ⲻ��һ��service��
  * service���Ƿ��̰߳�ȫ�ģ�Ҳ���������ܱ������ͬ�̵߳��ȣ�����
  * service�ڵ�session�����̰߳�ȫ�ġ�
  */
class IService
{
public:
    /** ���������������������α������澯 */
    virtual ~IService() {}
    
	virtual uint16_t get_id() const = 0;
	virtual uint32_t get_session_number() const = 0;
	virtual util::TReturnResult handle(IThreadBroker *thread_broker, TMessage* message) = 0;
	virtual void destroy_session(ISession* session) = 0;
};

MOOON_NAMESPACE_END
#endif // SCHED_SERVICE_H

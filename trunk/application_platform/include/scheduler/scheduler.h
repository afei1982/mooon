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
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "scheduler/object.h"
#include "scheduler/message.h"
#include "scheduler/service.h"
#include "scheduler/sched_config.h"
MOOON_NAMESPACE_BEGIN

/** Service��Objectֻ��ע�ᣬ����ע��������ֻ��������ʱ�����߳�ע�ᣬ����������Service��Object������Ч�ģ�
  * ��Session�Ƕ�̬�����ģ�������������ͬһ���̴߳��������٣������������̰߳�ȫ�ġ�
  */
class IServiceRegister
{
public:
    /** ���������������������α������澯 */
    virtual ~IServiceRegister() {}
    
	virtual bool register_object(IObject* object) = 0;
	virtual bool register_service(IService* service) = 0;	
};

class IScheduler
{
public:
    /** ���������������������α������澯 */
    virtual ~IScheduler() {}

	virtual IService* get_service(uint16_t service_id) const = 0;
	virtual IServiceRegister* get_service_register() const = 0;
    virtual bool push_message(TMessage* message) = 0;	
};

extern IScheduler* get_scheduler();
extern bool load_scheduler();
extern void unload_scheduler();

MOOON_NAMESPACE_END
#endif // SCHEDULER_H

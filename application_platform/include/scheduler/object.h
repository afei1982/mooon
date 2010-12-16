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
#ifndef SCHED_OBJECT_H
#define SCHED_OBJECT_H
#include "scheduler/thread_broker.h"

#define OBJECT_NUMBER_MAX 100 /** ����Scheduler�ɵ��ȵ����service���� */

MOOON_NAMESPACE_BEGIN

/** ������service������service����0����session���Բ���0�����session��Ϊ0��
  * ��˵�������̰߳�ȫ�ģ�����Ϊ���̰߳�ȫ
  */
class IObject
{
public:	
    /** ���������������������α������澯 */
    virtual ~IObject() {}
    
	virtual uint32_t get_id() const = 0;
	virtual util::TReturnResult handle(IThreadBroker *thread_broker, TMessage* message) = 0;
};

MOOON_NAMESPACE_END
#endif // SCHED_OBJECT_H

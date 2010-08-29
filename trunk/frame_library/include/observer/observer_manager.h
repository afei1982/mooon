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
#ifndef OBSERVER_MANAGER_H
#define OBSERVER_MANAGER_H
#include "util/log.h"
#include "observer/observable.h"
#include "observer/data_reporter.h"
MY_NAMESPACE_BEGIN

/** �۲��߹�����
  * �ṩע�ᱻ�۲��߽ӿ�
  */
class IObserverManager
{
public:
    virtual ~IObserverManager() {}
	// ����Ӧ����ϵͳ����ʱ�������߳��о�������е�ע�ᣬ���й����в�Ӧ����ע�ᣬ��������̰߳�ȫ����
	virtual void register_observee(IObservable* observee) = 0;
	// ֧��ע�����ܣ�����ԭ��: һ�ǻ����޴����󣬶��ǵ�����Ҫ����
	// virtual void deregiester_observee(IObservable* observee) = 0;
};

extern IObserverManager* create_observer_manager(IDataReporter* data_reporter, uint16_t report_frequency_seconds);
extern void destroy_observer_manager(IObserverManager* observer_manager);

MY_NAMESPACE_END
#endif // OBSERVER_MANAGER_H

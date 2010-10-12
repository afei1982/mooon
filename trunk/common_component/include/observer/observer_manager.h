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
#include <observer/observable.h>
#include <observer/data_reporter.h>
MOOON_NAMESPACE_BEGIN

/***
  * �۲��߹�����
  */
class IObserverManager
{
public:
    /** ����������������ΪӦ���������澯 */
    virtual ~IObserverManager() {}

	/***
      * ��Observerע��ɹ۲���
      * @observee: ��ע��Ŀɹ۲���
      */
	virtual void register_observee(IObservable* observee) = 0;
};

//////////////////////////////////////////////////////////////////////////
// ȫ��C��������

/** ���ٹ۲��߹����� */
extern "C" void destroy_observer_manager();

/** ��ù۲��߹����� */
extern "C" IObserverManager* get_observer_manager();

/***
  * �����۲��߹�����
  * @logger: ��־��
  * @data_reporter: �����ϱ���
  * @report_frequency_seconds: �����ϱ�Ƶ��(��λ: ��)
  */
extern "C" IObserverManager* create_observer_manager(sys::ILogger* logger, IDataReporter* data_reporter, uint16_t report_frequency_seconds);

MOOON_NAMESPACE_END
#endif // OBSERVER_MANAGER_H

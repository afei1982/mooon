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
 * Author: eyjian@qq.com or eyjian@gmail.com
 */
#ifndef CONFIG_OBSERVER_H
#define CONFIG_OBSERVER_H
#include <util/util_config.h>
MOOON_NAMESPACE_BEGIN

/***
  * ���ù۲��ߣ�ִ�о�������ø���
  */
class CALLBACK_INTERFACE IConfigObserver
{
public:
    /** �����������������α��������� */
    virtual ~IConfigObserver() {}

    /***
      * ��������Ҫ����ʱ�ص�Щ����
      * @config_name: ��Ҫ���µ���������
      * @return: ������³ɹ��򷵻�true�����򷵻�false
      */
    virtual bool on_config_updated(const char* config_name) = 0;
};

MOOON_NAMESPACE_END
#endif // CONFIG_OBSERVER_H

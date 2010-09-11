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
 * Author: JianYi, eyjian@qq.com
 */
#ifndef SYS_CONFIG_FILE_H
#define SYS_CONFIG_FILE_H
#include <vector>
#include "sys/sys_config.h"
SYS_NAMESPACE_BEGIN

/**
  * ��ȡ���ýӿ�
  * <A>
  *   <B>
  *     <C age="32"/>
  *   </B>
  * </A>
  *
  * path: /A/B/C
  * name: age
  * value: 32
  */
class IConfigReader
{
public:
    /** ���������������������α������澯 */
    virtual ~IConfigReader() {}
    virtual bool path_exist(const std::string& path) = 0;
	virtual bool name_exist(const std::string& path, const std::string& name) = 0;

    virtual bool get_string_value(const std::string& path, const std::string& name, std::string& value) = 0;
    virtual bool get_int16_value(const std::string& path, const std::string& name, int16_t& value) = 0;
    virtual bool get_int32_value(const std::string& path, const std::string& name, int32_t& value) = 0;
    virtual bool get_int64_value(const std::string& path, const std::string& name, int64_t& value) = 0;
    virtual bool get_uint16_value(const std::string& path, const std::string& name, uint16_t& value) = 0;
    virtual bool get_uint32_value(const std::string& path, const std::string& name, uint32_t& value) = 0;
    virtual bool get_uint64_value(const std::string& path, const std::string& name, uint64_t& value) = 0;

    virtual bool get_string_values(const std::string& path, const std::string& name, std::vector<std::string>& values) = 0;
    virtual bool get_int16_values(const std::string& path, const std::string& name, std::vector<int16_t>& values) = 0;
    virtual bool get_int32_values(const std::string& path, const std::string& name, std::vector<int32_t>& values) = 0;
    virtual bool get_int64_values(const std::string& path, const std::string& name, std::vector<int64_t>& values) = 0;
    virtual bool get_uint16_values(const std::string& path, const std::string& name, std::vector<uint16_t>& values) = 0;
    virtual bool get_uint32_values(const std::string& path, const std::string& name, std::vector<uint32_t>& values) = 0;
    virtual bool get_uint64_values(const std::string& path, const std::string& name, std::vector<uint64_t>& values) = 0;

    virtual bool get_sub_config(const std::string& path, std::vector<IConfigReader*>& sub_config_array) = 0;
};

class IConfigFile
{
public:
    /** ���������������������α������澯 */
    virtual ~IConfigFile() {}

    /** �������ļ�
      * @return: ����򿪳ɹ�����true�����򷵻�false
      * @exception: ���쳣�׳�
      */
	virtual bool open(const std::string& xmlfile) = 0;

    /** �رմ򿪵������ļ� */
    virtual void close() = 0;

    /** ��ȡһ��IConfigReader������ע��get_config_reader��release_config_reader�ĵ��ñ���ɶԣ�������ڴ�й©
      * @return: ����ɹ�����IConfigReader���͵�ָ�룬���򷵻�NULL
      */
    virtual IConfigReader* get_config_reader() = 0;
    virtual void release_config_reader(IConfigReader* config_reader) = 0;

    virtual int get_error_row() const = 0;
    virtual int get_error_col() const = 0;
    virtual std::string get_error_message() const = 0;
};

extern IConfigFile* g_config;

SYS_NAMESPACE_END
#endif // SYS_CONFIG_FILE_H

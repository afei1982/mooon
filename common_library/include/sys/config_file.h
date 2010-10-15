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

    /** �ж�ָ��·���Ƿ���� */
    virtual bool path_exist(const std::string& path) = 0;

    /** �ж�ָ��·���µ�ָ���������Ƿ���� */
	virtual bool name_exist(const std::string& path, const std::string& name) = 0;

    /***
      * �õ������ַ������͵�����ֵ
      * @path: ����·��
      * @name: ��������
      * @value: ���ڴ洢�õ�������ֵ
      * @return: ����ɹ��õ��ַ������͵�����ֵ���򷵻�true�����򷵻�false
      */
    virtual bool get_bool_value(const std::string& path, const std::string& name, bool& value) = 0;
    virtual bool get_string_value(const std::string& path, const std::string& name, std::string& value) = 0;
    virtual bool get_int16_value(const std::string& path, const std::string& name, int16_t& value) = 0;
    virtual bool get_int32_value(const std::string& path, const std::string& name, int32_t& value) = 0;
    virtual bool get_int64_value(const std::string& path, const std::string& name, int64_t& value) = 0;
    virtual bool get_uint16_value(const std::string& path, const std::string& name, uint16_t& value) = 0;
    virtual bool get_uint32_value(const std::string& path, const std::string& name, uint32_t& value) = 0;
    virtual bool get_uint64_value(const std::string& path, const std::string& name, uint64_t& value) = 0;

    /***
      * �õ�����ַ������͵�����ֵ
      * @path: ����·��
      * @name: ��������
      * @value: ���ڴ洢�õ�������ֵ������
      * @return: ����ɹ��õ��ַ������͵�����ֵ���򷵻�true�����򷵻�false
      */
    virtual bool get_string_values(const std::string& path, const std::string& name, std::vector<std::string>& values) = 0;
    virtual bool get_int16_values(const std::string& path, const std::string& name, std::vector<int16_t>& values) = 0;
    virtual bool get_int32_values(const std::string& path, const std::string& name, std::vector<int32_t>& values) = 0;
    virtual bool get_int64_values(const std::string& path, const std::string& name, std::vector<int64_t>& values) = 0;
    virtual bool get_uint16_values(const std::string& path, const std::string& name, std::vector<uint16_t>& values) = 0;
    virtual bool get_uint32_values(const std::string& path, const std::string& name, std::vector<uint32_t>& values) = 0;
    virtual bool get_uint64_values(const std::string& path, const std::string& name, std::vector<uint64_t>& values) = 0;

    /***
      * �õ������ö�ȡ��
      * @path: ����·��
      * @sub_config_array: ���ڴ洢�����ö�ȡ��������
      * @return: ����ɹ��õ������ö�ȡ�����򷵻�true�����򷵻�false
      */
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
    virtual void free_config_reader(IConfigReader* config_reader) = 0;

    /** �õ�����������к� */
    virtual int get_error_row() const = 0;

    /** �õ�����������к� */
    virtual int get_error_col() const = 0;

    /** �õ�������Ϣ */
    virtual std::string get_error_message() const = 0;
};

/***
  * ConfigReader�����࣬�����Զ��ͷ��Ѿ���ȡ��ConfigReader  
  */
class ConfigReaderHelper
{
public:
    /** ����һ��ConfigReader��������� */
    ConfigReaderHelper(IConfigFile* config_file, IConfigReader*& config_reader)
        :_config_file(config_file)
        ,_config_reader(config_reader)
    {
    }
    
    /** �����Զ��ͷ��Ѿ��õ���ConfigReader */
    ~ConfigReaderHelper()
    {
        if ((_config_file != NULL) && (_config_reader != NULL))
        {
            _config_file->free_config_reader(_config_reader);
            _config_reader = NULL;
        }
    }

private:
    IConfigFile* _config_file;
    IConfigReader*& _config_reader;
};

extern IConfigFile* g_config;

SYS_NAMESPACE_END
#endif // SYS_CONFIG_FILE_H

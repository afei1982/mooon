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
 * Author: jian yi, eyjian@qq.com
 */
#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <math.h>
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

class CStringUtil
{
public:
	/** ɾ���ַ�����ָ���ַ���ʼ������
	  * @source: ��Ҫ������ַ���
	  * @c: �ָ��ַ�
	  * @example:  ���strΪ��/usr/local/jian/bin/������cΪ��/���������str��ɡ�/usr/local/jian/bin��
	  */
	static void remove_last(std::string& source, char c);

	/** ɾ���ַ�����ָ���ַ�����ʼ������
	  * @source: ��Ҫ������ַ���
	  * @sep: �ָ��ַ���
	  * @example: ���strΪ��/usr/local/jian/bin/tt������sepΪ��/bin/���������str��ɡ�/usr/local/jian��
	  */
	static void remove_last(std::string& source, const std::string& sep);

	/**
	  * �ַ���ת������������
	  */
	
	/** ���ַ���ת����16λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
	static bool string2int16(const char* source, int16_t& result);

	/** ���ַ���ת����32λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
	static bool string2int32(const char* source, int32_t& result);

	/** ���ַ���ת����64λ���з�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
	static bool string2int64(const char* source, int64_t& result);

	/** ���ַ���ת����16λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
	static bool string2uint16(const char* source, uint16_t& result);

	/** ���ַ���ת����32λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
	static bool string2uint32(const char* source, uint32_t& result);

	/** ���ַ���ת����64λ���޷�������
	  * @source: ��ת�����������ַ���
	  * @result: ת���������
	  * @return: ���ת���ɹ�����true�����򷵻�false
	  */
	static bool string2uint64(const char* source, uint64_t& result);

    static std::string int16_tostring(int16_t source);
    static std::string int32_tostring(int32_t source);
    static std::string int64_tostring(int64_t source);
    static std::string uint16_tostring(uint16_t source);
    static std::string uint32_tostring(uint32_t source);
    static std::string uint64_tostring(uint64_t source);
    
    /** �����ո񲿷�
      */
    static const char* skip_spaces(const char* buffer);

    static uint32_t hash(const char *str, int len);
};

UTIL_NAMESPACE_END
#endif // STRING_UTIL_H

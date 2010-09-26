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
#ifndef DATETIME_UTIL_H
#define DATETIME_UTIL_H
#include <time.h>
#include "sys/sys_util.h"
SYS_NAMESPACE_BEGIN

/** ����ʱ�乤����
  */
class CDatetimeUtil
{
public:
    /** �ж�ָ������Ƿ�Ϊ���� */
    static bool is_leap_year(int year);
    
    /** �õ���ǰ���ں�ʱ�䣬���ظ�ʽΪ: YYYY-MM-DD HH:SS:MM
      * @datetime_buffer: �����洢��ǰ���ں�ʱ��Ļ�����
      * @datetime_buffer_size: datetime_buffer���ֽڴ�С����Ӧ����С��YYYY-MM-DD HH:SS:MM��
      */
    static void get_current_datetime(char* datetime_buffer, size_t datetime_buffer_size);
    static std::string get_current_datetime();    

    /** �õ���ǰ���ڣ����ظ�ʽΪ: YYYY-MM-DD
      * @date_buffer: �����洢��ǰ���ڵĻ�����
      * @date_buffer_size: date_buffer���ֽڴ�С����Ӧ����С��YYYY-MM-DD��
      */
    static void get_current_date(char* date_buffer, size_t date_buffer_size);
    static std::string get_current_date();

    /** �õ���ǰʱ�䣬���ظ�ʽΪ: HH:SS:MM
      * @time_buffer: �����洢��ǰʱ��Ļ�����
      * @time_buffer_size: time_buffer���ֽڴ�С����Ӧ����С��HH:SS:MM��
      */
    static void get_current_time(char* time_buffer, size_t time_buffer_size);
    static std::string get_current_time();

    /** �õ���ǰ���ں�ʱ��ṹ
      * @current_datetime_struct: ָ��ǰ���ں�ʱ��ṹ��ָ��
      */
    static void get_current_datetime_struct(struct tm* current_datetime_struct);

    /** ���ں�ʱ�� */
    static void to_current_datetime(struct tm* current_datetime_struct, char* datetime_buffer, size_t datetime_buffer_size);
    static std::string to_current_datetime(struct tm* current_datetime_struct);

    /** ������ */
    static void to_current_date(struct tm* current_datetime_struct, char* date_buffer, size_t date_buffer_size);
    static std::string to_current_date(struct tm* current_datetime_struct);

    /** ��ʱ�� */
    static void to_current_time(struct tm* current_datetime_struct, char* time_buffer, size_t time_buffer_size);
    static std::string to_current_time(struct tm* current_datetime_struct);

    /** ����� */
    static void to_current_year(struct tm* current_datetime_struct, char* year_buffer, size_t year_buffer_size);
    static std::string to_current_year(struct tm* current_datetime_struct);

    /** ���·� */
    static void to_current_month(struct tm* current_datetime_struct, char* month_buffer, size_t month_buffer_size);
    static std::string to_current_month(struct tm* current_datetime_struct);

    /** ���� */
    static void to_current_day(struct tm* current_datetime_struct, char* day_buffer, size_t day_buffer_size);
    static std::string to_current_day(struct tm* current_datetime_struct);

    /** ��Сʱ */
    static void to_current_hour(struct tm* current_datetime_struct, char* hour_buffer, size_t hour_buffer_size);
    static std::string to_current_hour(struct tm* current_datetime_struct);

    /** ������ */
    static void to_current_minite(struct tm* current_datetime_struct, char* minite_buffer, size_t minite_buffer_size);
    static std::string to_current_minite(struct tm* current_datetime_struct);

    /** ������ */
    static void to_current_second(struct tm* current_datetime_struct, char* second_buffer, size_t second_buffer_size);
    static std::string to_current_second(struct tm* current_datetime_struct);

    /***
      * ��һ���ַ���ת��������ʱ���ʽ
      * @str: ���ϡ�YYYY-MM-DD HH:MM:SS����ʽ������ʱ��
      * @datetime_struct: �洢ת���������ʱ��
      * @return: ת���ɹ�����true�����򷵻�false
      */
    static bool datetime_struct_from_string(const char* str, struct tm* datetime_struct);
    static bool datetime_struct_from_string(const char* str, time_t* datetime);
};

SYS_NAMESPACE_END
#endif // DATETIME_UTIL_H

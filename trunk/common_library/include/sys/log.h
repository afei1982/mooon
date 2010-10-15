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
#ifndef SYS_LOG_H
#define SYS_LOG_H
#include "sys/sys_config.h"
SYS_NAMESPACE_BEGIN

/** ��Ҫ�޸�����ĳ���ֵ����Ӧ��ͨ����Ӧ�ķ���ȥ�޸�
  * ��Щ����ֵ��Ҫ�Ƿ����ģ�鹲���ʷ����������ͷ�ļ�����
  */
enum
{
    LOG_LINE_SIZE_MIN              = 256,        /** ��־����С���� */
    LOG_LINE_SIZE_MAX              = 32768,      /** ��־����󳤶�(32K) */
    DEFAULT_LOG_FILE_SIZE          = 104857600,  /** Ĭ�ϵĵ�����־�ļ���С(100MB) */
    DEFAULT_LOG_FILE_BACKUP_NUMBER = 10          /** Ĭ�ϵ���־�ļ����ݸ��� */
};

/** ������־���� */
typedef enum
{
    LOG_LEVEL_DETAIL = 0,
    LOG_LEVEL_DEBUG  = 1,
    LOG_LEVEL_INFO   = 2,
    LOG_LEVEL_WARN   = 3,
    LOG_LEVEL_ERROR  = 4,
    LOG_LEVEL_FATAL  = 5,    
    LOG_LEVEL_STATE  = 6,  /** �����״̬���� */
    LOG_LEVEL_TRACE  = 7
}log_level_t;

/** ͨ����־�������õ���־���� */
extern log_level_t get_log_level(const char* level_name);
/** ͨ����־����õ���־���������������������־�����򷵻�NULL */
extern const char* get_log_level_name(log_level_t log_level);

/**
  * ��־���ӿڣ��ṩ������д��־����
  */
class ILogger
{
public:
    /** ���������������������α������澯 */
    virtual ~ILogger() {}
        
    /** �Ƿ�����ͬʱ�ڱ�׼����ϴ�ӡ��־ */
    virtual void enable_screen(bool enabled) = 0;
    /** �Ƿ������������־����������־����ͨ�������� */
    virtual void enable_bin_log(bool enabled) = 0;
    /** �Ƿ����������־��������־����ͨ�������� */
    virtual void enable_trace_log(bool enabled) = 0;
    /** �Ƿ��Զ���һ�к���ӽ�β�ĵ�ţ��������Ѿ��е�Ż򻻷������򲻻������ */
    virtual void enable_auto_adddot(bool enabled) = 0;
    /** �Ƿ��Զ���ӻ��з�������Ѿ��л��з����򲻻����Զ���ӻ��з� */
    virtual void enable_auto_newline(bool enabled) = 0;    
    /** ������־���𣬸�����־������ͨ���������� */
    virtual void set_log_level(log_level_t log_level) = 0;
    /** ���õ����ļ���������С */
    virtual void set_single_filesize(uint32_t filesize) = 0;
    /** ������־�ļ����ݸ�������������д����־�ļ� */
    virtual void set_backup_number(uint16_t backup_number) = 0;

    /** �Ƿ������������־ */
    virtual bool enabled_bin() = 0;
    /** �Ƿ�����Detail������־ */
    virtual bool enabled_detail() = 0;
    /** �Ƿ�����Debug������־ */
    virtual bool enabled_debug() = 0;
    /** �Ƿ�����Info������־ */
    virtual bool enabled_info() = 0;
    /** �Ƿ�����Warn������־ */
    virtual bool enabled_warn() = 0;
    /** �Ƿ�����Error������־ */
    virtual bool enabled_error() = 0;
    /** �Ƿ�����Fatal������־ */
    virtual bool enabled_fatal() = 0;
    /** �Ƿ��������״̬��־ */
    virtual bool enabled_state() = 0;
    /** �Ƿ�����Trace������־ */
    virtual bool enabled_trace() = 0;

    virtual void log_detail(const char* format, ...) = 0;
    virtual void log_debug(const char* format, ...)  = 0;
    virtual void log_info(const char* format, ...)   = 0;
    virtual void log_warn(const char* format, ...)   = 0;
    virtual void log_error(const char* format, ...)  = 0;
    virtual void log_fatal(const char* format, ...)  = 0;
    virtual void log_state(const char* format, ...)  = 0;
    virtual void log_trace(const char* format, ...)  = 0;

    /** д��������־ */
    virtual void bin_log(const char* log, uint16_t size) = 0;
};

//////////////////////////////////////////////////////////////////////////
// ��־�꣬�����¼��־
extern ILogger* g_logger; // ֻ�����������Ƕ��壬���ܸ�ֵŶ��

#define __MYLOG_DETAIL(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_detail()) \
		logger->log_detail(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_DEBUG(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_debug()) \
		logger->log_debug(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_INFO(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_info()) \
		logger->log_info(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_WARN(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_warn()) \
		logger->log_warn(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_ERROR(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_error()) \
		logger->log_error(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_FATAL(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_fatal()) \
		logger->log_fatal(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_STATE(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_state()) \
		logger->log_state(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_TRACE(logger, format, ...) \
do { \
	if (NULL == logger) \
		printf(format, ##__VA_ARGS__); \
	else if (logger->enabled_trace()) \
		logger->log_trace(format, ##__VA_ARGS__); \
} while(false)

#define __MYLOG_BIN(logger, log, size) \
do { \
    if ((logger != NULL) && logger->enabled_bin()) \
        logger->bin_log(log, size); \
} while(false)

#define MYLOG_BIN(log, size)         __MYLOG_BIN(sys::g_logger, log, size)
#define MYLOG_TRACE(format, ...)     __MYLOG_TRACE(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_STATE(format, ...)     __MYLOG_STATE(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_FATAL(format, ...)     __MYLOG_FATAL(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_ERROR(format, ...)     __MYLOG_ERROR(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_WARN(format, ...)      __MYLOG_WARN(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_INFO(format, ...)      __MYLOG_INFO(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_DEBUG(format, ...)     __MYLOG_DEBUG(sys::g_logger, format, ##__VA_ARGS__)
#define MYLOG_DETAIL(format, ...)    __MYLOG_DETAIL(sys::g_logger, format, ##__VA_ARGS__)

SYS_NAMESPACE_END
#endif // SYS_LOG_H

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
#ifndef SYS_LOGGER_H
#define SYS_LOGGER_H
#include "sys/log.h"
#include "sys/thread.h"
#include "util/array_queue.h"
SYS_NAMESPACE_BEGIN

class CLogger: public sys::ILogger
{
public:
    /** ����һ��Logger����
      * @log_line_size: Ĭ�������ÿ����־����󳤶ȣ�����ᳬ��LOG_LINE_SIZE_MAX������ض�   
      */
    CLogger(uint16_t log_line_size=512);
    virtual ~CLogger();
    
    void destroy();

    /** ��־����ʼ��
      * @log_path: ��־�ļ����λ��
      * @log_filename: ��־�ļ�����һ����·������ 
      * @log_queue_size: ������־���м��������ܴ�С
      * @log_queue_number: ��־���и���
      * @thread_orderly: ͬһ���̵߳���־�Ƿ�ʱ��˳��д
      * @exception: ��������׳�CSyscallException�쳣
      */
    void create(const char* log_path, const char* log_filename, uint32_t log_queue_size=10000, uint16_t log_queue_number=1, bool thread_orderly=true);

    /** �Ƿ�����ͬʱ�ڱ�׼����ϴ�ӡ��־ */
    virtual void enable_screen(bool enabled);
    /** �Ƿ������������־����������־����ͨ�������� */
    virtual void enable_bin_log(bool enabled);
    /** �Ƿ����������־��������־����ͨ�������� */
    virtual void enable_trace_log(bool enabled);
    /** �Ƿ��Զ���һ�к���ӽ�β�ĵ�ţ��������Ѿ��е�ţ��򲻻������ */
    virtual void enable_auto_adddot(bool enabled);
    /** �Ƿ��Զ���ӻ��з�������Ѿ��л��з����򲻻����Զ���ӻ��з� */
    virtual void enable_auto_newline(bool enabled);    
    /** ������־���𣬸�����־������ͨ���������� */
    virtual void set_log_level(log_level_t log_level);
    /** ���õ����ļ���������С */
    virtual void set_single_filesize(uint32_t filesize);
    virtual void set_backup_number(uint16_t backup_number);

    virtual bool enabled_bin();

    virtual bool enabled_detail();
    virtual bool enabled_debug();
    virtual bool enabled_info();
    virtual bool enabled_warn();
    virtual bool enabled_error();
    virtual bool enabled_fatal();
    virtual bool enabled_state();
    virtual bool enabled_trace();

    virtual void log_detail(const char* format, ...);
    virtual void log_debug(const char* format, ...);
    virtual void log_info(const char* format, ...);
    virtual void log_warn(const char* format, ...);
    virtual void log_error(const char* format, ...);
    virtual void log_fatal(const char* format, ...);
    virtual void log_state(const char* format, ...);
    virtual void log_trace(const char* format, ...);

    virtual void bin_log(const char* log, uint16_t size);

private:
    void do_log(log_level_t log_level, const char* format, va_list& args);
    
private:    
    bool _auto_adddot;
    bool _auto_newline;
    uint16_t _log_line_size;
    log_level_t _log_level;
    bool _bin_log_enabled;
    bool _trace_log_enabled;

private: // �ڲ���
    typedef struct
    {
        uint16_t length; // ��־���ݳ���
        char content[4]; // ��־����
    }log_message_t;

    class CLogThread: public CThread
    {
    public:
        CLogThread(const char* log_path, const char* log_filename, uint32_t queue_size, uint16_t queue_number, bool thread_orderly);
        ~CLogThread();

        void push_log(const log_message_t* log);
        void enable_screen(bool enabled);
        void set_single_filesize(uint32_t filesize);
        void set_backup_number(uint16_t backup_number);

    private:
        virtual void run();
        virtual bool before_start();

    private:
        bool write_log();
        int choose_queue(); 
        void close_logfile();
        void create_logfile(bool truncate);
        bool need_roll_file() const { return _current_bytes > _max_bytes; }
        void roll_file();
        bool need_create_file() const;

    private:    
        int _log_fd;
        atomic_t _log_number;
        volatile uint32_t _queue_index;             /** ��־�������� */
        uint16_t _queue_number;                     /** ��־���и��� */
        util::CArrayQueue<const log_message_t*>** _queue_array;    /** ��־�������� */
        CLock* _lock_array;        

    private:        
        bool _screen_enabled; 
        bool _thread_orderly;
        uint32_t _max_bytes;
        uint32_t _current_bytes; 
        uint16_t _backup_number;
        char _log_path[PATH_MAX];
        char _log_filename[FILENAME_MAX];
    }*_log_thread;
};

SYS_NAMESPACE_END
#endif // SYS_LOGGER_H

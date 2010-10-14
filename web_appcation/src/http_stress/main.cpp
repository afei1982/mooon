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
#include <sys/logger.h>
#include <sys/sys_util.h>
#include <net/epollable.h>
#include <util/string_util.h>
#include <dispatcher/dispatcher.h>
#include <plugin/plugin_tinyxml/plugin_tinyxml.h>
#include "counter.h"
#include "http_reply_handler.h"
#define DEFAULT_MESSAGE_NUMBER 100

// argv[1]: ��Ϣ����
int main(int argc, char* argv[])
{
    // ������־��
    sys::CLogger* logger = new sys::CLogger;

    try
    {    
        logger->create(".", "stress.log");
        printf("log file is stress.log at current directory.\n");
    }
    catch (sys::CSyscallException& ex)
    {
        fprintf(stderr, "Created logger failed: %s.\n", sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
        exit(1);
    }

    // �������ļ�
    sys::IConfigFile* config_file = plugin::create_config_file();
    if (!config_file->open("stress.xml"))
    {
        fprintf(stderr, "Open stress.xml failed for %s.\n", config_file->get_error_message().c_str());
        exit(1);
    }

    // ��־����
    sys::IConfigReader* config_reader = config_file->get_config_reader();

    // ��־����
    std::string level_name = "debug";
    config_reader->get_string_value("/stress/log", "level", level_name);

    // ����ȫ����־��
    sys::g_logger = logger;
    sys::g_logger->enable_screen(true);
    sys::g_logger->set_log_level(sys::get_log_level(level_name.c_str()));   

    // �߳���
    uint16_t thread_number = 1;
    config_reader->get_uint16_value("/stress/thread", "number", thread_number);

    // ������    
    uint32_t request_number = 1000;
    config_reader->get_uint32_value("/stress/request", "number", request_number);
    mooon::CCounter::set_request_number(request_number);

    // ����    
    std::string domain_name;
    config_reader->get_string_value("/stress/request", "domain_name", domain_name);
    mooon::CCounter::set_domain_name(domain_name);

    // URLs    
    config_reader->get_string_values("/stress/urls/url", "value", mooon::CCounter::get_urls());
    
    // ����httpӦ����������
    mooon::CHttpReplyHandlerFactory* http_reply_handler_factory = new mooon::CHttpReplyHandlerFactory;

    // ������Ϣ�ַ���
    mooon::IDispatcher* dispatcher = mooon::create_dispatcher(logger);
    if (!dispatcher->open("node.table", 100, thread_number, 10, http_reply_handler_factory))
    {
        fprintf(stderr, "Open dispatcher failed.\n");
        exit(1);
    }
    
    time_t begin_time = time(NULL);

    // ������Ϣ��
    uint32_t number = 0; // ����û��
    uint16_t sender_number = dispatcher->get_managed_sender_number();
    for (uint16_t i=0; i<sender_number; ++i)
    {        
        mooon::CCounter::send_http_request(i+1, number);
    }                 
    
    // �ȵ����
    uint32_t last_send_request_number = 0;    // ��һ�η��͵��������
    uint32_t current_send_request_number = 0; // ��ǰ���͵��������
    uint32_t total_request_number = sender_number * mooon::CCounter::get_request_number(); // ��Ҫ���͵���������
    printf("tatal request number: %u\n", total_request_number);

    while (true)
    {
        current_send_request_number = mooon::CCounter::get_send_request_number();
        if (current_send_request_number >= total_request_number) break;
        if (mooon::CCounter::wait_finish()) continue;                
                
        printf("%d --> %d\n", current_send_request_number, current_send_request_number-last_send_request_number);
        last_send_request_number = current_send_request_number;
    }
    
    time_t end_time = time(NULL);
    time_t interval = (end_time == begin_time)? 1: (end_time-begin_time);
    dispatcher->close();
    
    printf("time: %u seconds\n", (uint32_t)interval);
    printf("total number: %d\n", total_request_number);
    printf("failure number: %d\n", mooon::CCounter::get_failure_request_number());
    printf("success number: %d\n", mooon::CCounter::get_success_request_number());
    printf("percent number: %ld\n", total_request_number/interval);
    printf("bytes sent: %ld\n", net::get_send_buffer_bytes());
    printf("bytes received: %ld\n", net::get_recv_buffer_bytes());
    
    logger->destroy();
    mooon::destroy_dispatcher();    
    return 0;
}

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
#include <net/net_util.h>
#include <sys/close_helper.h>
#include <util/string_util.h>
#include <util/integer_util.h>
#include "managed_sender_table.h"
#include "default_reply_handler.h"
MOOON_NAMESPACE_BEGIN

CManagedSenderTable::~CManagedSenderTable()
{
    clear_sender();
    delete []_sender_table;
}

CManagedSenderTable::CManagedSenderTable(uint32_t queue_max, CSendThreadPool* thread_pool)
    :CSenderTable(queue_max, thread_pool)
    ,_managed_sender_number(0)
    ,_max_sender_table_size(UINT16_MAX)    
{
    _sender_table = new CManagedSender*[_max_sender_table_size];
    for (int i=0; i<_max_sender_table_size; ++i)
        _sender_table[i] = NULL;
}

uint16_t CManagedSenderTable::get_sender_number() const
{
    return _managed_sender_number;
}

// �ļ���ʽ: 
// ��һ�и�ʽ: �������͵ķַ������������Ϊ0�����ұ������Ч��������ͬ
// �ǵ�һ�и�ʽ: ID\tIP\tPORT
// ����IP��ΪIPV4��IPV6
bool CManagedSenderTable::load(const char* dispatch_table)
{
    if (NULL == dispatch_table)
    {
        DISPATCHER_LOG_ERROR("Loaded dispach table failed without filename.\n");
        return false;        
    }

    FILE* fp = fopen(dispatch_table, "r");
    sys::close_helper<FILE*> ch(fp);
    if (NULL == fp)
    {
        DISPATCHER_LOG_ERROR("Loaded dispach table from %s error for %s.\n", dispatch_table, strerror(errno));
        return false;
    }
        
    const char* ip;             // IP��ַ
    int32_t port;               // Ŀ¼�˿ں�
    int32_t node_id;            // �ڵ�ID
    bool is_host_name;          // ����IP������������������
    int32_t line_number =0;     // ��ǰ�кţ����㶨λ����λ��
    uint16_t item_number = 0;         // ��ǰ�Ѿ�ȷ������Ŀ����
    uint16_t item_number_total = 0;   // ��Ŀ����
    char line[LINE_MAX];              // һ�����ݣ�������ʽӦ��ΪID\tIP\tPORT
    char ip_or_name[IP_ADDRESS_MAX];  // Ŀ��IP��ַ��������������
    char check_filed[LINE_MAX];       // У���������ж��Ƿ���һ���ֶ�
    net::CNetUtil::TStringIPArray ip_array; // ���������������õ���IP����

    while (fgets(line, sizeof(line)-1, fp))
    {
        ++line_number; 
        util::CStringUtil::trim(line);

        // ��һ�в���Ϊ�գ�Ҳ����Ϊע���У�����Ϊ������¼
        if (1 == line_number)
        {
            if (!util::CStringUtil::string2uint16(line, item_number_total))
            {
                DISPATCHER_LOG_ERROR("The first line error, can not get total number at %s.\n", dispatch_table);
                return false;
            }
            else
            {
                continue;
            }
        }

        // �������к�ע����
        if (('\0' == line[0]) || ('#' == line[0])) continue;
        
        // �õ�id��ip��port
        if (sscanf(line, "%d%s%d%s", &node_id, ip_or_name, &port, check_filed) != 3)
        {
            DISPATCHER_LOG_ERROR("Format error of dispach table at %s:%d.\n", dispatch_table, line_number);
            return false;
        }

        // ���ID�Ƿ���ȷ
        if (!util::CIntegerUtil::is_uint16(node_id))
        {
            DISPATCHER_LOG_ERROR("Invalid node ID %d from dispach table at %s:%d.\n", node_id, dispatch_table, line_number);
            return false;
        }

        // ���IP�Ƿ���ȷ
        is_host_name = !net::CNetUtil::is_valid_ip(ip_or_name);
        
        // ���˿��Ƿ���ȷ
        if (!util::CIntegerUtil::is_uint16(port))
        {
            DISPATCHER_LOG_ERROR("Invalid port %d from dispach table at %s:%d.\n", port, dispatch_table, line_number);
            return false;
        }

        // �ظ���ͻ���Ѿ����ڣ�IP�����ظ�����ID������
        if (_sender_table[node_id] != NULL)
        {
            DISPATCHER_LOG_ERROR("Duplicate ID %d from dispach table at %s:%d.\n", node_id, dispatch_table, line_number);
            return false;
        }
        
        ip = ip_or_name;
        if (is_host_name)
        {
            std::string errinfo;            
            if (!net::CNetUtil::get_ip_address(ip_or_name, ip_array, errinfo))
            {
                DISPATCHER_LOG_ERROR("Invalid hostname %s from dispach table at %s:%d.\n", ip_or_name, dispatch_table, line_number);
                return false;
            }
            
            ip = ip_array[0].c_str();
        }

        try
        {      
            IReplyHandler* reply_handler = NULL;
            CSendThreadPool* thread_pool = get_thread_pool();
            IReplyHandlerFactory* reply_handler_factory = thread_pool->get_reply_handler_factory();
            if (NULL == reply_handler_factory)
            {
                reply_handler = new CDefaultReplyHandler;
            }
            else
            {
                reply_handler = reply_handler_factory->create_reply_handler();
            }
            CManagedSender* sender = new CManagedSender(thread_pool, node_id, get_queue_max(), reply_handler);            
                       
            net::ip_address_t ip_address(ip);
            sender->set_peer_ip(ip_address);
            sender->set_peer_port((uint16_t)port);
            if (is_host_name) sender->set_host_name(ip_or_name);

            sender->inc_refcount(); // ������Ҫ�������ü���������clear_sender�м�������ü���
            _sender_table[node_id] = sender;

            sys::CLockHelper<sys::CLock> lock(_lock);
            CSendThread* thread = thread_pool->get_next_thread();
            sender->inc_refcount(); // ����Ҳ��Ҫ�������ü���������CSendThread�м�������ü���
            thread->add_sender(sender);

            // ��Ŀ������
            if (++item_number > item_number_total) break;
        }
        catch (sys::CSyscallException& ex)
        {
            DISPATCHER_LOG_ERROR("Loaded dispatch table %s:%d exception: %s.\n"
                , dispatch_table, line_number
                , sys::CSysUtil::get_error_message(ex.get_errcode()).c_str());
            return false;
        }
    }

    if (item_number != item_number_total)
    {
        DISPATCHER_LOG_ERROR("Number mismatch %u and %u at %s.\n", item_number, item_number_total, dispatch_table);
        return false;
    }

    _managed_sender_number = item_number_total;
    return true;
}

bool CManagedSenderTable::send_message(uint16_t node_id, dispach_message_t* message)
{
    CManagedSender* sender = get_sender(node_id);
    if (sender != NULL)
    {
        sender->push_message(message);
        sender->dec_refcount();
    }

    return (sender != NULL);
}

CManagedSender* CManagedSenderTable::get_sender(uint16_t node_id)
{
    sys::CLockHelper<sys::CLock> lock(_lock);
    CManagedSender* sender = _sender_table[node_id];
    if (sender != NULL) sender->inc_refcount();
    return sender;
}

void CManagedSenderTable::clear_sender()
{
    // �������ѭ��������Ϊ65535�Σ���ֻ�и��·��ͱ�ʱ�ŷ��������Զ�����Ӱ����Ժ���
    sys::CLockHelper<sys::CLock> lock(_lock);    
    for (uint16_t i=0; i<_max_sender_table_size; ++i)
    {
        if (_sender_table[i] != NULL)
        {
            _sender_table[i]->dec_refcount();
            _sender_table[i] = NULL;
        }
    }
}

MOOON_NAMESPACE_END

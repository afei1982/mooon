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
#ifndef AGENT_MESSAGE_H
#define AGENT_MESSAGE_H
#include <net/net_util.h>

#define AM_VERSION  0x0101 /** ��Ϣ�汾��(ռ�����ֽ�) */

/***
  * ������Ϣ���ͺ�
  * AMD: Down Agent Message
  * AMU: Uplink Agent Message  
  */

/***
  * ������Ϣ����AMU��ͷ
  */
#define AMU_HEARTBEAT              0  /** ������Ϣ */
#define AMU_REPORT                 1  /** ״̬�ϱ���Ϣ */
#define AMU_CONFIG_UPDATED_SUCCESS 2  /** ���ø��³ɹ� */
#define AMU_CONFIG_UPDATED_FAILURE 3  /** ���ø���ʧ�� */

/***
  * ������Ϣ����AMD��ͷ
  */
#define AMD_CONFIG_UPDATED 0 /** ���ø�����Ϣ */

MOOON_NAMESPACE_BEGIN

/***
  * ��������
  */
enum
{
    /***
      * ��������ȡֵ��Χ: [0, MAX_BUILTIN_AGENT_COMMAND]
      * ����������ȡֵ��Χ: [MAX_BUILTIN_AGENT_COMMAND+1, MAX_NON_BUILTIN_AGENT_COMMAND]
      */
    MAX_BUILTIN_AGENT_COMMAND     = 64, /** ������������ */
    MAX_NON_BUILTIN_AGENT_COMMAND = 255 /** ���ķ�����������ܳ���һ���ֽڴ�С */
};

#pragma pack(4) /** ���ֽڶ��� */

/***
  * Agent��Ϣ�ṹͷ��ר����Agent��Center��ͨѶ
  */
typedef struct
{
    uint8_t  byte_order;  /** �ֽ���0Ϊ���ֽ���1ΪС�ֽ��� */
    uint8_t  command;     /** ��Ϣ������ */
    uint16_t version;     /** ��Ϣ�汾�� */    

    uint32_t body_length; /** ��Ϣ�峤�� */            
    uint32_t check_sum;   /** У��ͣ�Ϊversion��command��body_length����֮�� */
}agent_message_header_t;

/***
  * ������Ϣͷ�������Ϣ��У����
  */
inline uint32_t get_check_sum(const agent_message_header_t& header)
{
    return header.byte_order + header.command + header.version + header.body_length;
}

/** ת���������ֽ��� */
inline void to_host_bytes(agent_message_header_t& header)
{
    net::CNetUtil::is_little_endian()
}

/***
  * ������Ϣ: ������Ϣ
  */
typedef struct
{
    agent_message_header_t header;    
    uint32_t mem_used;    /** ��ʹ�õ������ڴ���(MB) */
    uint32_t mem_buffer;  /** ����buffer�������ڴ���(MB) */
    uint32_t mem_cache;   /** ����cache�������ڴ���(MB) */
    uint32_t swap_used;   /** ��ʹ�õĽ����ռ��С(MB) */    
    uint32_t process_mem_used;  /** ����ʹ�õ������ڴ���(MB) */
    uint16_t cpu_load;          /** ���һ���ӵ�CPU���� */
    uint16_t cpu_number:4;      /** CPU���� */
    uint16_t nic_number:4;      /** �������� */    
    uint16_t disk_number:8;     /** �������� */    
}heartbeat_message_header_t;

typedef struct
{
    heartbeat_message_header_t header;
    uint32_t cpu_percent[0];    /** ��CPU�ٷֱ� */
    uint32_t net_traffic[0];    /** �������� */
    uint32_t disk_free[0];      /** ���̿��д�С(MB) */
}heartbeat_message_t;

/***
  * ������Ϣ: �����ļ�������Ϣ
  */
typedef struct
{
    agent_message_header_t header;
    uint32_t name_length:8;   /** ���������� */
    uint32_t file_size:24;    /** �����ļ��ֽ��� */
    uint64_t file_md5_low;    /** �����ļ�MD5�� */
    uint64_t file_md5_high;   /** �����ļ�MD5�� */
    char config_name[0];      /** ��������������β�� */    
    char file_content[0];     /** �����ļ����ݣ���Ϊ������ */
}config_updated_message_t;

/***
  * �ж�һ�������Ƿ�ΪAgent��������༴Agent�������ڲ�����
  * �ڲ������ȡֵ��ΧΪ: [0~64)��[64~255]Ϊ�û�����ȡֵ��Χ
  */
extern bool is_builtin_agent_command(uint16_t command);

/***
  * �ж�һ�������Ƿ�Ϊ��Agent��������༴��Agent�������ڲ�����
  * �ڲ������ȡֵ��ΧΪ: [0~1024]��[1025~65536]Ϊ�û�����ȡֵ��Χ
  */
extern bool is_non_builtin_agent_command(uint16_t command);

#pragma pack() /** ���ֽڶ��� */
MOOON_NAMESPACE_END
#endif // AGENT_MESSAGE_H

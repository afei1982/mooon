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


/***
  * ��Ϣ�ṹ��
  */
#pragma pack(4) /** ���ֽڶ��� */

/***
  * Agent��Ϣ�ṹͷ��ר����Agent��Center��ͨѶ
  */
typedef struct
{
    uint16_t version;     /** ��Ϣ�汾�� */
    uint16_t command;     /** ��Ϣ���� */
    uint32_t body_length; /** ��Ϣ�峤�� */
    uint32_t check_sum;   /** У��ͣ�Ϊversion��command��body_length����֮�� */
}agent_message_t;

/***
  * ������Ϣ: �����ļ�������Ϣ
  */
typedef struct
{
    agent_message_t header;
    uint32_t name_length;     /** ���������� */
    uint32_t file_size;       /** �����ļ��ֽ��� */
    uint64_t file_md5_low;    /** �����ļ�MD5�� */
    uint64_t file_md5_high;   /** �����ļ�MD5�� */
    char config_name[0];      /** ��������������β�� */    
    char file_content[0];     /** �����ļ����ݣ�������β�� */
}config_updated_message_t;

#pragma pack() /** ���ֽڶ��� */
#endif // AGENT_MESSAGE_H

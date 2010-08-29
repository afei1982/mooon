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
 * Author: JianYi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef MESSAGE_H
#define MESSAGE_H
#include "util/util_config.h"
#define MESSAGE_MAX 8192 /** ������Ϣ������С */

MY_NAMESPACE_BEGIN

// ��Ϣ��4�ֽڶ���
#pragma pack(4)

// unique object 

/** ���session_idҲΪ0�������ѡ���̣߳�����session_id��ѡ���߳� */ 
typedef struct
{
	uint32_t node_id;     /** �������Ϣ�Ľڵ� */
	uint16_t port;        /** Ŀ��˿� */
	uint16_t service_id;  /** �������Ϣ��service */
	uint32_t session_id;  /** �������Ϣ��session */
	uint32_t thread_id;   /** �߳�ID */
}uoid_t;

typedef struct
{
	uint32_t byte_order:1; /** �ֽ���: 0 - С�ֽ���1 - ���ֽ��� */
	uint32_t pad:7;        /** ����ֶ�: ȫ0 */
	uint32_t type:8;       /** ��Ϣ���� */
	uint32_t magic:16;     /** У���� */
	uint32_t version;      /** �汾�� */	
	uint32_t body_size;    /** ��Ϣ���ֽ��� */
	uoid_t   self_uoid;
	uoid_t   peer_uoid;
}TMessage;

#pragma pack()

MY_NAMESPACE_END
#endif // MESSAGE_H

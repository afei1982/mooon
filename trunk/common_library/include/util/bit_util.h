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
#ifndef _BIT_UTIL_H
#define _BIT_UTIL_H
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/***
  * λ����������
  */
class CBitUtil
{
public:
    /***
      * ��ָ��λȡ��������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      */
    static void flip(char* bitmap, uint32_t position);

    /***
      * ����ָ��λ�Ƿ�Ϊ1������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      */
    static bool test(char* bitmap, uint32_t position);

    /***
      * �õ�ָ��λ��ֵ������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      */
    static uint8_t get_bit(char* bitmap, uint32_t position);

    /***
      * ����ָ��λ��ֵ������Խ����
      * @bitmap: λͼ����λ������С��position
      * @position: ��bitmap�е�λλ��
      * @zero: ��position����λ����Ϊ0��1�����Ϊtrue������Ϊ0����������Ϊ1
      */
    static void set_bit(char* bitmap, uint32_t position, bool zero);
};

UTIL_NAMESPACE_END
#endif // _BIT_UTIL_H

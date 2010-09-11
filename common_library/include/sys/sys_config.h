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
#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "util/util_config.h"

// �������ֿռ��
#define SYS_NAMESPACE_BEGIN namespace sys {
#define SYS_NAMESPACE_END                      }
#define SYS_NAMESPACE_USE using namespace sys;

/** �´����ļ���Ĭ��Ȩ�� */
#define FILE_DEFAULT_PERM (S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH)
/** �´���Ŀ¼��Ĭ��Ȩ�� */
#define DIRECTORY_DEFAULT_PERM (S_IRWXU | S_IXGRP | S_IXOTH)

/** ����������ֽڳ��� */
#define INTERFACE_NAME_MAX 20

#endif // SYS_CONFIG_H

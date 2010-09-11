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
#ifndef FILE_UTIL_H
#define FILE_UTIL_H
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

class CFileUtil
{
    /** �ļ����ƺ���
      * @src_fd: �򿪵�Դ�ļ����
      * @dst_fd: �򿪵�Ŀ���ļ����
      * @return: �����ļ���С
      * @exception: �����׳�CSyscallException�쳣
      */
    static size_t file_copy(int src_fd, int dst_fd);
    static size_t file_copy(int src_fd, const char* dst_filename);
    static size_t file_copy(const char* src_filename, int dst_fd);
    static size_t file_copy(const char* src_filename, const char* dst_filename);

    /** �õ��ļ��ֽ���
      * @fd: �ļ����
      * @return: �����ļ��ֽ���
      * @exception: �����׳�CSyscallException�쳣
      */
    static off_t get_file_size(int fd);
    static off_t get_file_size(const char* filename);
};

SYS_NAMESPACE_END
#endif // FILE_UTIL_H

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
#ifndef SYS_UTIL_H
#define SYS_UTIL_H
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/** ��ϵͳ�����йصĹ����ຯ��ʵ�� */
class CSysUtil
{
public:
    /** �̰߳�ȫ�ĺ��뼶sleep����
      * @millisecond: ��Ҫsleep�ĺ�����
      */
    static void millisleep(uint32_t millisecond);
    /** �õ�ָ��ϵͳ���ô�������ַ���������Ϣ
      * @errcode: ϵͳ���ô�����
      * @return: ϵͳ���ô�����Ϣ
      */
    static std::string get_error_message(int errcode);

    /** �õ���ǰ����������ִ���ļ����ڵľ���·������β��������б�� */
    static std::string get_program_path();
    
    /** �õ�ָ���ļ�����������Ӧ���ļ���������·������
      * @fd: �ļ�������
      * @return: �ļ���������·�����֣����ʧ���򷵻ؿ��ַ���
      */
	static std::string get_filename(int fd);

    /** �õ�CPU�˸���
      * @return: ����ɹ������ش���0��CPU�˸��������򷵻�0
      */
	static uint16_t get_cpu_number();    

    /** �õ���ǰ����ջ
      * ע������: ����Դ����ʱ����-rdynamic��-gѡ�������ܿ������Ǻ�����ַ�������Ǻ�����������
      * @call_stack: �洢����ջ
      * @return: �ɹ�����true�����򷵻�false
      */
    static bool get_backtrace(std::string& call_stack);

    /** �õ�ָ��Ŀ¼�ֽ�����С�����̰߳�ȫ������ͬһʱ��ֻ�ܱ�һ���̵߳���
      * @dirpath: Ŀ¼·��
      * @return: Ŀ¼�ֽ�����С
      */
    static off_t du(const char* dirpath);

    /** �õ��ڴ�ҳ��С */
    static int get_page_size();

    /** �õ�һ�����̿ɳ��е�����ļ�(�����׽��ֵ�)����� */
    static int get_fd_max();

    /***
      * �ݹ�Ĵ���Ŀ¼
      * @dirpath: ��Ҫ������Ŀ¼      
      * @permissions: Ŀ¼Ȩ�ޣ�ȡֵ����Ϊ���е��������:
      *                    S_IRWXU, S_IRUSR, S_IWUSR, S_IXUSR
      *                    S_IRWXG, S_IRGRP, S_IWGRP, S_IXGRP
      *                    S_IRWXO, S_IROTH, S_IWOTH, S_IXOTH
      * @exception: �������׳�CSyscallException
      */
    static void create_directory(const char* dirpath, int permissions=DIRECTORY_DEFAULT_PERM);

    /***
      * �ݹ�Ĵ���Ŀ¼
      * @dirpath: ��Ҫ������Ŀ¼      
      * @permissions: Ŀ¼Ȩ��
      * @exception: �������׳�CSyscallException
      */
    static void create_directory_recursive(const char* dirpath, int permissions=DIRECTORY_DEFAULT_PERM);

    /** ����is_xxx������������������׳�CSyscallException�쳣 */
    static bool is_file(int fd);
    static bool is_file(const char* path);
    static bool is_link(int fd);
    static bool is_link(const char* path);
    static bool is_directory(int fd);
    static bool is_directory(const char* path);  
    
    /***
      * �Ƿ�����ǰ��������coredump�ļ�
      * @enable: ���Ϊtrue��������ǰ��������coredump�ļ��������ֹ
      * @core_file_size: �������ɵ�coredump�ļ���С�����ȡֵС��0�����ʾ�������ļ���С
      * @exception: ������ó������׳�CSyscallException�쳣
      */
    static void enable_core_dump(bool enabled=true, int core_file_size=-1);

    /** �õ���ǰ������ */
    static const char* get_program_name();
    static const char* get_program_short_name();
};

SYS_NAMESPACE_END
#endif // SYS_UTIL_H

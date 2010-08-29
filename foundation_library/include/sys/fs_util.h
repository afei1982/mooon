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
#include "sys/syscall_exception.h"
SYS_NAMESPACE_BEGIN

/***
  * �����ļ�ϵͳ�Ĺ�����
  * ͨ����CFSTable��Ľ�ϣ����Եõ����������ܴ�С��ʣ���С������
  */
class CFSUtil
{
public:
    typedef struct
    {
        unsigned long block_bytes;            /** ÿ������ֽ�����С */
        unsigned long total_block_nubmer;     /** �ܵĿ��� */
        unsigned long free_block_nubmer;      /** û��ʹ�õĿ��� */
        unsigned long avail_block_nubmer;     /** ��root�û����õĿ��� */
        unsigned long total_file_node_nubmer; /** �ܵ��ļ������ */
        unsigned long free_file_node_nubmer;  /** û��ʹ�õ��ļ������ */
        unsigned long avail_file_node_nubmer; /** ��root�û����õ��ļ������ */
        unsigned long file_name_length_max;   /** ֧�ֵ�����ļ������� */
    }fs_stat_t;

public:
    /***
      * ͳ��ָ��fd��ָ����ļ����ڵ��ļ�ϵͳ���õ����ļ�ϵͳ��������Ϣ
      * @fd: �ļ����
      * @stat_buf: �洢ͳ����Ϣ
      * @exception: ��������������׳�CSyscallException�쳣
      */
    void stat_fs(int fd, fs_stat_t& stat_buf);

    /***
      * ͳ��ָ��·����ָ����ļ����ڵ��ļ�ϵͳ���õ����ļ�ϵͳ��������Ϣ
      * @path: ·����Ϊ���ڷ���������ڵ�·������
      * @stat_buf: �洢ͳ����Ϣ
      * @exception: ��������������׳�CSyscallException�쳣
      */
    void stat_fs(const char* path, fs_stat_t& stat_buf);
};

/***
  * �ļ�ϵͳ��
  * ʾ��:
  * CFSTable fst;
  * if (fst) {
  *     fs_entry_t ent;
  *     while (get_entry(ent)) {
  *         printf("fs_name=%s\n", ent.fs_name);
  *         printf("dir_path=%s\n", ent.dir_path);
  *         printf("type_name=%s\n", ent.type_name);
  *     }
  * }
  */
class CFSTable
{
public:
    typedef struct
    {
        std::string fs_name;   /** �ļ�ϵͳ�� */
        std::string dir_path;  /** �ļ�ϵͳ�����ص�Ŀ¼·�� */
        std::string type_name; /** �ļ�ϵͳ����������ext3�� */
    }fs_entry_t;

public:
    /***
      * ������󣬲����ļ�ϵͳ���������ɺ�Ӧ�����ж϶����Ƿ���ã��磺
      * CFSTable mt; if (mt) { }���������if��䲻���٣�ֻ��Ϊ��ʱ�ſ��Ե���get_entry
      * @mounted: �Ƿ�ֻ����Ѿ����ص��ļ�ϵͳ
      * @fsname_prefix: �����ĵ��ļ�ϵͳ��ǰ׺��ֻ��ƥ��ĲŹ��ģ����ΪNULL�����ʾ���е�
      */
    CFSTable(bool mounted=true, const char* fsname_prefix="/");
    ~CFSTable();

    /** ��λ�������µ���get_entry��ȡ�ļ�ϵͳ�б� */
    void reset();

    /***
      * ���ļ�ϵͳ����ȡһ����¼
      * @return: ���ȡ����¼���򷵻�ָ��entry��ָ�룬���򷵻�NULL����ʾ�Ѿ����������е�
      */
    fs_entry_t* get_entry(fs_entry_t& entry);

    /***
      * �ж��ڹ��캯�����Ƿ�ɹ������ļ�ϵͳ��
      * @return: ����Ѿ��򿪣��򷵻�true�����򷵻�false
      */
    operator bool() const { return _fp != NULL; }

private:
    FILE* _fp;
    std::string _fsname_prefix;
};

SYS_NAMESPACE_END

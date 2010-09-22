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
#ifndef SYSCALL_EXCEPTION_H
#define SYSCALL_EXCEPTION_H
#include "sys/sys_config.h"
SYS_NAMESPACE_BEGIN

/** ϵͳ���ó����쳣������ϵͳ���ó���ʱ�����Դ��쳣�ͷ����������� */
class CSyscallException
{
public:
    /** ����ϵͳ�����쳣
      * @errcode: ϵͳ���ó������
      * @filename: �������������ļ���
      * @linenumber: ���������к�
      * @tips: �������ǿ��Ϣ�����Խ�һ��������ʲô����
      */
	CSyscallException(int errcode, const char* filename, int linenumber, const char* tips=NULL);    

    /** �õ����ó��������ļ��� */
    const char* get_filename() const { return _filename; }

    /** �õ����ó���ʱ�������ļ��к� */
    int get_linenumber() const { return _linenumber; }

    /** �õ����ó���ʱ��ϵͳ�����룬��Linux��Ϊerrnoֵ */
    int get_errcode() const { return _errcode; }

    /** �õ����ó���ʱ����ʾ��Ϣ����ʾ��Ϣ���ڸ�����ȷ���⣬Ϊ��ѡ����
      * ������ط�NULL�����ʾ����ʾ��Ϣ�������ʾ����ʾ��Ϣ
      */
    const char* get_tips() const { return _tips.empty()? NULL: _tips.c_str(); }

private:
	int _errcode;
	int _linenumber;
	char _filename[FILENAME_MAX];
    std::string _tips;
};

SYS_NAMESPACE_END
#endif // SYSCALL_EXCEPTION_H

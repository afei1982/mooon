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
 *
 * ����: close�����࣬�����Զ�����close��ʡȥÿ����֧��ʾ����close�ķ�����ʹ�����ü������
 *
 */
#ifndef CLOSE_HELPER_H
#define CLOSE_HELPER_H
#include "sys/sys_config.h"
SYS_NAMESPACE_BEGIN

/***
  * ������close���ֺ�����Ҫ������й��е�close����
  */
template <class ClassType>
class close_helper
{
public:
    /***
      * ����һ��close_helper����
      * @obj: ��Ҫclose_helper�Զ������乫��close�����Ķ���(��ָ��)
      */
    close_helper(ClassType& obj)
        :_obj(obj)
    {
    }
    
    /** ���������������Զ����ö����close���� */
    ~close_helper()
    {
        _obj.close();
    }
 
private:
    ClassType& _obj;
};
 
/***
  * ������������ļ����
  */
template <>
class close_helper<int>
{
public:
    close_helper<int>(int& fd)
        :_fd(fd)
    {
    }
    
    ~close_helper<int>()
    {
        if (_fd != -1)
        {
            ::close(_fd);
            _fd = -1;
        }
    }
 
private:
    int& _fd;
};
 
/***
  * ��Ա�׼I/O
  */
template <>
class close_helper<FILE*>
{
public:
    close_helper<FILE*>(FILE*& fp)
        :_fp(fp)
    {
    }
    
    ~close_helper<FILE*>()
    {
        if (_fp != NULL)
        {
            fclose(_fp);
            _fp = NULL;
        }
    }
 
private:
    FILE*& _fp;
};

SYS_NAMESPACE_END
#endif // CLOSE_HELPER_H

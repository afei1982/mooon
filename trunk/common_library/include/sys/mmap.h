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
#ifndef SYS_MMAP_H
#define SYS_MMAP_H
#include "sys/sys_util.h"
SYS_NAMESPACE_BEGIN

typedef struct
{
    int fd;      /** �����ͨ��fdӳ��ģ���Ϊ�ļ�����ĸ�ֵ������Ϊmmap����ʱ�򿪵��ļ���� */
    void* addr;  /** �ļ�ӳ�䵽�ڴ�ĵ�ַ������ļ�δ��ӳ�䵽�ڴ���ΪNULL */
    size_t len;  /** �ƻ�ӳ�䵽�ڴ�Ĵ�С */
}mmap_t;

/** �ڴ��ļ�ӳ������࣬���Գ�Ա��Ϊ��̬���Ա */
class CMMap
{
public:
    /** ��ֻ����ʽ���ļ�ӳ�䵽�ڴ�
      * @fd: �ļ��������������Ҫ����رմ˾����mmap_t�ṹ��fd��ԱΪ���ĸ�ֵ
      * @size: ��Ҫӳ�䵽�ڴ�Ĵ�С�����Ϊ0��ӳ�������ļ�
      * @offset: ӳ���ƫ��λ��
      * @size_max: ����ӳ���ֽ����������˴�С���ļ������ᱻӳ�䵽�ڴ棬mmap_t�ṹ��addr��Ա��ΪNULL
      * @return: ����ָ��mmap_t�ṹ��ָ�룬����ֵ���ǲ���ΪNULL
      * @exception: �����׳�CSyscallException�쳣
      */
    static mmap_t* map_read(int fd, size_t size=0, size_t offset=0, size_t size_max=0);

    /** ��ֻ����ʽ���ļ�ӳ�䵽�ڴ�
      * @filename: �ļ�����mmap_t�ṹ��fd��ԱΪ�򿪴��ļ��ľ��
      * @size_max: ����ӳ���ֽ����������˴�С���ļ������ᱻӳ�䵽�ڴ棬mmap_t�ṹ��addr��Ա��ΪNULL
      * @return: ����ָ��mmap_t�ṹ��ָ�룬����ֵ���ǲ���ΪNULL
      * @exception: �����׳�CSyscallException�쳣
      */
    static mmap_t* map_read(const char* filename, size_t size_max=0);

    /** ��ֻд��ʽ���ļ�ӳ�䵽�ڴ�
      * @fd: �ļ��������������Ҫ����رմ˾����mmap_t�ṹ��fd��ԱΪ���ĸ�ֵ
      * @size: ��Ҫӳ�䵽�ڴ�Ĵ�С�����Ϊ0��ӳ�������ļ�
      * @offset: ӳ���ƫ��λ��
      * @size_max: ����ӳ���ֽ����������˴�С���ļ������ᱻӳ�䵽�ڴ棬mmap_t�ṹ��addr��Ա��ΪNULL
      * @return: ����ָ��mmap_t�ṹ��ָ�룬����ֵ���ǲ���ΪNULL
      * @exception: �����׳�CSyscallException�쳣
      */
    static mmap_t* map_write(int fd, size_t size=0, size_t offset=0, size_t size_max=0);

    /** ��ֻд��ʽ���ļ�ӳ�䵽�ڴ�
      * @filename: �ļ�����mmap_t�ṹ��fd��ԱΪ�򿪴��ļ��ľ��
      * @size_max: ����ӳ���ֽ����������˴�С���ļ������ᱻӳ�䵽�ڴ棬mmap_t�ṹ��addr��Ա��ΪNULL
      * @return: ����ָ��mmap_t�ṹ��ָ�룬����ֵ���ǲ���ΪNULL
      * @exception: �����׳�CSyscallException�쳣
      */
    static mmap_t* map_write(const char* filename, size_t size_max=0);

    /** �Զ���д��ʽ���ļ�ӳ�䵽�ڴ�
      * @fd: �ļ��������������Ҫ����رմ˾����mmap_t�ṹ��fd��ԱΪ���ĸ�ֵ
      * @size: ��Ҫӳ�䵽�ڴ�Ĵ�С�����Ϊ0��ӳ�������ļ�
      * @offset: ӳ���ƫ��λ��
      * @size_max: ����ӳ���ֽ����������˴�С���ļ������ᱻӳ�䵽�ڴ棬mmap_t�ṹ��addr��Ա��ΪNULL
      * @return: ����ָ��mmap_t�ṹ��ָ�룬����ֵ���ǲ���ΪNULL
      * @exception: �����׳�CSyscallException�쳣
      */
    static mmap_t* map_both(int fd, size_t size=0, size_t offset=0, size_t size_max=0);

    /** �Զ���д��ʽ���ļ�ӳ�䵽�ڴ�
      * @filename: �ļ�����mmap_t�ṹ��fd��ԱΪ�򿪴��ļ��ľ��
      * @size_max: ����ӳ���ֽ����������˴�С���ļ������ᱻӳ�䵽�ڴ棬mmap_t�ṹ��addr��Ա��ΪNULL
      * @return: ����ָ��mmap_t�ṹ��ָ�룬����ֵ���ǲ���ΪNULL
      * @exception: �����׳�CSyscallException�쳣
      */
    static mmap_t* map_both(const char* filename, size_t size_max=0);

    /***
      * �ͷ��Ѵ������ڴ�ӳ�䣬�����ͨ��ָ���ļ���ӳ��ģ���ر���mmap�д򿪵ľ��
      * @ptr: �Ѵ������ڴ�ӳ��  
      * @exception: �����׳�CSyscallException�쳣
      */
    static void unmap(mmap_t* ptr);

    /***
      * ͬ���ؽ��ڴ�ˢ�µ�����
      * @ptr: ָ��map_read��map_both��map_write�õ���mmap_tָ��
      * @offset: ��Ҫˢ�µ�ƫ��λ��
      * @length: ��Ҫˢ�µĴ�С�����Ϊ0�����ʾ��ƫ�ƴ�����β��
      *          ��������߽磬��ֻˢ�µ��߽�
      * @invalid: �Ƿ��ʶ�ڴ�Ϊ��Ч
      * @exception: �����׳�CSyscallException�쳣
      */
    static void sync_flush(mmap_t* ptr, size_t offset=0, size_t length=0, bool invalid=false);

    /***
      * �첽�ؽ��ڴ�ˢ�µ�����
      * @ptr: ָ��map_read��map_both��map_write�õ���mmap_tָ��
      * @offset: ��Ҫˢ�µ�ƫ��λ��
      * @length: ��Ҫˢ�µĴ�С�����Ϊ0�����ʾ��ƫ�ƴ�����β��
      *          ��������߽磬��ֻˢ�µ��߽�
      * @invalid: �Ƿ��ʶ�ڴ�Ϊ��Ч
      * @exception: �����׳�CSyscallException�쳣
      */
    static void async_flush(mmap_t* ptr, size_t offset=0, size_t length=0, bool invalid=false);

private:
    static mmap_t* do_map(int prot, int fd, size_t size, size_t offset, size_t size_max, bool byfd);
};

/***
  * CMMap�����࣬�����Զ�ж���Ѿ�ӳ����ļ�
  */
class CMMapHelper
{
public:
    /** ����һ��CMmap������ */
    CMMapHelper(mmap_t*& ptr)
        :_ptr(ptr)
    {        
    }

    /** ���������������Զ�ж���Ѿ�ӳ����ļ� */
    ~CMMapHelper()
    {
        CMMap::unmap(_ptr);
        _ptr = NULL;
    }

private:
    mmap_t*& _ptr;
};

SYS_NAMESPACE_END
#endif // SYS_MMAP_H

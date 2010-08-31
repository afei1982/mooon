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
#ifndef TCP_WAITER_H
#define TCP_WAITER_H
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

class CTcpWaiter: public CEpollable
{
public:
	CTcpWaiter();
	~CTcpWaiter();

    void attach(int fd);

    /** ����SOCKET����
      * @buffer: ���ջ�����
      * @buffer_size: ���ջ������ֽ���
      * @return: ����յ����ݣ��򷵻��յ����ֽ���������Զ˹ر������ӣ��򷵻�0��
      *          ���ڷ��������ӣ���������ݿɽ��գ��򷵻�-1
      * @exception: ���Ӵ����׳�CSyscallException�쳣
      */
    ssize_t receive(char* buffer, size_t buffer_size);

    /** ����SOCKET����
      * @buffer: ���ͻ�����
      * @buffer_size: ��Ҫ���͵��ֽڴ�С
      * @return: ������ͳɹ����򷵻�ʵ�ʷ��͵��ֽ��������ڷ����������ӣ�������ܼ������ͣ��򷵻�-1
      * @exception: �����������������׳�CSyscallException�쳣
      * @ע�������֤������0�ֽڵ����ݣ�Ҳ����buffer_size�������0
      */
    ssize_t send(const char* buffer, size_t buffer_size);

    /** �������գ�����ɹ����أ���һ��������ָ���ֽ���������
      * @buffer: ���ջ�����
      * @buffer_size: ���ջ������ֽڴ�С
      * @return: ����ɹ����򷵻�true������������ӱ��Զ˹ر��򷵻�false
      * @exception: �����������������׳�CSyscallException�����ڷ��������ӣ�Ҳ�����׳�CSyscallException�쳣
      */
    bool complete_receive(char* buffer, size_t buffer_size);

    /** �������ͣ�����ɹ����أ������Ƿ�����ָ���ֽ���������
      * @buffer: ���ͻ�����
      * @buffer_size: ��Ҫ���͵��ֽ���
      * @return: �޷���ֵ
      * @exception: �������������׳�CSyscallException�쳣�����ڷ��������ӣ�Ҳ�����׳�CSyscallException�쳣
      * @ע�������֤������0�ֽڵ����ݣ�Ҳ����buffer_size�������0
      */
    void complete_send(const char* buffer, size_t buffer_size);

    ssize_t send_file(int file_fd, off_t *offset, size_t count);
    void complete_send_file(int file_fd, off_t *offset, size_t count);

private:
    void* _data_channel;
};

NET_NAMESPACE_END
#endif // TCP_WAITER_H

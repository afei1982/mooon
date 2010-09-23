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
 * Author: Jian Yi, eyjian@qq.com or eyjian@gmail.com
 */
#ifndef NET_TCP_CLIENT_H
#define NET_TCP_CLIENT_H
#include "net/epollable.h"
NET_NAMESPACE_BEGIN

/***
  * TCP�ͻ����࣬�ṩ�ͻ��˵ĸ��ֹ���
  */
class CTcpClient: public CEpollable
{
public:
	CTcpClient();
	~CTcpClient();

    /** �õ��Զ�IP��ַ */
    uint32_t get_peer_ip() const { return _peer_ip; }

    /** �õ��Զ˶˿ں� */
    uint16_t get_peer_port() const { return _peer_port; }

    /** ���öԶ�IP��ַ */
	void set_peer_ip(uint32_t ip) { _peer_ip = ip; }

    /** ���öԶ˶˿ں� */
	void set_peer_port(uint16_t port) { _peer_port = port; }

    /** �������ӵ�����ĳ�ʱ������ */
	void set_connect_timeout_milliseconds(uint32_t milli_seconds) { _milli_seconds = milli_seconds; }

    /***
      * �첽���ӣ������Ƿ����������ӳɹ���������������
      * @return: ������ӳɹ����򷵻�true����������������ӹ����У��򷵻�false
      * @exception: ���Ӵ����׳�CSyscallException�쳣
      */
    bool async_connect();

    /***
      * ��ʱ����
      * ��������������ӳɹ�����ȴ���set_connect_timeout_millisecondsָ����ʱ����
      * ��������ʱ������δ���ӳɹ�������������
      * @exception: ���ӳ����ʱ���׳�CSyscallException�쳣
      */
    void timed_connect();
    
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
      */
    ssize_t send(const char* buffer, size_t buffer_size);

    /** �������գ�����ɹ����أ���һ��������ָ���ֽ���������
      * @buffer: ���ջ�����
      * @buffer_size: ���ջ������ֽڴ�С������ʵ���Ѿ����յ����ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @return: ����ɹ����򷵻�true������������ӱ��Զ˹ر��򷵻�false
      * @exception: �����������������׳�CSyscallException�����ڷ��������ӣ�Ҳ�����׳�CSyscallException�쳣
      */
    bool complete_receive(char* buffer, size_t& buffer_size);

    /** �������ͣ�����ɹ����أ������Ƿ�����ָ���ֽ���������
      * @buffer: ���ͻ�����
      * @buffer_size: ��Ҫ���͵��ֽ���������ʵ���Ѿ������˵��ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @return: �޷���ֵ
      * @exception: �������������׳�CSyscallException�쳣�����ڷ��������ӣ�Ҳ�����׳�CSyscallException�쳣
      * @ע�������֤������0�ֽڵ����ݣ�Ҳ����buffer_size�������0
      */
    void complete_send(const char* buffer, size_t& buffer_size);

    /** �����ļ��������߱��뱣֤offset+count�������ļ���С
      * @file_fd: �򿪵��ļ����
      * @offset: �ļ�ƫ��λ�ã�����ɹ��򷵻��µ�ƫ��λ��
      * @count: ��Ҫ���͵Ĵ�С
      */
    ssize_t send_file(int file_fd, off_t *offset, size_t count);
    void complete_send_file(int file_fd, off_t *offset, size_t& count);

    /** �����ڴ�ӳ��ķ�ʽ���գ��������ݴ���ļ����ʺ��ļ�����̫��
      * @file_fd: �򿪵��ļ����
      * @size: ��Ҫд���ļ��Ĵ�С������ʵ���Ѿ����յ����ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @offset: д���ļ���ƫ��ֵ
      * @return: ������ӱ��Զ˹رգ��򷵻�false����ɹ�����true
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣
      */
    bool complete_receive_tofile_bymmap(int file_fd, size_t& size, size_t offset);

    /** ����write���õķ�ʽ���գ��������ݴ���ļ����ʺ������С���ļ������Ǵ��ļ��ᵼ�¸õ��ó�ʱ������
      * @file_fd: �򿪵��ļ����
      * @size: ��Ҫд���ļ��Ĵ�С������ʵ���Ѿ����յ����ֽ���(���ܳɹ�����ʧ�ܻ��쳣)
      * @offset: д���ļ���ƫ��ֵ
      * @return: ������ӱ��Զ˹رգ��򷵻�false����ɹ�����true
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣
      */
    bool complete_receive_tofile_bywrite(int file_fd, size_t& size, size_t offset);
    
    /***
      * һ���Զ�һ�����ݣ���ϵͳ����readv���÷���ͬ
      * @return: ����ʵ�ʶ�ȡ�����ֽ���
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣 
      */
    ssize_t readv(const struct iovec *iov, int iovcnt);

    /***
      * һ����дһ�����ݣ���ϵͳ����writev���÷���ͬ
      * @return: ����ʵ��д����ֽ���
      * @exception: �������ϵͳ���ô������׳�CSyscallException�쳣 
      */
    ssize_t writev(const struct iovec *iov, int iovcnt);

    /** �ж������Ƿ��Ѿ�����
      * @return: ��������Ѿ��������򷵻�true�����򷵻�false
      */
	bool is_connect_established() const;
    bool is_connect_establishing() const;

    /** ����Ϊ�Ѿ�����״̬���������첽���ӣ��������ʲô������
      * async_connect���ܷ�������������״̬�������ӳɹ�����Ҫ���ô˺��������ó��Ѿ�����״̬�������ڵ���close֮ǰ
      * ��һֱ����������״̬֮��
      */
    void set_connected_state();

public: // override
    /** �ر����� */
	virtual void close();

private:
    bool do_connect(int& fd, bool nonblock);

private:
	uint32_t _peer_ip;          /** ���ӵĶԶ�IP��ַ */
	uint16_t _peer_port;        /** ���ӵĶԶ˶˿ں� */
	uint32_t _milli_seconds;    /** ���ӳ�ʱ�ĺ����� */
    void* _data_channel;
	uint8_t _connect_state;     /** ����״̬��1: �Ѿ�������2: ���ڽ������ӣ�0: δ���� */
};

NET_NAMESPACE_END
#endif // NET_TCP_CLIENT_H

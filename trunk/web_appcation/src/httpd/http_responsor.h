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
#ifndef HTTP_RESPONSOR_H
#define HTTP_RESPONSOR_H
#include "http_cache.h"
#include "gtf/responsor.h"
#include "http_requester.h"
#include "util/util_config.h"
MY_NAMESPACE_BEGIN

class CHttpResponsor: public IResponsor
{
public:
	CHttpResponsor(CHttpRequester* requester);
	~CHttpResponsor();
	void set_filename(const char* filename, int filename_length);
    
private:
	virtual void reset();
	virtual int send_file(int sockfd);
	virtual off_t get_buffer_length() const;
	virtual char* get_buffer();
	virtual void offset_buffer(off_t offset);
	virtual bool keep_alive() const;

private:
	int _response_code;
    CHttpRequester* _requester;
	CCacheEntity* _cache_entity;
    
private:
	int _fd;	
	mutable off_t _header_length; // ��ͷ���ֽ���
	off_t _header_offset; // �Ѿ����͵İ�ͷ�ֽ���	
	off_t _body_length;   // �������ֽ���
	off_t _body_offset;   // �Ѿ����͵İ����ֽ���   	
	bool _send_header;    // �Ƿ��Ѿ���������Ӧ��ͷ��ʶ
	char* _body_buffer;
	char* _header_buffer;	
};

MY_NAMESPACE_END
#endif // HTTP_RESPONSOR_H

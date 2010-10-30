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
#ifndef PROTOCOL_PARSER_H
#define PROTOCOL_PARSER_H
#include <util/util_config.h>
MOOON_NAMESPACE_BEGIN

class CALLBACK_INTERFACE IProtocolParser
{
public:
    /** ���������������������α������澯 */
    virtual ~IProtocolParser() {}

    virtual void reset() = 0;

    virtual util::handle_result_t parse(const char* buffer, int buffer_length) = 0;
    virtual uint32_t get_buffer_length() const = 0;
    virtual char* get_buffer() = 0;
};

MOOON_NAMESPACE_END
#endif // PROTOCOL_PARSER_H

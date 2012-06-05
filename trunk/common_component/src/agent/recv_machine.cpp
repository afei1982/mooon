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
#include "recv_machine.h"
#include "agent_thread.h"
AGENT_NAMESPACE_BEGIN

CRecvMachine::CRecvMachine(CAgentThread* thread)
 :_thread(thread)
{
    set_next_state(rs_header);
}

// 状态机入口函数
// 状态机工作原理：-> rs_header -> rs_body -> rs_header
//                 -> rs_header -> rs_error -> rs_header
//                 -> rs_header -> rs_body -> rs_error -> rs_header
// 参数说明：
// buffer - 本次收到的数据，注意不是总的
// buffer_size - 本次收到的数据字节数
util::handle_result_t CRecvMachine::work(const char* buffer, size_t buffer_size)
{   
    RecvStateContext next_ctx(buffer, buffer_size);    
    util::handle_result_t hr = util::handle_continue;
    
    // 状态机循环条件为：util::handle_continue == hr
    while (util::handle_continue == hr)
    {        
        RecvStateContext cur_ctx(next_ctx);
        
        switch (_recv_state)
        {
        case rs_header:
            hr = handle_header(cur_ctx, &next_ctx);
            break;
        case rs_body:
            hr = handle_body(cur_ctx, &next_ctx);
            break;
        default:
            hr = handle_error(cur_ctx, &next_ctx);
            break;
        }
    }
        
    return hr;
}

void CRecvMachine::reset()
{
    set_next_state(rs_header);
}

// 处理消息头部
// 参数说明：
// cur_ctx - 当前上下文，
//           cur_ctx.buffer为当前收到的数据buffer，包含了消息头，但也可能包含了消息体。
//           cur_ctx.buffer_size为当前收到字节数
// next_ctx - 下一步上下文，
//           由于cur_ctx.buffer可能包含了消息体，所以在一次接收receive动作后，
//           会涉及到消息头和消息体两个状态，这里的next_ctx实际为下一步handle_body的cur_ctx
util::handle_result_t CRecvMachine::handle_header(const RecvStateContext& cur_ctx, RecvStateContext* next_ctx)
{
    if (_finished_size + cur_ctx.buffer_size < sizeof(agent_message_header_t))
    {
        memcpy(reinterpret_cast<char*>(&_header) + _finished_size
              ,cur_ctx.buffer
              ,cur_ctx.buffer_size);
              
        _finished_size += cur_ctx.buffer_size;
        return util::handle_continue;
    }
    else
    {
        size_t need_size = sizeof(agent_message_header_t) - _finished_size;
        memcpy(reinterpret_cast<char*>(&_header) + _finished_size
              ,cur_ctx.buffer
              ,need_size);
              
        // TODO: Check header here
        
        size_t remain_size = cur_ctx.buffer_size - need_size;
        if (remain_size > 0)
        {
            next_ctx->buffer = cur_ctx.buffer + need_size;
            next_ctx->buffer_size = cur_ctx.buffer_size - need_size;
        }
        
        // 只有当包含消息体时，才需要进行状态切换，
        // 否则维持rs_header状态不变
        if (_header.size > 0)
        {
            // 切换状态
            set_next_state(rs_body);
        }
        else
        {            
            CProcessorManager* processor_manager = _thread->get_processor_manager();            
            if (!processor_manager->on_message(_header, 0, NULL, 0))
            {
                return util::handle_error;
            }
        }
                
        return (remain_size > 0)
              ? util::handle_continue // 控制work过程是否继续循环
              : util::handle_finish;
    }
}

// 处理消息体
// 参数说明：
// cur_ctx - 当前上下文，
//           cur_ctx.buffer为当前收到的数据buffer，包含了消息体，但也可能包含了消息头。
//           cur_ctx.buffer_size为当前收到字节数
// next_ctx - 下一步上下文，
//           由于cur_ctx.buffer可能包含了消息头，所以在一次接收receive动作后，
//           会涉及到消息头和消息体两个状态，这里的next_ctx实际为下一步handle_header的cur_ctx
util::handle_result_t CRecvMachine::handle_body(const RecvStateContext& cur_ctx, RecvStateContext* next_ctx)
{
    CProcessorManager* processor_manager = _thread->get_processor_manager();
    
    if (_finished_size + cur_ctx.buffer_size < _header.size)
    {
        if (!processor_manager->on_message(_header, _finished_size, cur_ctx.buffer, cur_ctx.buffer_size))
        {
            return util::handle_error;
        }
        
        _finished_size += cur_ctx.buffer_size;
        return util::handle_continue;
    }
    else
    {
        size_t need_size = _header.size - _finished_size;
        if (!processor_manager->on_message(_header, _finished_size, cur_ctx.buffer, need_size))
        {
            return util::handle_error;
        }
        
        // 切换状态
        set_next_state(rs_header);
        
        size_t remain_size = cur_ctx.buffer_size - need_size;
        if (remain_size > 0)
        {
            next_ctx->buffer = cur_ctx.buffer + need_size;
            next_ctx->buffer_size = cur_ctx.buffer_size - need_size;
            return util::handle_continue;
        }

        return util::handle_finish;     
    }
}

util::handle_result_t CRecvMachine::handle_error(const RecvStateContext& cur_ctx, RecvStateContext* next_ctx)
{
    //AGENT_LOG_ERROR("Network error.\n");
    set_next_state(rs_header); // 无条件切换到rs_header，这个时候应当断开连接重连接
    return util::handle_error;
}

AGENT_NAMESPACE_END
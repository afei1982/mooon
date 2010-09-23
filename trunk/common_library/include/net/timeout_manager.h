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
#ifndef TIMEOUT_MANAGER_H
#define TIMEOUT_MANAGER_H
#include "net/timeoutable.h"
#include "util/list_queue.h"
NET_NAMESPACE_BEGIN

/***
  * ��ʱ����������ӿ�
  * ������ʱʱ���ص�����on_timeout_event����
  */
template <class TimeoutableClass>
class CALLBACK_INTERFACE ITimeoutHandler
{
public:
    /** ���������������������α������澯 */
    virtual ~ITimeoutHandler() {}
    /** ��ʱʱ���ص� */
    virtual void on_timeout_event(TimeoutableClass* timeoutable) = 0;
};

/***
  * ��ʱ����ģ���࣬ά��һ����ʱ���У��ṩ��ʱ��ⷽ��
  * TimeoutableClassҪ��ΪCTimeoutable��������
  * �����е����ж������ǰ�ʱ���Ⱥ�˳�����ģ����ֻ��Ҫ�Ӷ��׿�ʼ�����Щ��ʱ��
  * ���̰߳�ȫ�࣬���ͨ��һ���߳�һ��CTimeoutManagerʵ������
  * TimeoutableClass���͵Ķ���ͨ��Ҳ�����̣߳�
  * �Ᵽ֤��Ч��ǰ�ᣬʹ��������ʱ���0����
  */
template <class TimeoutableClass>
class CTimeoutManager
{
public: 
    /*** �޲������캯�� */
    CTimeoutManager()
        :_keep_alive_second(0)
        ,_timeout_handler(NULL)
    {
    }

    /** ���ó�ʱ������Ҳ���������ʱ���ڲ��㳬ʱ */
    void set_keep_alive_second(uint32_t keep_alive_second)
    {
        _keep_alive_second = keep_alive_second;
    }

    /** ���ó�ʱ������ */
    void set_timeout_handler(ITimeoutHandler<TimeoutableClass>* timeout_handler)
    {
        _timeout_handler = timeout_handler;
    }

    /***
      * ����ʱ������뵽��ʱ����β
      * @timeoutable: ָ��ɳ�ʱ�Ķ���ָ��
      * @current_time: ��ǰʱ��
      */
    void push(TimeoutableClass* timeoutable, time_t current_time)
    {
        timeoutable->set_timestamp(current_time);
        _list_queue.push(timeoutable);
    }

    /***
      * ��һ���ɳ�ʱ����Ӷ�����ɾ��
      * @timeoutable: ָ��ɳ�ʱ�Ķ���ָ��
      */
    void remove(TimeoutableClass* timeoutable)
    {
        timeoutable->set_timestamp(0);
        _list_queue.remove(timeoutable);
    }    

    /***
      * ����������Щ�������˳�ʱ
      * @current_time: ��ǰʱ��
      * ˵��: �Ӷ��׿�ʼѭ��������Щ�������˳�ʱ�������ʱ��������
      * �ص�ITimeoutHandler��on_timeout_event������
      * ֱ�Ӽ�⵽ĳ����δ��ʱ����������
      */
    void check_timeout(time_t current_time)
    {
        for (;;)
        {
            TimeoutableClass* timeoutable = _list_queue.front();
            if (NULL == timeoutable) break;

            time_t last_time = timeoutable->get_timestamp();
            if (!is_timeout(last_time, current_time)) break;
          
            _timeout_handler->on_timeout_event(timeoutable);
            _list_queue.remove(timeoutable);          
        }
    }

private:
    inline bool is_timeout(time_t last_time, time_t current_time)
    {
        return last_time + _keep_alive_second < current_time;
    }
    
private:
    time_t _keep_alive_second;
    ITimeoutHandler<TimeoutableClass>* _timeout_handler;    
    typename util::CListQueue<TimeoutableClass> _list_queue;
};

NET_NAMESPACE_END
#endif // TIMEOUT_MANAGER_H

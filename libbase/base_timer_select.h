

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
 * Author: 89617663@qq.com
 */
 

#ifndef _BASE_TIMER_SELECT_H
#define _BASE_TIMER_SELECT_H

#include "base_common.h"
#include "base_thread.h"


NS_BASE_BEGIN


//�ⲿӦ����Ҫ�̳�
class Select_Timer_handler
{
public:
	Select_Timer_handler();

	virtual ~Select_Timer_handler();

	virtual int handle_timeout(void *args = NULL);
};




//���̶߳�ʱ��
class Select_Timer : public Thread
{
public:
	Select_Timer();

	~Select_Timer();

	//handler�������ⲿӦ��ͨ��new ������ ����Ҫ���������ͷ�
	int register_timer_handler(Select_Timer_handler *handler, unsigned long interval=1000000);

private:
	virtual int svc();
	
private:
	Select_Timer_handler  *_handler;
    long _sec;         //seconds
    long _usec;        //microseconds   ΢��	

};

	
NS_BASE_END

#endif



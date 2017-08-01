
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

 

#ifndef _BASE_EVENT_HANDLER_H
#define _BASE_EVENT_HANDLER_H

#include "base_common.h"
#include "base_smart_ptr_t.h"


NS_BASE_BEGIN


class Event_Handler : public RefCounter
{	
public:
	Event_Handler(){printf("Event_Handler()\n");};

	virtual ~Event_Handler(){printf("~Event_Handler()\n");};

	//handle_xxxx �ɹ�����0��ʧ�ܷ��ط�0
	
	//���������������¼�
	virtual int handle_accept(int fd){return 0;};

	//������¼�
	virtual int handle_input(int fd){return 0;};

	//����д�¼�
	virtual int handle_output(int fd){return 0;};

	//������������¼�
	virtual int handle_exception(int fd){return 0;};

	//�������ӹر��¼�
	virtual int handle_close(int fd){return 0;};

	//udp reactor ʹ��
	virtual int handle_input(const std::string &ip, unsigned short port, char *buf, unsigned int len){return 0;};

	//����udp �ر��¼�(ģ��)
	virtual int handle_close(const std::string &ip, unsigned short port){return 0;};

	/*
	����ӿ��������ʵ��, ��������:
	virtual Event_Handler* renew(){return new Event_Handler;};
	*/
	virtual Event_Handler* renew() = 0;
	
};
typedef Smart_Ptr_T<Event_Handler>  Event_Handler_Ptr;


NS_BASE_END

#endif



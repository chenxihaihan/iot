
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

 

#ifndef _BASE_SOCKET_OPER_H
#define _BASE_SOCKET_OPER_H

#include "base_common.h"

NS_BASE_BEGIN


class Socket_Oper
{
public:
	
	/*
	�ڳ�ʱʱ����ѭ������ָ�����ȵ�����(ͬ��������ʽ)
	buf���ⲿӦ������ռ�
	lenָ���ⲿ����ռ��С������󷵻ؽ��յ������ݳ���
	timeout��λ��΢��
	����ֵ:
	(1) 0:fd�ر�
	(2) 1:����
	(3) 2:��ʱ
	(4)С��0: fd����ʧ�ܷ��ظ�ֵ������
	*/
	static int recv_n(int fd, char *buf, unsigned int &len, const unsigned int timeout=10000);



	/*
	�ڳ�ʱʱ���н�����һ��ָ�����ȵ�����(ͬ��������ʽ)
	buf���ⲿӦ������ռ�
	lenָ���ⲿ����ռ��С������󷵻ؽ��յ������ݳ���
	timeout��λ��΢��
	����ֵ:
	(1) 0:fd�ر�
	(2) 1:����
	(3) 2:��ʱ
	(4)С��0: fd����ʧ�ܷ��ظ�ֵ������
	*/
	static int recv(int fd, char *buf, unsigned int &len, const unsigned int timeout=10000);
	


	/*
	�ڳ�ʱʱ����ѭ������ָ�����ȵ�����(ͬ��������ʽ)
	buf���ⲿӦ������ռ�
	lenָ���ⲿ����ռ��С������󷵻ؽ��յ������ݳ���
	timeout��λ��΢��
	����ֵ:
	(1) 1:����
	(2) 2:��ʱ
	(3)С��0: fd����ʧ�ܷ��ظ�ֵ������
	*/
	static int send_n(int fd, const char *buf, unsigned int &len, int flags=0, const unsigned int timeout=10000);


	/*
	�ڳ�ʱʱ���н�����һ��ָ�����ȵ�����(ͬ��������ʽ)
	buf���ⲿӦ������ռ�
	lenָ���ⲿ����ռ��С������󷵻ؽ��յ������ݳ���
	timeout��λ��΢��
	����ֵ:
	(1) 1:����
	(2) 2:��ʱ
	(3)С��0: fd����ʧ�ܷ��ظ�ֵ������
	*/
	static int send(int fd, const char *buf, unsigned int &len, int flags=0, const unsigned int timeout=10000);


	static int send_s(int fd, const char *buf, unsigned int &len, int flags=0);
	


	/*
	(1) 1:����
	(2) 2:��ʱ
	(3)С��0: fd����ʧ�ܷ��ظ�ֵ������
	*/
	static int connect(int fd, std::string ip, unsigned short port, const unsigned int timeout=0);


	static int connect_s(int fd, std::string ip, unsigned short port, const unsigned int timeout=0);	

};

NS_BASE_END

#endif




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
 * Author: xucuiping(89617663@qq.com)
 */
 

#ifndef _BASE_TAIL_H
#define _BASE_TAIL_H

#include "base_common.h"
#include "base_thread.h"

NS_BASE_BEGIN

class Tail_Handler
{
public:
	Tail_Handler();

	virtual ~Tail_Handler();

	/*
	�ɹ���ȡ�ļ����ص��ú���
	�������ֵΪ��0�� ��ʾ����Ҫ������ȡ
	*/
	virtual int process(const std::string &line);

	//�ļ�����ĩ�˺�����
	virtual void complete();

};


/*
x-tail ֧��
*/
class X_Tail : public Thread
{
public:
	/*
	ending ��ʾ�ļ�����ĩ�˵��ַ����� ��Ҫҵ���Զ���
	truncate_mode=true: ��ʾ�ļ�ʹ��truncate ��ʽ�ضϣ��ļ������û�иı䣬��Ҫ���ļ�ĩβ��ʼ��ȡ�ļ���
	truncate_mode=false: ��ʾ�ļ�ʹ��close -> mv -> create new file  ��ʽ���ļ���������ı���Ҫ�����ļ�ͷ��ʼ���ļ���
	*/
	X_Tail(const std::string &file, const std::string &ending, Tail_Handler *handler, bool truncate_mode=true);

	~X_Tail();
	
private:
	virtual int do_init(void *args);
		
	virtual int svc();

private:
	std::string _file;
	std::string _ending; //�ļ�������ʾ�ַ���
	Tail_Handler *_handler;
	unsigned long long _pre_size;
	std::fstream _fin;
	bool _truncate_mode;
	bool _truncated;
	
};


NS_BASE_END

#endif



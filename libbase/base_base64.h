
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

 
#ifndef _BASE_SECURITY_H
#define _BASE_SECURITY_H

#include "base_common.h"

NS_BASE_BEGIN


//--------------------------- base64 ------------------------------
/*
Base64������������ASCII�ַ�������ת����ASCII�ַ���һ�ַ�����
����ĳЩϵͳ��ֻ��ʹ��ASCII�ַ�ϵͳ��
base64�ر��ʺ���http��mimeЭ���¿��ٴ������ݡ�
Ŀǰbase64�����ص�ַ�ϵõ��㷺��ʹ�á�
*/
class Base64
{
public:
	
	Base64();

	~Base64();

	int encrypt(const char *pSrc, const unsigned int len, char* &pDst);

	int decrypt(const char *pSrc, char* &pDst);
	
private:
	bool is_base64_alphabet(char ch);

	char get_base64_value(char ch);
};

NS_BASE_END

#endif




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
 

#ifndef _BASE_ZLIB_H
#define _BASE_ZLIB_H

#include "base_common.h"
#include "zlib.h"

NS_BASE_BEGIN

/*
deflate�� �����ս�   ----- ѹ��
inflate��  ��������   ----- ��ѹ
*/

class Zlib
{
public:
	
	//zip ѹ����pDst ���ڲ����������ⲿӦ��ͨ��free(pDst) �ͷſռ�
	static int zip_mem(char *pSrc, const int src_len, 
		                  char* &pDst, int &dst_len, const int level = -1);

	//unzip ��ѹ��pDst ���ڲ����������ⲿӦ��ͨ��free(pDst) �ͷſռ�
	static int unzip_mem(char *pSrc, const int src_len, 
		                     char* &pDst, int &dst_len);

	//gzip ѹ����pDst ���ڲ����������ⲿӦ��ͨ��free(pDst) �ͷſռ�
	static int gzip_mem(char *pSrc, const int src_len, 
		                  char* &pDst, int &dst_len, const int windowBits=MAX_WBITS, const int memLevel=MAX_MEM_LEVEL, const int level = -1);

	static std::string version();
	
};


NS_BASE_END


#endif




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
 

#ifndef _BASE_CRYPTOPP_H
#define _BASE_CRYPTOPP_H

#include "base_common.h"


/*
���ڼ��ܽ����㷨����Կ���Ⱥ����ݿ鳤�ȶ��ǹ̶��ģ�������ʵ��
Ӧ�������ĵĳ����ǲ�ȷ���ģ����һ�Զ���ڼ��ܽ����㷨�����ݿ�
�Ĺ̶����ȣ� �������ʱ�����Ҫʹ��һ����ģʽ��������Щ�����
�ͳ��������ڵļ��ּ��ܽ���ģʽ�� ���磺 ECB��CBC��CFB��OFB����Щ
ģʽ�ṩ�ֿ飬���棬�����ֽڣ����ܽ��ܵȹ��ܣ� �ṩ����Ч�ʡ�


(1)ECB (Electronic Code Book:���뱾) 
----------------------------------------
ECB����򵥵�ģʽ��ͬ�������ķ������Ǽ��ܳ���ͬ�����ķ��顣
����ڷ���ʹ��Ƶ�ʲ��ߵ����ķ����Ƿǳ��õģ�
�����ڷ���Ƶ�ʽϸ����ķ��鲻�Ǻܺã���Ϊ��ͬ�����Ļ���ܳ�
ͬ�������ı���η��ͣ� ����һ���İ�ȫ������
ECB���������Ƕ�ÿһ��������ͬ�ķ�ʽ���м��ܡ�������ǵ���Կ
�������ݲ��Ϸ����仯��ECB����ȫ��ȫ�ġ�
�����������ͬ������Կ���ܷ����Ժ󣬹����߿��ܻ��һЩ����
�������ñ���֪������Ϣ��


(2)CBC (Cipher Block Chaining:�����������)
----------------------------------------
CBCģʽ�ı��˼��ܷ�ʽ��ͬ�������ķ��鲻һ�����ܻ����ͬ����
���Ŀ飬��˽����ECB���ڵ���Ҫ���⡣
CBCʹ��ǰһ�������Ϣ���ܵ�ǰ���顣��˺�ECBģʽ����ͬ��
���������Ȼ�������⣬�Ǿ�����ͬ����Ϣ
��Ȼ���ܳ���ͬ�����ģ���Ϊ���еķ�����ͬʱ������ķ���ġ�
Ϊ�˽��������⣬����һ��IV(Initialization Vector����ʼ������)��Ҳ����ǰ���������ʵ���IV���⡣IV������
һ����ʼ�����ܳ�����������
���������ܱ��棬����ÿһ����Ϣ��˵�����ǲ�ͬ�ģ�ͨ�������ʽ��
��ʹ��������ͬ����Ϣ��ֻҪ�����в�ͬ��IV
����ô���Ǽ��ܺ������Ҳ�ǲ�ͬ�ġ��������������˵����ʼ��
�������ɾͺͿ�����ܹ�����ʹ�õļ���ֵ��һ���ġ�
CBC���ʺ��ı����䣬����ÿһ�ζ���Ҫ����һ�����������ݿ飬
һ��ѡ8���ַ���
���Ǹü���ģʽ��ȱ���ǲ���ʵʱ���ܣ�Ҳ����˵������ȵ�ÿ8��
�ֽڶ����ܵ�֮����ܿ�ʼ���ܣ�����Ͳ��ܵõ���ȷ�Ľ����
����Ҫ��ʵʱ�ԱȽϸߵ�ʱ����Եò������ˡ�

��������
���ܹ���:  ���Ŀ� XOR  IV  ---> ����---> ����
���ܹ���:  ����---> ����---> XOR ǰһ������� --->  ����
���ڽ��ܺ�����������һ�Σ� ��ͬ�������XOR������Ϊ0x0��
0x0�ٺ�Դ���ݿ������������Դ���ݿ�



(3)CFB (Cipher FeedBack:���뷴��)
----------------------------------------
CFB�Ĺ�����ʽ��CBC���ƣ���������ִ�и�С�����ݿ飬���͵���8λ��
��ǳ��ʺϼ���������Ի���������Ϣ��
��Ϊÿ�ο��Է��͵�һ���ֽ����ݿ顣��CBCһ����CFBҲ��Ҫһ��IV��
����ͬ��Կ���͵�ÿ����Ϣ��IV������
��Ψһ�ġ�


(4)OFB (Output FeedBack:�������)
-----------------------------------------
OFB�����ڴ������ܸ������ṩ���õı�����
��ֹ���ݶ�ʧ�⣬������CFB���ơ�
������һλ����Ҳֻ��������е�һλ���������ķ�ʽ�����
�����鶪ʧ����CBC�Լ�CFBһ����OFBҲ��Ҫһ��IV��

*/



NS_BASE_BEGIN


/*
��MD5�㷨��Ҫ����������Ϊ��MD5��512λ�����������������Ϣ��
��ÿһ�����ֱ�����Ϊ16��32λ�ӷ��飬�����Ⱥͳ�ʼ��32λ���龭����
һϵ�еĴ��������ĸ�32λ���飬�ٽ����ĸ�32λ������������512λ
����������㣬�������һ��128(16�ֽ�)λɢ��ֵ��
*/
class X_MD5
{
public:
	X_MD5();
	
	~X_MD5();

	static void info();
	
	bool calculate_digest(const std::string src, std::string &digest);	
};


//------------------------------------------


/*
��MD5 �㷨���ƣ�Ҳ�Ƕ�512 bit���ݿ��������
����256bit (32�ֽ�)��ժҪ�� �ٺ���һ��512 bit ��
�ݿ���м������ղ���shaժҪ��
shaĿǰ��sha-1 sha-2 sha-3 ��׼
sha-2ϵ�а���: sha-224, sha-256, sha-384, sha-512
��������ֱ�ʾ���ɵ�ժҪ����
*/
class X_SHA256
{
public:
	X_SHA256();
	
	~X_SHA256();

	static void info();
	
	bool calculate_digest(const std::string src, std::string &digest);
	
};


//------------------------------------------


class X_SHA1
{
public:
	X_SHA1();
	
	~X_SHA1();

	static void info();
	
	bool calculate_digest(const std::string src, std::string &digest);

	bool calculate_digest(const std::string src, char *digest);
	
};


//------------------------------------------


class X_HMACSHA1
{
public:
	X_HMACSHA1();
	
	~X_HMACSHA1();

	static void info();
	
	bool calculate_digest(const std::string key, const std::string src, std::string &digest);
	
};



//------------------------------------------


class X_BASE64
{
public:
	X_BASE64();
	
	~X_BASE64();

	static void info();

	bool encrypt(const char *pSrc, const unsigned int len, char* &pDst);

	bool decrypt(const char *pSrc, char* &pDst);
	
};



//------------------------------------------



/*
AES ����ʹ��128 bit (16 byte)��192 bit (24 byte)��256 bit (32 byte) ��Կ
ֻ�ܼ���128 bit (16 byte)�����ݿ飬 ���ܺ�����ݿ鳤��Ҳ��128λ
*/
class X_AES_CBC
{
public:
	X_AES_CBC();
	
	~X_AES_CBC();

	static void info();

	bool encrypt(const std::string key, const std::string plaintext, std::string &ciphertext, const std::string iv="");

	bool decrypt(const std::string key, const std::string ciphertext, std::string &plaintext, const std::string iv="");
	
};



//------------------------------------------




/*
DES ʹ��64 bit (8 byte)��Կ����64bit (8byte)�����ݿ飬 ���ܺ�����ݿ鳤��Ҳ��64λ
*/
class X_DES_CBC
{
public:
	X_DES_CBC();
	
	~X_DES_CBC();

	static void info();

	bool encrypt(const std::string key, const std::string plaintext, std::string &ciphertext, const std::string iv="");

	bool decrypt(const std::string key, const std::string ciphertext, std::string &plaintext, const std::string iv="");
	
};





//------------------------------------------


/*
RSA��ȻҲ���Կ����ǻ���Block�ļ��ܣ����ǣ�RSA������������Block�Ĵ�С�ǲ�һ���ģ�
Block�Ĵ�С��������ʹ�õ�RSA Key �ĳ��Ⱥ�RSA��paddingģʽ��
�ֱ��RSA�������ֳ��ȵ�Key��768, 1024, 2048����2��paddingģʽ��PKCS 1.5 ��OAEP�����������:

RSA                        InBlock��С   OutBlock��С  (��λ���ֽ�)
=============      ==========    =============
768bit/PKCS            85                    96
1024bit/PKCS          117                  128
2048bit/PKCS          245                  256

------------------------------------------------

768bit/OAEP           54                    192
1000bit/OAEP         83                     250
1024bit/OAEP         86                     256    <-- RSAES_OAEP_SHA_Encryptor Ĭ��ʹ��1024λRSA_Key����
2048bit/OAEP         214                   512

��ͬ��Կ���ȣ����ܳ��������ĳ���Ҫ������Ҫ������OAEP��InBlock/OutBlockҪ
��PKCS��InBlock/OutBlockҪС�������صĽǶȣ���ζ��OAEP paddingģʽ���������أ�
OAEPҪ��PKCS����ȫ

OAEP(��ѷǶԳƼ������):
��һ��ͨ����RSAһ��ʹ�õ���䷽����
OAEP��Bellare��Rogaway ����ġ�

PKCS(The Public-Key Cryptography Standards):
��������RSA���ݰ�ȫ��˾�����������ƶ���һ�鹫Կ����ѧ��׼��
���а���֤�����롢֤����¡�֤�����ϱ�������չ֤�������Լ�
����ǩ���������ŷ�ĸ�ʽ�ȷ����һϵ�����Э�顣

�����㷨�ǳ��ɿ�����ԿԽ��������Խ���ƽ⡣�����Ѿ���¶�����ף�
Ŀǰ���ƽ���RSA��Կ��768��������λ��Ҳ����˵�����ȳ���768λ����Կ��
���޷��ƽ⣨����û�˹�������������˿�����Ϊ��1024λ��RSA��Կ������ȫ��
2048λ����Կ���䰲ȫ��


X_RSA_OAEP�ṩ����1024 key

*/
class X_RSA_OAEP
{
public:
	X_RSA_OAEP();
	
	~X_RSA_OAEP();

	static void info();

	//key_path �����Ѿ�����
	bool generate_rsa_key(const std::string &key_path, const std::string &key_name);

	bool encrypt(const std::string public_key_file, const std::string plaintext, std::string &ciphertext);

	bool decrypt(const std::string private_key_file, const std::string ciphertext, std::string &plaintext);
	
};



NS_BASE_END


#endif



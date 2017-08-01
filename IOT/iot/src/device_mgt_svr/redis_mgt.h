
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
 

#ifndef _REDIS_MGT_H
#define _REDIS_MGT_H

#include "base/base_thread_mutex.h"
#include <hiredis/hiredis.h>
#include "base/base_singleton_t.h"
#include "comm/common.h"


USING_NS_BASE;

class Redis_Mgt
{
public:
    Redis_Mgt();
    virtual ~Redis_Mgt();

	int init(const std::string &ip, const unsigned int port, const std::string &auth);

	void release();

	void check();	

	int connect();

	int ping();

public:
	int create_security_channel(const std::string &id, std::string &key, std::string &err_info);

	int refresh_security_channel(const std::string &id, std::string &err_info);

	int release_security_channel(const std::string &id, std::string &err_info);

	int set(const std::string &id, const unsigned long long &value);

	int get(const std::string &id, unsigned long long &value);

	int hset(const std::string &id, const std::string &key, const std::string &value, std::string &err_info);

	int hget(const std::string &id, const std::string &key, std::string &value, std::string &err_info);	

	int remove(const std::string &id);
	
	bool isExist(const std::string &id);

	bool isExist(const std::string &id, const std::string &key);

private:
	redisContext *_redis;  //��Ҫȷ���Ƿ����̰߳�ȫ��
	Thread_Mutex _mutex;
	bool _valid;
	std::string _ip;
	unsigned short _port;
	std::string _auth;
	
};

#define PSGT_Redis_Mgt Singleton_T<Redis_Mgt>::getInstance()

#endif

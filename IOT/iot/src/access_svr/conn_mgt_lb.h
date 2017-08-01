
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


#ifndef _CONN_MGT_LB_H
#define _CONN_MGT_LB_H

#include "base/base_common.h"
#include "base/base_thread_mutex.h"
#include "comm/common.h"
#include "conn.h"

USING_NS_BASE;


class Conn_Mgt_LB
{
public:
	Conn_Mgt_LB();

	virtual ~Conn_Mgt_LB();

	int init(std::vector<StSvr> *svrs, Event_Handler *handler=NULL, bool asyn=false, bool do_register=false);

	void refresh(std::vector<StSvr> *svrs);

	virtual void check();
	
	virtual void check_conn();

	virtual void check_hb();

	bool get_conn(Conn_Ptr &conn);

	bool get_conn(const std::string &id, Conn_Ptr &conn);	

public:
	bool _out_ip;
	
private:
	Thread_Mutex _mutex;
	std::vector<Conn_Ptr> _conns;
	unsigned int _index;

	Event_Handler *_handler;
	bool _asyn;          //是否异步
	bool _do_register;   //是否需要注册和HB
	
};


#endif


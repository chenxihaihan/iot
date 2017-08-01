
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

 

#ifndef _BASE_SIGNAL_H
#define _BASE_SIGNAL_H

#include "base_common.h"
#include <signal.h>

NS_BASE_BEGIN


int send_signal(pid_t pid, int signo);

int add_signal_in_set(sigset_t &sigset, unsigned int num, ...);


//�ṩ���ź�����num ����ͺ���Ŀɱ�����б�Ҫһ�£������crash	
int block_process_signal(sigset_t sigset);


//�ú���һ�����ڴ����źŵ��߳��У� ѭ����������ȡ�������ź�
int wait_singal(sigset_t sigset, int &signo);


/*
handler ����������3��ֵ:
(1) SIG_IGN
(2) SIG_DFL
(3) ���յ����ź�Ҫ���õĺ���ָ��/��ַ
     void sig_handler(int sig)
*/
int signal_api(sighandler_t handler, unsigned int num, ...);


NS_BASE_END

#endif



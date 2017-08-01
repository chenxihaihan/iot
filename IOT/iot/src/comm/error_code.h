
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
 

#ifndef _ERROR_CODE_H
#define _ERROR_CODE_H


enum EnErrCode
{ 
	ERR_SUCCESS = 0,
	ERR_SYSTEM = -1,

	//Common -400 ~ -2
	ERR_COMMON_BEGIN = -400,
	ERR_QUEUE_FULL = -399,              //������
	ERR_PUSH_QUEUE_FAIL = -398,         //�������ʧ��
	ERR_INVALID_REQ = -397,             //���󴮸�ʽ�Ƿ�
	ERR_REACH_MAX_MSG = -396,           //���󴮴�����󳤶�
	ERR_REACH_MIN_MSG = -395,           //����С����С����
	ERR_SEND_FAIL = -394,               //����ʧ��
	ERR_RCV_FAIL = -393,                //����ʧ��
	ERR_SESSION_TIMEOUT = -392,         //�Ự��ʱ
	ERR_NO_CONN_FOUND = -391,           //�Ҳ���conn
	ERR_GET_MYSQL_CONN_FAILED = -390,   //��ȡmysql ����ʧ��
	ERR_DUPLICATE_KEY = -389,           //�ظ���
	ERR_TABLE_EXISTS = -388,            //���ݱ����
	ERR_REACH_MAX_MSG_CNT = -387,       //�ﵽÿ�������Ϣ����
	ERR_NO_WORKER_FOUND = -386, 		//û���ҵ���Ӧ��worker
	ERR_WORKER_ID_EXIST = -385,         //worker id �Ѿ�����		
	ERR_WORKER_FD_EXIST = -384, 		//worker fd �Ѿ�����

	//LB
	ERR_LB_BEGIN = -10000,
	
	//UID
	ERR_UID_BEGIN = -11000,

	//Conf
	ERR_CONF_BEGIN = -12000,

	//Security 
	ERR_SECURITY_BEGIN = -13000,

	//Access
	ERR_ACCESS_BEGIN = -14000,
	ERR_DATA_DECRYPT_FAILED = ERR_ACCESS_BEGIN - 10,     //���ݽ���ʧ��

	//Device Mgt
	ERR_DEVICE_MGT_BEGIN = -15000,

	//Family Mgt
	ERR_AUTHENTIONCATION_BEGIN = -16000,

	//User Mgt
	ERR_USER_MGT_BEGIN = -17000,
	//----UserCommon
	ERR_UNKNOW                              = ERR_USER_MGT_BEGIN - 10, //��֪����
	ERR_LESS_PARAM                          = ERR_USER_MGT_BEGIN - 20, //ȱ�ٲ���
	ERR_USER_INVALID_CODE                   = ERR_USER_MGT_BEGIN - 30, //code ��Ч
	ERR_USER_INVALID_SECRET                 = ERR_USER_MGT_BEGIN - 40, //�����Ч
	ERR_USER_PWD_COMPLEX                    = ERR_USER_MGT_BEGIN - 50, //���븴�ӶȲ���
	ERR_USER_PHONE_COMPLEX                  = ERR_USER_MGT_BEGIN - 60, //�ֻ����븴�ӶȲ���
	ERR_USER_PHONE_NOT_REGISTE              = ERR_USER_MGT_BEGIN - 70, //�ֻ�����δע��
	ERR_USER_PHONE_REGISTED                 = ERR_USER_MGT_BEGIN - 80, //�ֻ������Ѿ�ע��
	ERR_TOKEN_SIG_ERROR                     = ERR_USER_MGT_BEGIN - 90, //token ǩ������
	ERR_TOKEN_GENERATE_ERROR                = ERR_USER_MGT_BEGIN - 100,//token ����ʧ��
	ERR_TOKEN_INVALID                       = ERR_USER_MGT_BEGIN - 110,//token ��Ч
	ERR_TOKEN_EXPIRED                       = ERR_USER_MGT_BEGIN - 120,//token ����
	ERR_SECRET_EXPIRED                      = ERR_USER_MGT_BEGIN - 130,//Ʊ�ݹ���
	ERR_CODE_EXPIRED                        = ERR_USER_MGT_BEGIN - 140,//code ��Ч
	ERR_SECRET_INVALID                      = ERR_USER_MGT_BEGIN - 150,//Ʊ����Ч
	ERR_USER_ID_NOT_EXSIT                   = ERR_USER_MGT_BEGIN - 160,//�û�ID������
	ERR_USER_LOGIN_FORBID                   = ERR_USER_MGT_BEGIN - 170,//�û��ʺű�����
	ERR_CODE_NOT_EXIST                      = ERR_USER_MGT_BEGIN - 180,//code������,���ȷ�����֤��
	ERR_CODE_NOT_CORRECT                    = ERR_USER_MGT_BEGIN - 190,//code ����ȷ
	ERR_REDIS_NET_ERROR                     = ERR_USER_MGT_BEGIN - 200,//redis�������
	ERR_MYSQL_NET_ERROR                     = ERR_USER_MGT_BEGIN - 210,//mysql�������
	ERR_RESPONSE_BUF_ERROR                  = ERR_USER_MGT_BEGIN - 220,//�ذ�����
	ERR_INNER_SVR_NET_ERROR                 = ERR_USER_MGT_BEGIN - 230,//�ڲ������������ 
	ERR_USER_ID_NOT_EXIST                   = ERR_USER_MGT_BEGIN - 240,//�û�ID������ 

	//----get_phone_code                    
	ERR_GET_PHONE_CODE_LIMIT                = ERR_USER_MGT_BEGIN - 250,//ÿ����ֻ�ܷ�һ����֤��			
	ERR_GET_PHONE_CODE_SEND_SMS_ERROR       = ERR_USER_MGT_BEGIN - 260,//������֤��ʧ��
	//-----check_phone_code                 
	ERR_CHECK_PHONE_CODE_NOT_CORRECT        = ERR_USER_MGT_BEGIN - 270,//��֤�벻��ȷ
	ERR_CHECK_PHONE_CODE_NOT_EXIST          = ERR_USER_MGT_BEGIN - 280,//��֤�벻���ڣ��Ѿ�����

	//-----user_register                    	
	ERR_REGISTER_PHONE_REGISTED             = ERR_USER_MGT_BEGIN - 290,//�ֻ������Ѿ�ע��
	ERR_REGISTER_GET_UID_FAIL               = ERR_USER_MGT_BEGIN - 300,//ע��ʱ��ȡUID�Ľӿڷ��ش���
	//------login code                      
	ERR_LOGIN_CODE_TRY_LIMIT                = ERR_USER_MGT_BEGIN - 310,//code��¼�����������
	//------login pwd                       
	ERR_LOGIN_PWD_NOT_CORRECT               = ERR_USER_MGT_BEGIN - 320,//�������
    ERR_PWD_OLD_NOT_CORRECT                 = ERR_USER_MGT_BEGIN - 321,//�����벻��
	//-------um auth                         
	ERR_AUTH_TOKEN_NOT_MATCH                = ERR_USER_MGT_BEGIN - 330,//token��ƥ��
	//------set pwd
	//------reset pwd
	//------get profile
	//------set profile

	//MDP
	ERR_MDP_BEGIN = -18000,

	//Push
	ERR_PUSH_BEGIN = -19000,

	//SMS
	ERR_SMS_BEGIN = -20000,
	
	ERR_END
};


#endif



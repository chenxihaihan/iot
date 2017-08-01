
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
 

#ifndef _COMMON_H
#define _COMMON_H

#include "base/base_common.h"
#include "cmd.h"
#include "error_code.h"

USING_NS_BASE;


//----------- ϵͳ����-----------

const unsigned int MIN_MSG_LEN = 35;                  //��С��Ϣ����{"head":{"cmd":"hb", "time":111111}}
const unsigned int MAX_MSG_LEN = 20480;               //�����Ϣ����20KB
const unsigned int MSG_CNT_PER_SEC = 100;             //ÿ������ÿ������෢�ͼ�����Ϣ   
const unsigned int SECURITY_CHANNEL_TTL = 259200;     //��ȫͨ��key��ttl����(ȱʡ3��)
const unsigned int MAX_SEARCH_CLIENT_CNT = 500;       //ÿ�δ�Redis ��ȡ���ͻ�����
const unsigned int MAX_CLIENT_CNT = 20000;            //Access Svr ��󲢷�����
const std::string  AES_CBC_IV = "1234567890123456";   //AES�ԳƼ��ܽ���IV

const unsigned int MAX_UUID_LEN = 64;                 //�ͻ���UUID ����
const unsigned int MAX_AES_KEY_LEN = 32;              //AES key ����


//---------------------------  ȫ�ֱ���-------------------------------

//��������
const std::string ST_LB = "lb_svr";                   //LB Svr
const std::string ST_ACCESS = "access_svr";           //Access Svr
const std::string ST_USER_MGT = "user_mgt_svr";       //User Mgt Svr
const std::string ST_DEVICE_MGT = "device_mgt_svr";   //Device Mgt Svr
const std::string ST_FAMILY_MGT = "family_mgt_svr";   //Family Mgt Svr
const std::string ST_MDP = "mdp";                     //Mdp
const std::string ST_CONF = "conf_svr";               //Conf Svr
const std::string ST_UID = "uid_svr";                 //Uid Svr
const std::string ST_SECURITY = "security_svr";       //Security Svr
const std::string ST_PUSH = "push_svr";               //Push Svr
const std::string ST_SMS = "sms_svr";                 //SMS Svr
const std::string ST_BUSINESS = "business_svr";       //ҵ�������ͳ��


//�ն�����
const std::string TT_APP = "APP";                     //APP
const std::string TT_IOS = "IOS";                     //IOS
const std::string TT_ANDRIOD = "Andriod";             //Andriod
const std::string TT_ROUTER = "Router";               //Router
const std::string TT_UNKNOWN = "unknown";             //unknown


//�Ự����
const std::string MT_P2P = "P2P";                             //app --> app
const std::string MT_P2R = "P2R";                             //app --> router
const std::string MT_P2F = "P2F";                             //app --> family
const std::string MT_R2P = "R2P";                             //router -->app
const std::string MT_R2F = "R2F";                             //router -->family
const std::string MT_BROADCAST = "broadcast";                 //�㲥all
const std::string MT_BROADCAST_APP = "broadcast_app";         //�㲥app
const std::string MT_BROADCAST_ROUTER = "broadcast_router";   //�㲥·����


//Method ǰ׺
const std::string MP_UM = "um_";                      //user mgt svr request
const std::string MP_FM = "fm_";                      //family mgt svr request
const std::string MP_DM = "dm_";                      //device mgt svr request


// �û�״̬
const std::string US_ONLINE = "online";          //����
const std::string US_OFFLINE = "offline";        //����
const std::string US_KICKOFF = "kickoff";        //������ 


//ϵͳ���ü�ֵ
const std::string SYS_SEND_RATE = "send_rate";          //���������ӷ�������
const std::string SYS_MAX_CLIENT = "max_client";        //����������ͻ�����



//---------------------------  ȫ��ö��-------------------------------

// ��ͥ��Ա��ɫ
enum member_role_in_family
{
	FAMILY_ROLE_OWNER = 0,
	FAMILY_ROLE_MEMBER = 1,

	FAMILY_ROLE_END
};



// �Ա�
enum member_gender
{
	GENDER_MALE = 0,
	GENDER_FEMALE = 1,

	GENDER_END
};


//token������
enum TokenType {
	TokenTypeBegin			= 0,
	TokenTypeGetAccount		= 1,
	TokenTypeInviteMember	= 2,
	TokenTypeLogin			= 3,
	TokenTypeUpdatePwd		= 4,
	TokenTypeEnd			= 5
};


// �豸����
enum DM_DEVICE_CATEGORY
{
	DM_DEVICE_CATEGORY_AIR_CONDITIONER 		= 1,	// �յ�
	DM_DEVICE_CATEGORY_WINDOW_CURTAINS 		= 2,	// ����
	DM_DEVICE_CATEGORY_TELEVISION			= 3,	// ����
	DM_DEVICE_CATEGORY_ELECTRIC_COOKER		= 4,	// �緹��
	DM_DEVICE_CATEGORY_LAMP					= 5,	// ��
	DM_DEVICE_CATEGORY_ELECTRONIC_BALANCE	= 6,	// ���ӳ�
	DM_DEVICE_CATEGORY_CAMERA				= 7,	// ����ͷ
	DM_DEVICE_CATEGORY_INTELLIGENT_SWITCH	= 8,	// ���ܿ���
	DM_DEVICE_CATEGORY_AIR_QUALITY_DETECTOR	= 9,	// �������������
	DM_DEVICE_CATEGORY_HYGROTHERMOGRAPH		= 10,	// ��ʪ�ȼ�
	DM_DEVICE_CATEGORY_ROUTER				= 11,	// ·����
	DM_DEVICE_CATEGORY_DOOR_SENSOR			= 12,	// �Ŵ�������
	DM_DEVICE_CATEGORY_BODY_SENSOR			= 13,	// ���崫����
	DM_DEVICE_CATEGORY_VISUAL_INTERCOM		= 14,	// ���ӶԽ���

	DM_DEVICE_CATEGORY_END
};



// �豸��Ч��Ч
enum DM_DEVICE_STATE
{
	DM_DEVICE_STATE_ENABLE = 0,
	DM_DEVICE_STATE_DISNABLE = 1,

	DM_DEVICE_STATE_END
};



// �澯����״̬
enum DM_DEVICE_ALERT_STATUS
{
	DM_DEVICE_ALERT_STATUS_INITIAL = 0,
	DM_DEVICE_ALERT_STATUS_HANDLED = 1,
	DM_DEVICE_ALERT_STATUS_IGNORE = 2,

	DM_DEVICE_ALERT_STATUS_END
};



// �豸״̬����
enum DM_DEVICE_STATUS_TYPE
{
	DM_DEVICE_STATUS_TYPE_TEST = 1,
		
	DM_DEVICE_STATUS_TYPE_END
};



// �豸�澯����
enum DM_DEVICE_ALERT_TYPE
{
	DM_DEVICE_ALERT_TYPE_TEST = 1,
		
	DM_DEVICE_ALERT_TYPE_END
};



//---------------------------  ȫ�ֽṹ��-------------------------------


//ϵͳ���ýṹ��
typedef struct StSysInfo
{
	std::string _id;                             //ԭʼID
	std::string _new_id;                         //��ID
	std::string _log_id;                         //��־ID
	unsigned int _pid;                           //����ID
	std::string _ip;                             //IP��ַ
	std::string _ip_out;                         //����IP��ַ
	unsigned short _port;                        //�����˿�
	unsigned short _worker_port;                 //ע��˿�
	unsigned short _thr_num;                     //�����̸߳���
	unsigned int _max_queue_size;                //����������ߴ�
	unsigned int _rate_limit;                    //��������
	std::string _TZ;                             //ʱ��
	
	StSysInfo():_id(""), _new_id(""), _ip(""), _port(0), _worker_port(0),
		_thr_num(0), _max_queue_size(0), _rate_limit(0), _TZ("")
	{
	}
	
}StSysInfo;


//----------------------------------------------


typedef struct StSvr
{
   	std::string _ip;                   //����IP
   	std::string _ip_out;               //����IP��ַ
	unsigned short _port;          	   //����˿�

	StSvr(): _ip(""), _ip_out(""), _port(0)
	{
	}
	
} StSvr;


//----------------------------------------------


//mysql ����������Ϣ�ṹ��
typedef struct StMysql_Access
{
	std::string _ip;
	unsigned short _port;
	std::string _db;
	std::string _user;
	std::string _pwd;
	std::string _chars;
	unsigned int _num;
	
	StMysql_Access(): _ip(""), _port(0), _db(""), _user(""), _pwd(""), _chars(""), _num(1)
	{
	}
	
} StMysql_Access;



//Mongo ����������Ϣ�ṹ��
typedef struct StMongo_Access
{
	std::string _ip;
	unsigned short _port;
	std::string _server;
	std::string _db;
	std::string _collection;
	
	StMongo_Access(): _ip(""), _port(0), _server(""), _db(""), _collection("")
	{
	}
	
} StMongo_Access;




//Redis ����������Ϣ�ṹ��
typedef struct StRedis_Access
{
	std::string _ip;
	unsigned short _port;
	std::string _auth;
	
	StRedis_Access(): _ip(""), _port(0), _auth("")
	{
	}
	
} StRedis_Access;



//----------------------------------------------


/*
�ͻ�����Ϣ�ṹ��
4GB �ڴ������Դ洢4000W�ͻ��˼�¼(ÿ���ͻ���100�ֽ�)
4GB �ڴ����2000W �ͻ��˼�¼����
*/
typedef struct StClient
{   
	unsigned long long _id;            //�ͻ���ID:  �û�ID��Router ID
	std::string _type;                 //�ͻ�������
	std::string _session_id;           //�ỰID

	std::string _uuid;                 //��ȫͨ��ID 
	std::string _key;                  //��ȫͨ����Կ

	int _fd;                           //�ÿͻ���fd
	std::string _ip;                   //�ͻ��˵�IP
	unsigned short _port;              //�ͻ��˶˿�
	
	unsigned long long _stmp_create;    //�ÿͻ��˴���ʱ���
	std::string _geography;             //����λ��
	
	StClient(): _id(0), _type(TT_UNKNOWN), _session_id(""), _uuid(""), _key(""), _fd(-1), _ip(""), _port(0), 
		_stmp_create(0), _geography("")
	{
	
	}
	
} StClient;



#endif


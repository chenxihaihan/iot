
#include "redis_mgt.h"
#include "redis_timer_handler.h"
#include "base/base_logger.h"
#include "base/base_time.h"
#include "base/base_convert.h"

extern Logger g_logger;


Redis_Mgt::Redis_Mgt(): _redis(NULL), _valid(false), _ip(""), _port(0), _auth("") 
{

}


Redis_Mgt::~Redis_Mgt()
{

}



int Redis_Mgt::init(const std::string &ip, const unsigned int port, const std::string &auth)
{
	int nRet = 0;

	_ip = ip;
	_port = port;
	_auth = auth;

	//��ʼ��������һ�£����涨ʱ���ᶨʱ���redis server��״̬
	connect();

	//����conn mgt  timer
	XCP_LOGGER_INFO(&g_logger, "--- prepare to start redis mgt timer ---\n");
	Select_Timer *timer = new Select_Timer;
	Redis_Timer_handler *conn_thandler = new Redis_Timer_handler;
	nRet = timer->register_timer_handler(conn_thandler, 5000000);
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "register redis mgt timer handler falied, ret:%d\n", nRet);
		return nRet;
	}
		
	nRet = timer->init();
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "int redis mgt timer failed, ret:%d\n", nRet);
		return nRet;
	}

	nRet = timer->run();
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "redis mgt timer run failed, ret:%d\n", nRet);
		return nRet;
	}
	XCP_LOGGER_INFO(&g_logger, "=== complete to start redis mgt timer ===\n");
	
	return 0;
	
}




void Redis_Mgt::release()
{
	//����ͷ� redisContext ����    
	redisFree(_redis);
	_redis = NULL;
	_valid = false;
}


void Redis_Mgt::check()
{
	int nRet = 0;
	
	Thread_Mutex_Guard guard(_mutex);

	if(_valid)
	{
		nRet = ping();
		if(nRet == 0)
		{
			//ping �ɹ�
			return;
		}
		else
		{
			//ping ʧ��
			connect();
		}

	}
	else
	{
		//��������
		connect();
	}
	
}



int Redis_Mgt::connect()
{
	int nRet = 0;
	
	/*
	���Ӵ����Ժ��ڲ�����һ�� redisContext �Ѷ���һ�����ӶϿ���Ҫ�������ӣ�
	û���Զ���������
	*/
    struct timeval timeout = {0, 300000};
    _redis = redisConnectWithTimeout(_ip.c_str(), _port, timeout);
    if (_redis == NULL || _redis->err) 
	{
        if (_redis) 
		{
			XCP_LOGGER_INFO(&g_logger, "connect redis(%s:%u) failed, err:%s\n", 
				_ip.c_str(), _port, _redis->errstr);
			release();

        } 
		else 
		{
			XCP_LOGGER_INFO(&g_logger, "connect redis(%s:%u) failed, can't allocate redis context\n", 
				_ip.c_str(), _port);
        }
		
        return -1;
    }

	XCP_LOGGER_INFO(&g_logger, "connect redis(%s:%u) success.\n", _ip.c_str(), _port);
	
	if(!_auth.empty())
	{
		redisReply *reply = NULL;
		reply = (redisReply*)redisCommand(_redis, "auth %s", _auth.c_str());
		if(!reply)
		{
			//���redis server ֹͣ��reply is null
			XCP_LOGGER_INFO(&g_logger, "reply is null, pls check redis server(%s:%u)\n", _ip.c_str(), _port);
			release();
			return -1;
		}
		else
		{
			if(reply && (reply->type == REDIS_REPLY_STATUS) && (strcasecmp(reply->str,"OK") == 0))
			{
				XCP_LOGGER_INFO(&g_logger, "auth redis server(%s:%u) success.\n", _ip.c_str(), _port);
			}
			else
			{
				XCP_LOGGER_INFO(&g_logger, "auth redis failed, err:%s, pls check redis server(%s:%u), auth:%s\n", 
					reply->str, _ip.c_str(), _port, _auth.c_str());
				release();
				return -1;
			}
		}

	}

	_valid = true;

	return nRet;
	
}



int Redis_Mgt::ping()
{
	int nRet = 0;
	
	if(!_valid || !_redis)
	{
		XCP_LOGGER_INFO(&g_logger, "redisContext is invalid, pls check redis server(%s:%u)\n", 
			_ip.c_str(), _port); 

		return -1;
	}

    redisReply *reply = NULL;
	reply = (redisReply*)redisCommand(_redis, "ping");
	if(!reply)
	{
		//���redis server ֹͣ��reply is null
		XCP_LOGGER_INFO(&g_logger, "reply is null, pls check redis server(%s:%u)\n", 
		_ip.c_str(), _port);
		nRet = -2;

		release();
	}
	else
	{
		if(reply && (reply->type == REDIS_REPLY_STATUS) && (strcasecmp(reply->str,"PONG") == 0))
		{
			//XCP_LOGGER_INFO(&g_logger, "ping redis server(%s:%u) success.\n", _ip.c_str(), _port);
		}
		else
		{
			XCP_LOGGER_INFO(&g_logger, "ping redis failed, err:%s, pls check redis server(%s:%u)\n", 
				reply->str, _ip.c_str(), _port);
			nRet = -3;

			release();
		}
    }

    freeReplyObject(reply);

	return nRet;
	
}




int Redis_Mgt::register_route(const unsigned long long id, const std::string &access_svr_id, std::string &err_info)
{
	int nRet = 0;
	err_info = "";

	XCP_LOGGER_INFO(&g_logger, "prepare to register route. id:%llu, access_svr_id:%s\n", id, access_svr_id.c_str());
		
	Thread_Mutex_Guard guard(_mutex);
	
	if(!_valid || !_redis)
	{
		XCP_LOGGER_INFO(&g_logger, "redisContext is invalid, pls check redis server(%s:%u)\n", _ip.c_str(), _port);
		err_info = "user redis svr is disconnect.";
		return -1;
	}

    redisReply *reply = (redisReply*)redisCommand(_redis, "sadd mdp:%llu %s", id, access_svr_id.c_str());
	if(!reply)
	{
		//���redis server ֹͣ��reply is null
		XCP_LOGGER_INFO(&g_logger, "reply is null, pls check redis server(%s:%u).\n", _ip.c_str(), _port);
		err_info = "reply is null, pls check redis server.";
		release();
		return -1;
	}
	freeReplyObject(reply);

	XCP_LOGGER_INFO(&g_logger, "complete to register route.\n");

	return nRet;

}



int Redis_Mgt::unregister_route(const unsigned long long id, const std::string &access_svr_id, std::string &err_info)
{
	int nRet = 0;
	err_info = "";

	XCP_LOGGER_INFO(&g_logger, "prepare to unregister route. id:%llu, access_svr_id:%s\n", id, access_svr_id.c_str());
		
	Thread_Mutex_Guard guard(_mutex);
	
	if(!_valid || !_redis)
	{
		XCP_LOGGER_INFO(&g_logger, "redisContext is invalid, pls check redis server(%s:%u)\n", _ip.c_str(), _port);
		err_info = "user redis svr is disconnect.";
		return -1;
	}

    redisReply *reply = (redisReply*)redisCommand(_redis, "srem mdp:%llu %s", id, access_svr_id.c_str());
	if(!reply)
	{
		//���redis server ֹͣ��reply is null
		XCP_LOGGER_INFO(&g_logger, "reply is null, pls check redis server(%s:%u).\n", _ip.c_str(), _port);
		err_info = "reply is null, pls check redis server.";
		release();
		return -1;
	}
	freeReplyObject(reply);

	XCP_LOGGER_INFO(&g_logger, "complete to unregister route.\n");

	return nRet;

}



int Redis_Mgt::get_security_channel(const std::string &id, std::string &key, std::string &err_info)
{
	int nRet = 0;
	err_info = "";
	
	Thread_Mutex_Guard guard(_mutex);
	
	if(!_valid || !_redis)
	{
		XCP_LOGGER_INFO(&g_logger, "redisContext is invalid, pls check redis server(%s:%u)\n", _ip.c_str(), _port);
		err_info = "user redis svr is disconnect.";
		return -1;
	}

    redisReply *reply = NULL;
    reply = (redisReply*)redisCommand(_redis, "hmget %s key %s", id.c_str(), key.c_str());
	if(!reply)
	{
		//���redis server ֹͣ��reply is null
		XCP_LOGGER_INFO(&g_logger, "reply is null, pls check redis server(%s:%u).\n", _ip.c_str(), _port);
		err_info = "reply is null, pls check redis server.";
		release();
		return -1;
		
	}
	else
	{
		if((reply->type == REDIS_REPLY_ARRAY))
		{
			if((reply->element[0]->type == REDIS_REPLY_STRING))
			{
				key = reply->element[0]->str;
			}
			
		}
		else
		{
			XCP_LOGGER_INFO(&g_logger, "the reply type of security channel id isn't array.\n");
			err_info = "the reply type of security channel id isn't array.";
		}

	}
	freeReplyObject(reply);
	
	return nRet;

}




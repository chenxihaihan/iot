
#include "conn_mgt_lb.h"
#include "base/base_logger.h"
#include "base/base_string.h"
#include "base/base_time.h"
#include "base/base_timer_select.h"
#include "conn_timer_handler.h"


extern Logger g_logger;


//--------------- conn mgt ----------------
Conn_Mgt_LB g_lb_conn;
Conn_Mgt_LB g_conf_mgt_conn;
Conn_Mgt_LB g_user_mgt_conn;
Conn_Mgt_LB g_device_mgt_conn;
Conn_Mgt_LB g_family_mgt_conn;
Conn_Mgt_LB g_mdp_conn;
Conn_Mgt_LB g_push_conn;


Conn_Mgt_LB::Conn_Mgt_LB(): _index(0), _handler(NULL), _asyn(false), _svr_type(ST_BUSINESS)
{

}



Conn_Mgt_LB::~Conn_Mgt_LB()
{

}



int Conn_Mgt_LB::init(std::vector<StSvr> *svrs, Event_Handler *handler, bool asyn, const std::string &svr_type)
{
	int nRet = 0;

	_handler = handler;
	_asyn = asyn;
	_svr_type = svr_type;

	refresh(svrs);

	//启动conn mgt  timer，5秒钟就可以检测对端服务是否重新启动
	XCP_LOGGER_INFO(&g_logger, "--- prepare to start conn mgt lb timer ---\n");
	Select_Timer *timer_req = new Select_Timer;
	Conn_Timer_handler *conn_thandler = new Conn_Timer_handler;
	conn_thandler->_conn_mgt = this;
	nRet = timer_req->register_timer_handler(conn_thandler, 5000000);
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "register conn mgt lb timer handler falied, ret:%d\n", nRet);
		return nRet;
	}
		
	nRet = timer_req->init();
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "int conn mgt lb timer failed, ret:%d\n", nRet);
		return nRet;
	}

	nRet = timer_req->run();
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "conn mgt lb timer run failed, ret:%d\n", nRet);
		return nRet;
	}
	XCP_LOGGER_INFO(&g_logger, "=== complete to start conn mgt lb timer ===\n");
	
	return 0;

}




void Conn_Mgt_LB::refresh(std::vector<StSvr> *svrs)
{
	Thread_Mutex_Guard guard(_mutex);
	
	//释放原来连接池中的无效链接
	std::vector<Conn_Ptr>::iterator itr = _conns.begin();
	for(; itr != _conns.end();)
	{
		bool found = false;
		for(unsigned int i=0; i<svrs->size(); i++)
		{
			if(((*svrs)[i]._ip == (*itr)->_svr._ip) &&
			   ((*svrs)[i]._port == (*itr)->_svr._port))
			{
				found = true;
				break;
			}
		}

		//对于无效的conn关闭和释放
		if(!found)
		{
			std::string id = format("%s_%u", (*itr)->_svr._ip.c_str(), (*itr)->_svr._port);
			XCP_LOGGER_INFO(&g_logger, "**** release useless conn(%s) ****\n", id.c_str());
			printf("**** release useless conn(%s) ****\n", id.c_str());
			
			(*itr)->close();
			_conns.erase(itr); //调用Conn析构函数
		}
		else
		{
			++itr;
		}

	}
	
		
	//创建新链接
	for(unsigned int i=0; i<svrs->size(); i++)
	{
		bool found = false;
		std::vector<Conn_Ptr>::iterator itr = _conns.begin();
		for(; itr != _conns.end(); itr++)
		{
			if(((*svrs)[i]._ip == (*itr)->_svr._ip) &&
			   ((*svrs)[i]._port == (*itr)->_svr._port))
			{
				found = true;
				break;
			}
		
		}

		if(!found)
		{
			std::string id = format("%s_%u", (*svrs)[i]._ip.c_str(), (*svrs)[i]._port);
			XCP_LOGGER_INFO(&g_logger, "**** add new conn(%s) ****\n", id.c_str());
			printf("**** add new conn(%s) ****\n", id.c_str());
			
			Conn_Ptr conn = new Conn((*svrs)[i], _handler, _asyn);
			conn->connect();
			
			//新链接追加在后面
			_conns.push_back(conn);
		}
		
	}

	
}





void Conn_Mgt_LB::check()
{
	if(_svr_type == ST_BUSINESS)
	{
		check_conn();
	}
	else if(_svr_type == ST_LB)
	{
		check_lb_conn();
	}
	else if(_svr_type == ST_MDP)
	{
		check_mdp_conn();
	}
	else
	{
		//no-todo
	}
	
}




void Conn_Mgt_LB::check_conn()
{
	Thread_Mutex_Guard guard(_mutex);
	
	std::vector<Conn_Ptr>::iterator itr = _conns.begin();
	for(; itr != _conns.end(); itr++)
	{
		(*itr)->connect();
	}
}




void Conn_Mgt_LB::check_mdp_conn()
{
	int nRet = 0;
	
	Thread_Mutex_Guard guard(_mutex);
	
	std::vector<Conn_Ptr>::iterator itr = _conns.begin();
	for(; itr != _conns.end(); itr++)
	{
		if((*itr)->is_close())
		{
			//连接
			(*itr)->connect();	
		}
		else
		{
			//如果已经注册就发hb
			if(!((*itr)->_registered))
			{
				//注册到mdp
				nRet = (*itr)->register_mdp();
				if(nRet != 0)
				{
					//注册失败关闭连接，准备重新连接
					(*itr)->close();
				}
			}
			
		}
		
	}

}



void Conn_Mgt_LB::check_lb_conn()
{
	int nRet = 0;
	
	Thread_Mutex_Guard guard(_mutex);
	
	std::vector<Conn_Ptr>::iterator itr = _conns.begin();
	for(; itr != _conns.end(); itr++)
	{
		if((*itr)->is_close())
		{
			//连接
			(*itr)->connect();	
		}
		else
		{
			//如果已经注册就发hb
			if((*itr)->_registered)
			{
				//心跳
				nRet = (*itr)->hb_lb();
				if(nRet != 0)
				{
					//心跳失败就关闭连接，准备重新连接
					(*itr)->close();
				}			
			}
			else
			{
				//注册worker
				nRet = (*itr)->register_lb();
				if(nRet != 0)
				{
					//注册失败关闭连接，准备重新连接
					(*itr)->close();
				}
			}
			
		}
		
	}

}



bool Conn_Mgt_LB::get_conn(Conn_Ptr &conn)
{
	Thread_Mutex_Guard guard(_mutex);

	if(_conns.empty())
	{
		XCP_LOGGER_ERROR(&g_logger, "conn is empty.\n");
		return false;
	}

	if(_index >= _conns.size())
	{
		_index = 0;
	}
	
	for(; _index<_conns.size(); _index++)
	{
		conn = _conns[_index];
		if(!conn->is_close())
		{
			return true;
		}
	}	

	XCP_LOGGER_ERROR(&g_logger, "no conn is found.\n");
	return false;

}




bool Conn_Mgt_LB::get_conn(const std::string &id, Conn_Ptr &conn)
{
	int bRet = true;

	Thread_Mutex_Guard guard(_mutex);

	if(_conns.empty())
	{
		XCP_LOGGER_ERROR(&g_logger, "no conn is found.\n");
		return false;
	}

	std::vector<Conn_Ptr>::iterator itr = _conns.begin();
	for(; itr != _conns.end(); itr++)
	{
		std::string _id = format("%s_%u", (*itr)->_svr._ip.c_str(), (*itr)->_svr._port);
		if(_id == id)
		{
			conn = *itr;
			if(conn->is_close())
			{
				XCP_LOGGER_INFO(&g_logger, "the conn is closed, id:%s\n", id.c_str());
				return false;
			}
			return true;
		}
	}

	return false;

}




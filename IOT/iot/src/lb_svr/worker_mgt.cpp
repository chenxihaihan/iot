
#include "worker_mgt.h"
#include "comm/error_code.h"
#include "base/base_logger.h"

extern Logger g_logger;

Worker_Mgt::Worker_Mgt(): _index(0)
{

}


Worker_Mgt::~Worker_Mgt()
{

}


int Worker_Mgt::register_worker(Worker_Ptr worker)
{
	int nRet = 0;

	Thread_Mutex_Guard guard(_mutex);
	
	//����_workers
	std::set<std::string>::iterator itr_worker = _ids.find(worker->_new_id);
	if(itr_worker != _ids.end())
	{
		//id �Ѿ�����
		XCP_LOGGER_ERROR(&g_logger, "the worker id is exist(%s)\n", worker->to_string().c_str());
		return ERR_WORKER_ID_EXIST;		
	}
	else
	{
		//id ������
		
		//����_fds
		std::map<int, Worker_Ptr>::iterator itr_fd = _fds.find(worker->_fd);	
		if(itr_fd != _fds.end())
		{
			//fd �Ѿ�����
			XCP_LOGGER_ERROR(&g_logger, "the worker fd is exist(%s)\n", worker->to_string().c_str());
			return ERR_WORKER_FD_EXIST;		
		}
		else
		{
			//fd ������
			_fds.insert(std::make_pair(worker->_fd, worker));
			_ids.insert(worker->_new_id);
			_workers.push_back(worker);
		}
		
	}

	return nRet;
	
}




void Worker_Mgt::unregister_worker(Worker_Ptr worker)
{
	Thread_Mutex_Guard guard(_mutex);
	do_unregister_worker(worker);
}



bool Worker_Mgt::get_worker(Worker_Ptr &worker)
{
	int bRet = true;

	Thread_Mutex_Guard guard(_mutex);
	
	if(_workers.empty())
	{
		XCP_LOGGER_ERROR(&g_logger, "no worker is found.\n");
		return false;
	}

	if(_index >= _workers.size())
	{
		_index = 0;
	}

	//����
	worker = _workers[_index++];
	
	return bRet;
	
}




bool Worker_Mgt::get_worker_new(Worker_Ptr &worker)
{
	int bRet = true;

	Thread_Mutex_Guard guard(_mutex);

	if(_workers.empty())
	{
		XCP_LOGGER_ERROR(&g_logger, "no worker is found.\n");
		return false;
	}
	else if(_workers.size() == 1)
	{
		worker = _workers[0];
	}
	else
	{
		unsigned int index = 0;
		for(unsigned int i=0; (i<_workers.size()-1); i++)
		{
			//ѡ����ͻ��������������ٵ�message svr
			if((_workers[i]->_client_num) < (_workers[i+1]->_client_num))
			{
				index = i;
			}
			else
			{
				index = i+1;
			}
		}
		worker = _workers[index];
	}

	return bRet;
	
}




bool Worker_Mgt::get_worker(int fd, Worker_Ptr &worker)
{
	bool bRet = true;

	Thread_Mutex_Guard guard(_mutex);
	
	std::map<int, Worker_Ptr>::iterator itr = _fds.find(fd);
	if(itr != _fds.end())
	{
		worker = itr->second;
	}
	else
	{
		return false;
	}
	
	return bRet;

}



void Worker_Mgt::check()
{
	Thread_Mutex_Guard guard(_mutex);
	
	unsigned long long cur_time = getTimestamp();
	std::vector<Worker_Ptr>::iterator itr_worker = _workers.begin();
	for(; itr_worker != _workers.end(); )
	{
		//20 ���ڻ�û���յ�����������Ϊ�Զ��Ѿ���ͨ
		if((*itr_worker)->_hb_stmp + 20 <= cur_time)
		{
			XCP_LOGGER_ERROR(&g_logger, "hb isn't received from worker. worker(%s) is close possibly, hb_stmp:%llu, cur_time:%llu\n", 
				(*itr_worker)->to_string().c_str(), (*itr_worker)->_hb_stmp, cur_time);

			int fd = (*itr_worker)->_fd;
			std::map<int, Worker_Ptr>::iterator itr_fd = _fds.find((*itr_worker)->_fd);	
			if(itr_fd != _fds.end())
			{
				//fd �Ѿ�����
				_fds.erase(itr_fd);
				
				//ɾ��_ids
				std::set<std::string>::iterator itr_id = _ids.find((*itr_worker)->_new_id);
				if(itr_id != _ids.end())
				{
					//id����
					_ids.erase(itr_id);
				}
				
			}

			XCP_LOGGER_ERROR(&g_logger, "===unregister worker1(%s)\n", (*itr_worker)->to_string().c_str());
			
			//ɾ��_workers
			itr_worker = _workers.erase(itr_worker);
			/*
			��ʵ�����Ĺر�fd������һ�������⣬epoll_wait�е�fd û�б��޳���
			����Ҫ����fd ��Ӧ��event_handlerû���ͷ�
			*/
			::close(fd);
			
		}
		else
		{
			++itr_worker;
		}

	}
	
}



void Worker_Mgt::do_unregister_worker(Worker_Ptr worker)
{
	
	std::map<int, Worker_Ptr>::iterator itr_fd = _fds.find(worker->_fd);	
	if(itr_fd != _fds.end())
	{
		//fd �Ѿ�����
		
		//ɾ��_ids
		std::set<std::string>::iterator itr_id = _ids.find(itr_fd->second->_new_id);
		if(itr_id != _ids.end())
		{
			//id����
			_ids.erase(itr_id);
		}
		else
		{
			XCP_LOGGER_ERROR(&g_logger, "id(%s) isn't found.\n", (itr_fd->second->_new_id).c_str());
		}

		//ɾ��_workers
		std::vector<Worker_Ptr>::iterator itr_worker = _workers.begin();
		for(; itr_worker != _workers.end(); )
		{
			if(worker->_fd == (*itr_worker)->_fd)
			{
				XCP_LOGGER_ERROR(&g_logger, "===unregister worker(%s)\n", (*itr_worker)->to_string().c_str());

				itr_worker = _workers.erase(itr_worker);

				break;
			}
			else
			{
				++itr_worker;
			}
		}
	
		_fds.erase(itr_fd);
		
	}
	else
	{
		//fd ������
		XCP_LOGGER_ERROR(&g_logger, "fd(%d) isn't exist\n", worker->_fd);	
	}

}



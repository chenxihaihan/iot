
#include "base_kv_parser.h"
#include "base_string.h"

NS_BASE_BEGIN


KV_Parser::KV_Parser()
{

}


KV_Parser::~KV_Parser()
{

}

	
//msg��ת�����ַ���
//&&key1=value1&key2=value2&key2=value&&
void KV_Parser::msg(const std::string &str)
{
	_mapKV.clear();

	std::string msg = str;
	trim(msg, '&');
	if(msg.empty())
	{
		printf("str(size:%d) is empty.\n", str.size());
		return;
	}
	
	std::string::size_type pre_pos = 0;
	std::string::size_type pos = msg.find("&");
	while(pos != std::string::npos)
	{
		std::string pair = msg.substr(pre_pos, pos-pre_pos);
			
		pre_pos = pos+1;

		//����key=value
		std::string::size_type pre_tag = 0;
		std::string::size_type tag = pair.find("=");
		if(tag != std::string::npos)
		{
			std::string key = pair.substr(pre_tag, tag-pre_tag);
			trim(key);
			if(key.empty())
			{
				printf("key is empty. pair:%s\n", pair.c_str());
			}
			else
			{	
				std::string value = "";
				if(tag != pair.size()-1)
				{
					value = pair.substr(tag+1, (pair.size()-tag-1));
					trim(value);
				}
				_mapKV.insert(std::make_pair(key, value));
			}
		}
		else
		{
			//printf("pair is invalid. pair:%s\n", pair.c_str());
		}

		if(pos < msg.size()-1)
		{
			//Ѱ����һ��&
			pos = msg.find("&", pos+1);
		}
		
	}


	//�������һ��key=value
	if(pre_pos < msg.size()-1)
	{
		std::string pair = msg.substr(pre_pos, (msg.size()-pre_pos+1));
		
		//����key=value
		std::string::size_type pre_tag = 0;
		std::string::size_type tag = pair.find("=");
		if(tag != std::string::npos)
		{
			std::string key = pair.substr(pre_tag, tag-pre_tag);
			trim(key);
			if(key.empty())
			{
				printf("key is empty. pair:%s\n", pair.c_str());
			}
			else
			{	
				std::string value = "";
				if(tag != pair.size()-1)
				{
					value = pair.substr(tag+1, (pair.size()-tag-1));
					trim(value);
				}
				_mapKV.insert(std::make_pair(key, value));
			}

		}
		else
		{
			//printf("pair is invalid. pair:%s\n", pair.c_str());
		}		
	
	}

}




std::string KV_Parser::new_msg(std::string tail)
{
	std::string msg = "";

	std::string pair = "";
	std::string key = "";
	std::string value = "";
	std::map<std::string, std::string>::iterator itr = _mapKV.begin();
	for(; itr != _mapKV.end(); ++itr)
	{
		key = itr->first;
		value = itr->second;
		
		pair = key + std::string("=") + value;

		if(itr == _mapKV.begin())
		{
			msg = pair;
		}
		else
		{
			msg += (std::string("&")+pair); 
		}
	}

	if(!tail.empty() & !msg.empty())
	{
		msg += tail;
	}
	
	return msg;

}


void KV_Parser::clear()
{
	_mapKV.clear();
}


//��ȡ�����Ƿ�ת���value�����õ��Ƿ�ת���value
std::string& KV_Parser::operator [](const std::string &key)
{
	std::map<std::string, std::string>::iterator itr = _mapKV.find(key);
	if(itr == _mapKV.end())
	{
		std::string value = "";
		std::pair<std::map<std::string, std::string>::iterator, bool> ret = 
			_mapKV.insert(std::make_pair(key, value));

		itr = ret.first;
	}
	
	return itr->second;

}


void KV_Parser::show()
{
	std::map<std::string, std::string>::iterator itr = _mapKV.begin();
	for(; itr != _mapKV.end(); ++itr)
	{
		printf("key:%s, value:%s\n", itr->first.c_str(), itr->second.c_str());
	}
}


NS_BASE_END



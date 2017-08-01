
#include "base_tail.h"
#include "base_fs.h"
#include "base_time.h"

NS_BASE_BEGIN

Tail_Handler::Tail_Handler()
{

}

Tail_Handler::~Tail_Handler()
{

}

int Tail_Handler::process(const std::string &line)
{
	printf("line(%u):%s\n", line.size(), line.c_str());
	return 0;
}


void Tail_Handler::complete()
{
	printf("file is over!\n");
}



//---------------------------

X_Tail::X_Tail(const std::string &file, const std::string &ending, Tail_Handler *handler, bool truncate_mode):
	_file(file), _ending(ending), _handler(handler), _pre_size(0), 
	_truncate_mode(truncate_mode), _truncated(false)
{

}


X_Tail::~X_Tail()
{
	printf("destroy x-tail!\n");
	DELETE_POINTER(_handler);
}


int X_Tail::do_init(void *args)
{
	if(!isFile(_file))
	{
		return -1;
	}

	return 0;
}


int X_Tail::svc()
{
	int nRet = 0;
	
	unsigned long long size = 0;
	nRet = getFileSize_stat(_file, size);
	if(nRet != 0)
	{
		printf("getFileSize_stat(%s) failed, ret:%d\n", _file.c_str(), nRet);
		return 0;
	}

	//printf("size:%llu, _pre_size:%llu\n", size, _pre_size);
	
	if(size == _pre_size)
	{
		//��ǰ�ļ�û��׷����Ϣ���Ѿ������ļ�ĩβ��Ҫ���´��ļ�������ȡ
		//printf("file(%s) isn't appended!\n", _file.c_str());
		sleep(10000, true); //ͣ��10����
		return 0;
	}
	else if(size < _pre_size)
	{
		if(_truncate_mode)
		{
			//��ʾ�ļ����ض���
			_truncated = true;
			_pre_size = size;
			printf("file(%s) is truncated.\n", _file.c_str());
		}
		else
		{
			//��ʾ�ļ����½���
			_pre_size = 0;
			printf("file(%s) is new one.\n", _file.c_str());
		}
	}
	else
	{
		//no todo
	}
	
	_fin.open(_file.c_str(), std::ios::in);
	if(_fin.is_open())
	{
		if(_truncated)
		{
			/*
			�ڽضϺ����´��ļ���϶���ļ����ܻ��ڳ���
			д�����ʱ�����´��ļ����Ҵ�ĩβ��ȡ�ļ�
			������ٶ��������ݵ������
			*/
			_fin.seekg(0, std::ios::end);
			_truncated = false;
		}
		else
		{
			//�����һ�ζ�ȡ�ļ�λ��Ϊ��׼�������ļ�
			_fin.seekg(_pre_size, std::ios::cur);
		}
		
		//��ȡ���Ҵ����ļ�
		std::string line = "";
		while(std::getline(_fin, line))
		{
			if(line == "")
			{
				continue;
			}
				
			if(line == _ending)
			{
				//�ļ�����
				_handler->complete();
				_fin.close();
				return -1;
			}

			//��������
			nRet = _handler->process(line);
			if(nRet != 0)
			{
				//���ҵ���Լ����ط�0�� ��ʾ���ٶ��ļ�����tail ����
				_fin.close();
				return nRet;
			}
			
			/*
			�ļ�ÿ����0D0A ����(0D: �س�\r;  0A: ����\n)
			std::getline ��ȡ������Ϣ��\n ȥ��������ͨ��
			std::getline �����ļ��ߴ��ʱ����Ҫ��1
			*/
			_pre_size += (line.size() + 1);
			
		}

		//�����ļ�ĩβ
		_fin.close();
		
	}
	else
	{
		//�ļ���ʧ�ܺ�Ҫ�������Դ򿪺Ͷ�ȡ�ļ�
		printf("open file(%s) failed.\n", _file.c_str());
	}

	return 0;

}


NS_BASE_END


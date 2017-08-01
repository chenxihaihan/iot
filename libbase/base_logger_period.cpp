
#include "base_logger_period.h"
#include "base_os.h"
#include "base_time.h"
#include "base_fs.h"

#include <stdarg.h>

NS_BASE_BEGIN

Logger_Period_Thread::Logger_Period_Thread(X_Queue<std::string> *queue,
		std::string log_path, std::string service_name, 
		EnLogPeriodMode period_mode, unsigned int period_value, unsigned long long max_size, unsigned int buf_sec, 
		std::string ip, unsigned short port): m_fp(NULL), m_open(false),
			m_log_path(log_path), m_log_file(""), m_ip(ip), m_port(port), m_service_name(service_name),
			m_pid(0), m_max_size(max_size), m_buf_sec(buf_sec), m_seq(0), 
			m_pre_buf_timestamp(0), m_period_mode(period_mode), m_period_value(period_value), m_pre_period_index(0), 
			m_queue(queue), m_log_msg("")
{
	m_pid = get_pid();

	//���ó�ʼperiod index
	get_tm();
	m_pre_period_index = get_period_index();
}



Logger_Period_Thread::~Logger_Period_Thread()
{
	close();
}



int Logger_Period_Thread::svc()
{
	check_log();
	
	std::string log_msg = "";
	int nRet = m_queue->pop(log_msg, 1000);
	if(nRet == 0)
	{
		//׷����־
		m_log_msg += log_msg;
	}

	//��ȡ��ǰʱ���
	unsigned long long timestamp = getTimestamp();

	//���log����ߴ����LOG_BUF_MAX_SIZE �Ϳ�ʼдlog	
	if(m_log_msg.size() >= 2*1024*1024)
	{
		if(m_fp)
		{
			fprintf(m_fp, "%s", m_log_msg.c_str());
		}
		else
		{
			//���·ŵ����к����ֹ��־��ʧ
			m_queue->push(m_log_msg);
		}
		
		m_log_msg = "";
		m_pre_buf_timestamp = timestamp;
		return 0;
	}

	//�����ǰʱ�䳬������־��������ʱ�䣬 ��������־��Ϣ��ʼдlog
	if(m_log_msg.size() > 0 && ((timestamp-m_pre_buf_timestamp) > m_buf_sec))
	{
		if(m_fp)
		{
			fprintf(m_fp, "%s", m_log_msg.c_str());
		}
		else
		{
			//���·ŵ����к����ֹ��־��ʧ
			m_queue->push(m_log_msg);
		}
		m_log_msg = "";
		m_pre_buf_timestamp = timestamp;
	}

	return 0;
	
}



int Logger_Period_Thread::do_init(void *args)
{
	//��ʼ���ȴ�����־�ļ�
	return open();
}



int Logger_Period_Thread::open()
{
	int nRet = 0;

	if(m_open)
	{
		printf("log file(%s) is open already.\n", m_log_file.c_str());
		return -3;
	}
	

	//--------------------------------

	//�жϺ�ȷ����һ�������ļ�(һ����������ഴ��100�����ļ�)
	//�ҵ���һ�������ڵ���־�ļ���ʼ��������ǰ��û���ļ�������
	//�ļ������������Щ���⡣
	bool check = false;
	for(; m_seq<100; ++m_seq)
	{
		//����log name
		char szLog[200] = {0};
		snprintf(szLog, 200, "%s/%s.%s.%02u.log", 
			m_log_path.c_str(), m_service_name.c_str(), get_period_date().c_str(), m_seq);
		m_log_file = szLog;

		m_fp = fopen(m_log_file.c_str(), "a+");
		if(m_fp == NULL)
		{
			printf("fopen(%s) failed. errno:%d, errmsg:%s\n", m_log_file.c_str(), errno, strerror(errno));
			return -1;
		}
		else
		{
			//�򿪳ɹ��������´�����һ���ļ�Ҳ������ԭ�����ļ�
			printf("fopen(%s) success.\n", m_log_file.c_str());

			unsigned long long size = 0;
			nRet = getFileSize_stat(m_log_file, size);
			if(nRet == 0)
			{
				//�ж�log�ļ��ߴ��Ƿ񳬹����ֵ
				if(size >= (m_max_size*1024*1024))
				{
					printf("the size(%lluB) of log file(%s) is bigger than max size(%dM).\n", 
						size, m_log_file.c_str(), m_max_size);
					close();
					continue;
				}
			}

			check = true;
			break;
		
		}
		
	}

	if(!check)
	{
		printf("check all file failed.\n");
		m_seq = 0; //���¿�ʼ
		return -1;		
	}

	//--------------------------------
	

	//����Ҫlibc ���壬 ֱ�ӽ����ں˻���
	setbuf(m_fp, NULL);
		 
	m_open = true;

	/*
	//��ӡ�ļ�ͷ��Ϣ
	fprintf(m_fp, "Log file created at:%s\n", FormatDateTimeStr(0, "%Y%m%d%H%M%S").c_str());
	fprintf(m_fp, "Service:%s, Machine:%s:%d, Process:%d\n", 
		m_service_name.c_str(), m_ip.c_str(), m_port, m_pid);
	fprintf(m_fp, "IWEF|yyyymmdd hhmmss.uuuuuu|threadid|file:line| msg\n\n");
	*/
	
	return nRet;

}


void Logger_Period_Thread::close()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;

		m_open = false;
	}
}


/*
��������: û������ֵ�Ƚ��жϡ�
����: ÿ��5����
*/
unsigned int Logger_Period_Thread::get_period_index()
{	
	unsigned int period = 0;
	switch(m_period_mode)
	{	
		case LP_MIN:
		{
			period = m_tm.tm_min;
			break;
		}
		case LP_HOUR:
		{
			period = m_tm.tm_hour;
			break;
		}
		case LP_DAY:
		{
			period = m_tm.tm_mday;
			break;
		}
		case LP_MONTH:
		{
			period = m_tm.tm_mon;
			break;
		}			
		default:
		{
			period = m_tm.tm_hour;
			break;
		}
		
	}

	//��ǰ������ֵ����Ԥ������ֵ������������0��ʼ
	return period/m_period_value;

}




std::string Logger_Period_Thread::get_period_date()
{

	char period_date[100] = {0};
	unsigned int period_postfix = m_pre_period_index * m_period_value;

	switch(m_period_mode)
	{	
		case LP_MIN:
		{
			snprintf(period_date, 100, "%d%02d%02d%02d%02d", 
				m_tm.tm_year+1900, m_tm.tm_mon+1, m_tm.tm_mday, m_tm.tm_hour, period_postfix);
			break;
		}
		case LP_HOUR:
		{
			snprintf(period_date, 100, "%d%02d%02d%02d", 
				m_tm.tm_year+1900, m_tm.tm_mon+1, m_tm.tm_mday, period_postfix);
			break;
		}
		case LP_DAY:
		{
			snprintf(period_date, 100, "%d%02d%02d", 
				m_tm.tm_year+1900, m_tm.tm_mon+1, period_postfix);
			break;
		}
		case LP_MONTH:
		{
			snprintf(period_date, 100, "%d%02d", 
				m_tm.tm_year+1900, period_postfix);
			break;
		}			
		default:
		{
			//no-todo
		}
		
	}

	return period_date;

}




void Logger_Period_Thread::check_log()
{
	int nRet = 0;

	//��ȡ��ǰʱ��tm ��Ϣ
	get_tm();
		
	unsigned long long size = 0;
	nRet = getFileSize_stat(m_log_file, size);
	if(nRet != 0)
	{
		/*
		������������һ�㶼������log ���ⲿɾ����
		��Ҫ���´����µ�log�ļ�
		*/
		printf("getFileSize_stat failed, ret:%d\n", nRet);
		
		//���´���log �ļ�
		m_open = false;
		nRet = open();
		if(nRet != 0)
		{
			printf("open failed, ret:%d\n", nRet);
			return;
		}
		size = 0;
	}

	bool bMove = false;
	
	//�ж�log�ļ��ߴ��Ƿ񳬹����ֵ
	if(size >= (m_max_size*1024*1024))
	{
		printf("the size(%lluB) of log file is bigger than max size(%dM).\n", size, m_max_size);
		bMove = true;
	}

	//�ж��Ƿ�ʱ���Ѿ��仯
	unsigned int period_index = get_period_index();
	if(m_pre_period_index != period_index)
	{
		printf("period mode:%u, pre period index:%u, cur period index:%u\n", m_period_mode, m_pre_period_index, period_index);
		m_pre_period_index = period_index;
		bMove = true;
		m_seq = 0;
	}
	
	if(bMove)
	{
		//�ر�ԭ�����ļ����
		close();

		//���´����ļ�
		nRet = open();
		if(nRet != 0)
		{
			printf("open failed, ret:%d\n", nRet);
		}

		printf("--- complete to backup old log and renew log ---\n");
		
	}
	
}




void Logger_Period_Thread::get_tm()
{
	/*
	time_t time(time_t *t); 
	����:���ش�1970-01-01 00:00:00 +0000 (UTC) ��Ŀǰ������
	���t��ΪNULL �� ���ص�ֵҲ��洢��t ִ�пռ���
	*/
	unsigned long long timestamp = 0;
	timestamp = time(NULL);
	if((time_t)timestamp == (time_t)-1)
	{
		printf("time failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
	}	
			
	/*
	struct tm *localtime_r(const time_t *timep, struct tm *result);
	����: ��time_t (ʱ���) ת����tm��ʽ(����ʱ��)
	����ɹ�����ָ��ͬresult һ��
	���򷵻�NULL
	
	��localtime_r�������gmtime_r�� ���ص���UTCʱ��(��ͬGMTʱ��--��������ʱ��)
	struct tm *gmtime_r(const time_t *timep, struct tm *result); 
	*/
	struct tm *ptm = NULL;
	ptm = localtime_r((time_t*)&timestamp, &m_tm);
	if(ptm == NULL)
	{
		printf("localtime_r failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
	}

}




//------------------------------------------

Logger_Period::Logger_Period(): m_queue(NULL), m_thread(NULL), m_init(false), m_close(false)
{

}


Logger_Period::~Logger_Period()
{

}


int Logger_Period::init(std::string log_path, std::string service_name, 
		 EnLogPeriodMode period_mode, unsigned int period_value, unsigned long long max_size, unsigned int buf_sec,
		 std::string ip, unsigned short port)
{
	int nRet = 0;

	if(m_init)
	{
		printf("Logger is init already.\n");
		return -1;
	}

	if(log_path.empty())
	{
		printf("log_path is empty.\n");
		return -1;
	}

	if(!isDirectory(log_path))
	{
		printf("%s isn't not directory.\n", log_path.c_str());
		return -1;		
	}

	//���÷�������
	if(service_name == "")
	{
		service_name = "test";
	}
	

	//�ж���־������Ϣ
	switch(period_mode)
	{	
		case LP_MIN:
		{
			if((period_value > 59) || (period_value == 0))
			{
				printf("invlide period info, period mode:%d, period value:%d\n", period_mode, period_value);
				return -1;
			}
			break;

		}
		case LP_HOUR:
		{
			if((period_value > 23) || (period_value == 0))
			{
				printf("invlide period info, period mode:%d, period value:%d\n", period_mode, period_value);
				return -1;
			}
			break;
		}
		case LP_DAY:
		{
			if((period_value > 31) || (period_value == 0))
			{
				printf("invlide period info, period mode:%d, period value:%d\n", period_mode, period_value);
				return -1;
			}
			break;
		}
		case LP_MONTH:
		{
			if((period_value > 12) || (period_value == 0))
			{
				printf("invlide period info, period mode:%d, period value:%d\n", period_mode, period_value);
				return -1;
			}
			break;
		}			
		default:
		{
			printf("invlide period mode, period mode:%d, period value:%d\n", period_mode, period_value);
			return -1;
		}
		
	}
	

	//������־���ߴ磬��λ: M
	if(max_size > MAX_LOG_SIZE)
	{
		//max_size = MAX_LOG_SIZE;
	}
	else if(max_size < MIN_LOG_SIZE)
	{
		max_size = MIN_LOG_SIZE;
	}
	else
	{
		//no to-do
	}

	//������־���泬ʱʱ��
	if(buf_sec == 0)
	{
		buf_sec = 1;
	}


	//����ip
	if(ip == "")
	{
		ip = "127.0.0.1";
	}
	
	//����log ����
	m_queue = new X_Queue<std::string>(1000000);

	//����logger thread
	m_thread = new Logger_Period_Thread(m_queue, log_path, service_name, 
		period_mode, period_value, max_size, buf_sec, ip, port);
	
	nRet = m_thread->init();
	if(nRet != 0)
	{
		printf("Logger_Thread init failed, ret:%d\n", nRet);
		return nRet;
	}
	
	nRet = m_thread->run(NULL);
	if(nRet != 0)
	{
		printf("Logger_Thread run failed, ret:%d\n", nRet);
		return nRet;
	}

	m_init = true;
	
	return nRet;
	
}



int Logger_Period::log_info(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("I", file, line, format, args);

	va_end(args);

	return nRet;

}

int Logger_Period::log_warning(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("W", file, line, format, args);

	va_end(args);

	return nRet;

}

int Logger_Period::log_error(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("E", file, line, format, args);

	va_end(args);

	return nRet;

}

int Logger_Period::log_fatal(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("F", file, line, format, args);

	va_end(args);

	return nRet;

}



void Logger_Period::close()
{
	DELETE_POINTER(m_queue);

	m_thread->close();
	m_thread->stop();
}



int Logger_Period::do_log(const char *level, const char* file, int line, const char *format, va_list args)
{
	int nRet = 0;
	
	if(!m_init)
	{
		printf("logger isn't init.\n");
		return -1;
	}
	
	//��ӡӦ����־��ʹ��vsnprintf
	char log_msg[MAX_LOG_LEN] = {0};
	vsnprintf(log_msg, MAX_LOG_LEN, format, args);

	std::string log = "";
	log = log_msg;
	
	m_queue->push(log);
	
	return nRet;

}
	


NS_BASE_END




#include "base_logger.h"
#include "base_os.h"
#include "base_time.h"
#include "base_fs.h"

#include <stdarg.h>

NS_BASE_BEGIN


Logger_Thread::Logger_Thread(X_Queue<std::string> *queue,
		std::string log_path, std::string service_name, 
		unsigned int max_size, unsigned long long max_sec, unsigned int buf_sec, 
			std::string ip, unsigned short port): m_fp(NULL), m_open(false),
			m_log_path(log_path), m_log_file(""), m_ip(ip), m_port(port), m_service_name(service_name),
			m_pid(0), m_max_sec(max_sec), m_max_size(max_size), m_buf_sec(buf_sec), m_pre_timestamp(0),
			m_create_date(""), m_cur_timestamp(0), m_queue(queue), m_log_msg("")
{
	m_pid = get_pid();
	m_pre_timestamp = getTimestamp();

	//����log name
	char szLog[200] = {0};
	snprintf(szLog, 200, "%s/%s.log", m_log_path.c_str(), m_service_name.c_str());
	m_log_file = szLog;
	
}



Logger_Thread::~Logger_Thread()
{
	close();
}



int Logger_Thread::svc()
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

	//���log����ߴ����512KB �Ϳ�ʼдlog	
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
		m_cur_timestamp = timestamp;
		return 0;
	}

	//�����ǰʱ�䳬������־��������ʱ�䣬 ��������־��Ϣ��ʼдlog
	if(m_log_msg.size() > 0 && ((timestamp-m_cur_timestamp) >= m_buf_sec))
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
		m_cur_timestamp = timestamp;
	}

	return 0;
	
}



int Logger_Thread::do_init(void *args)
{
	return open();
}



int Logger_Thread::open()
{
	int nRet = 0;

	if(m_open)
	{
		printf("log file(%s) is open already.\n", m_log_file.c_str());
		return -3;
	}

	m_fp = fopen(m_log_file.c_str(), "a+");
	if(m_fp == NULL)
	{
		printf("fopen(%s) failed. errno:%d, errmsg:%s\n",
			m_log_file.c_str(), errno, strerror(errno));
		return -1;
	}
	else
	{
		printf("fopen(%s) success.\n", m_log_file.c_str());
	}

	//����Ҫlibc ���壬 ֱ�ӽ����ں˻���
	setbuf(m_fp, NULL);

	m_create_date = FormatDateTimeStr(0, "%Y%m%d%H%M%S");
		 
	m_open = true;

	//��ӡ�ļ�ͷ��Ϣ
	fprintf(m_fp, "====================================================\n");
	fprintf(m_fp, "Log file created at:%s\n", m_create_date.c_str());
	fprintf(m_fp, "Service:%s, Machine:%s:%d, Process:%d\n", 
		m_service_name.c_str(), m_ip.c_str(), m_port, m_pid);
	fprintf(m_fp, "IWEF|yyyymmdd hhmmss.uuuuuu|threadid|file:line| msg\n");
	fprintf(m_fp, "====================================================\n");
	
	return nRet;

}


void Logger_Thread::close()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;

		m_open = false;
	}
}



void Logger_Thread::check_log()
{
	int nRet = 0;
	
	unsigned long long size = 0;
	nRet = getFileSize_stat(m_log_file, size);
	if(nRet != 0)
	{
		printf("getFileSize_stat failed, ret:%d\n", nRet);
		
		//���´����ļ�
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

	//�ж��Ƿ�ʱ���Ѿ��������ʱ������
	if(m_max_sec)
	{
		unsigned long long timestamp = getTimestamp();
		if(timestamp-m_pre_timestamp >= m_max_sec)
		{
			m_pre_timestamp = timestamp;
			bMove = true;
		}
	}

	if(bMove)
	{
		//�ر�ԭ�����ļ����
		close();

		//����ԭ������־�ļ��������޸ı�����־����
		std::string end_date = FormatDateTimeStr(0, "%Y%m%d%H%M%S");
		
		char szBackupLog[200] = {0};
		snprintf(szBackupLog, 200, "%s/%s_%s_%s.log", m_log_path.c_str(), m_service_name.c_str(), 
			m_create_date.c_str(), end_date.c_str());

		printf("start to backup old log(%s~%s) and renew log(%s) ....\n",
			m_create_date.c_str(), end_date.c_str(), szBackupLog);

		//�ƶ��ļ�	
		nRet = moveFile(m_log_file, szBackupLog);
		if(nRet != 0)
		{
			printf("moveFile failed, ret:%d\n", nRet);
			return;
		}

		//���´����ļ�
		nRet = open();
		if(nRet != 0)
		{
			printf("open failed, ret:%d\n", nRet);
		}

		printf("--- complete to backup old log and renew log ---\n");
		
	}
	
}





//------------------------------------------

Logger::Logger(): m_queue(NULL), m_thread(NULL), m_init(false), m_close(false)
{

}


Logger::~Logger()
{

}


int Logger::init(std::string log_path, std::string service_name, 
		 unsigned int max_size, unsigned long long max_sec, unsigned int buf_sec,
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
		return -2;		
	}

	//���÷�������
	if(service_name == "")
	{
		service_name = "test";
	}
	

	//������־���ߴ磬��λ: M
	if(max_size > MAX_LOG_SIZE)
	{
		max_size = MAX_LOG_SIZE;
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
		buf_sec = LOG_BUF_SECS;
	}


	//����ip
	if(ip == "")
	{
		ip = "127.0.0.1";
	}
	
	//����log ����
	m_queue = new X_Queue<std::string>(1000000);

	//����logger thread
	m_thread = new Logger_Thread(m_queue, log_path, service_name, 
		max_size, max_sec, buf_sec, ip, port);
	
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



int Logger::log_info(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("I", file, line, format, args);

	va_end(args);

	return nRet;

}

int Logger::log_warning(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("W", file, line, format, args);

	va_end(args);

	return nRet;

}

int Logger::log_error(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("E", file, line, format, args);

	va_end(args);

	return nRet;

}

int Logger::log_fatal(const char* file, int line, const char *format, ...)
{
	int nRet = 0;
	
	va_list args;
   	va_start(args, format);

	nRet = do_log("F", file, line, format, args);

	va_end(args);

	return nRet;

}


void Logger::close()
{
	DELETE_POINTER(m_queue);

	m_thread->close();
	m_thread->stop();
}



int Logger::do_log(const char *level, const char* file, int line, const char *format, va_list args)
{
	int nRet = 0;

	if(!m_init)
	{
		//printf("logger isn't init.\n");
		return -1;
	}
	
	unsigned long long sec = 0;
	unsigned long long usec = 0;
	getTimeOfDay(sec, usec);
	std::string date = FormatDateTimeStr(sec, "%Y%m%d %H%M%S");

	std::string log = "";
	//��ӡ��ͷ��Ϣ
	char log_head[100] = {0};
	snprintf(log_head, 100, "%s|%s.%06llu|%lu|%s:%05d| ", level, date.c_str(), usec, 
		get_thread_id(), file, line);
	log += log_head;
		
	//��ӡӦ����־��ʹ��vsnprintf
	char log_msg[MAX_LOG_LEN] = {0};
	vsnprintf(log_msg, MAX_LOG_LEN, format, args);
	log += log_msg;
	//log += log_msg + std::string("\n");
	
	m_queue->push(log);
	
	return nRet;

}


NS_BASE_END



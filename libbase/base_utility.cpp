
#include "base_utility.h"
#include "base_close_helper.h"
#include "base_os.h"

#include <sys/stat.h>
#include <math.h>
#include <signal.h>
#include <execinfo.h>
#include "base_time.h"


NS_BASE_BEGIN

void init_daemon()
{
	printf("---- init_daemon ----\n");
	printf("---- father process ----\n");

	//������
	int pid = 0;
	if(pid == fork())
	{
		printf("---- exit father process ----\n");
		exit(0); //�������
	}
	else if(pid<0)
	{
		printf("fork child process failed.\n");
		exit(-1);
	}

	//�ӽ���
	printf("---- child process ----\n");
	
	//setsid �ӽ��̳�Ϊ�µĻ���鳤�ͽ����鳤������ԭ���Ŀ����ն˷���
	setsid();

	//chdir("/"); //�ı�����̹���·�� "/"
	
	umask(0);   //���������ļ�����������

	printf("---- complete to init_daemon ----\n");


};



void xprintf(const char* file, int line, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	unsigned long long sec = 0;
	unsigned long long usec = 0;
	getTimeOfDay(sec, usec);
	std::string date = FormatDateTimeStr(sec, "%Y/%m/%d %H:%M:%S");

	char buf[1024] = {0};
	snprintf(buf, 1024, "%s.%llu|%u:%lu|%s:%d|%s", 
		date.c_str(), usec, get_pid(), get_thread_id(), file, line, format);
	vprintf(buf, args);
		
	va_end(args);
}



void set_random_seed()
{
	srand((unsigned int)time(NULL));
}


//#include <math.h>
unsigned int get_random(unsigned int range)
{
	unsigned int num = 0;
	
	if(range == 0)
	{
		num = rand();
	}
	else
	{
		float result = ((float)rand()/(float)RAND_MAX)*((float)range);		
		num = (unsigned int)round(result);
	}
	
	return num;
	
}



int pid_file(const std::string &file)
{
	int nRet = 0;
	
	if(file == "")
	{
		printf("pid_file is invalid.\n");	
		return -1;
	}
	
	FILE *fp = NULL;
	fp = fopen(file.c_str(), "w");
	if(fp == NULL)
	{
		printf("fopen(%s) failed, errno:%d, errmsg:%s\n", file.c_str(), errno, strerror(errno));
 		return -1;
	}

	CloseHelper<FILE*> tmp(fp);
		
	/*
	fprintf д���棬 ��ִ��fclose(fp)��ʱ�����������������д��
	Ӳ�̣�write ���ֵͼ�io������û��д�����
	*/
	fprintf(fp, "%d,%llu", get_pid(), getTimestamp());
    
    return nRet;

}



NS_BASE_END



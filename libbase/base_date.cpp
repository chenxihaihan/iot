
#include "base_date.h"

NS_BASE_BEGIN

XDate::XDate(unsigned long long timestamp): _timestamp(timestamp)
{
	if(timestamp == 0)
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
		else
		{
			_timestamp = timestamp;
			update();
		}	
	}

}




XDate::XDate(const char *date, const char *format): _timestamp(0)
{

	/*
	#include <time.h>
	void tzset (void);

	3��ȫ�ֱ���:
	extern char *tzname[2];
	extern long timezone;
	extern int daylight

	tzset()����ʹ�û�������TZ�ĵ�ǰ���ð�ֵ��������ȫ�ֱ���:daylight,timezone��tzname
	*/
	tzset();

	/*
	char *strptime(const char *s, const char *format, struct tm *tm);
	����: ����format ��ʱ���ַ���ת����tm����
	����ɹ�����s�������ƥ�䴦����ַ���һ���ַ�(�������ַ���������\0)
	����������󷵻�NULL

	ע:  glibc �����ܽ�strptime ֧�ֵĸ�ʽ���ַ���ͬstrftime һ���࣬
	���ǻ�����Щ��ʽ���ַ�����(%Z %u $F ��) ��strptime�в�֧��
	
	����:
	strptime("2001-11-12 18:31:01 CST", "%Y-%m-%d %H:%M:%S %Z", &tm);
	*/
	struct tm ret_tm;
	if(strptime(date, format, &ret_tm) == NULL)
	{
		printf("strptime failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
	}
	else
	{
		/*
		time_t mktime(strcut tm * timeptr);
		����:  ��tm����ת����time_t(ʱ���)
		����ɹ����ض�Ӧʱ���
		���ʧ�ܷ���-1
		*/
		unsigned long long timestamp = 0;
		timestamp = mktime(&ret_tm);
		if((time_t)timestamp == (time_t)-1)
		{
			printf("mktime failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
		}
		else
		{
			_timestamp = timestamp;
			update();
		}
	}

}




void XDate::update()
{
	if(_timestamp)
	{
		/*
		struct tm 
		{
		   int tm_sec;      //seconds  0~59
		   int tm_min;      // minutes  0~59
		   int tm_hour;     //hours      0~23
		   int tm_mday;    //day of the month    1~31
		   int tm_mon;     //month     0~11
		   int tm_year;     //year       ��1900�����ڵ�����
		   int tm_wday;    //day of the week   0~6
		   int tm_yday;     //day in the year    0~365
		   int tm_isdst;     //daylight saving time   ������ʾ����ʱ��Ч; 
		                            0��ʾ����ʱ��Ч;
		                            ������ʾϵͳ��֧��
		};

		struct tm *localtime_r(const time_t *timep, struct tm *result);
		����: ��time_t (ʱ���) ת����tm��ʽ(����ʱ��)
		����ɹ�����ָ��ͬresult һ��
		���򷵻�NULL

		��localtime_r�������gmtime_r�� ���ص���UTCʱ��(��ͬGMTʱ��--��������ʱ��)
		struct tm *gmtime_r(const time_t *timep, struct tm *result); 
		
		*/
		struct tm *ptm = NULL;
		ptm = localtime_r((time_t*)&_timestamp, &_tm);
		if(ptm == NULL)
		{
			printf("localtime_r failed. errno:%d, errmsg:%s\n", errno, strerror(errno));
		}
			

	}
	
}



unsigned int XDate::year()
{
	return _tm.tm_year+1900;
}




unsigned int XDate::month()
{
	return _tm.tm_mon+1;
}



unsigned int XDate::day()
{
	return _tm.tm_mday;
}


unsigned int XDate::hour()
{
	return _tm.tm_hour;
}


unsigned int XDate::min()
{
	return _tm.tm_min;
}


unsigned int XDate::sec()
{
	return _tm.tm_sec;
}


unsigned int XDate::week()
{
	return _tm.tm_wday+1;
}


unsigned int XDate::yday()
{
	return _tm.tm_yday;
}


struct tm XDate::datails()
{
	return _tm;
}


std::string XDate::to_string(const char *format)
{
	/*
	struct size_t strftime(char *strDest, size_t maxsize, const char *format, const struct tm *timeptr);
	����: ����format ��tm����ת������Ӧ��ʱ���ַ�����ʽ
	����ɹ����ش������ַ���strDest���ַ��ĸ�����
	����������󷵻���
	*/
	char tmp[100] = {0};
	if(format == NULL)
	{
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S %Z", &_tm);
	}
	else
	{
		strftime(tmp, sizeof(tmp), format, &_tm);
	}
	
	return std::string(tmp);
	
}




unsigned long long XDate::timestamp()
{
	return _timestamp;
}


unsigned long long XDate::spend()
{
	return _timestamp;
}



//���ز�����
XDate XDate::operator+(const XDate &obj)
{
	XDate tmp;
	tmp._timestamp = this->_timestamp + obj._timestamp;
	tmp.update();
	
	return tmp;
}


XDate XDate::operator-(const XDate &obj)
{
	XDate tmp;
	tmp._timestamp = this->_timestamp - obj._timestamp;
	tmp.update();

	return tmp;
}



XDate XDate::operator+(unsigned long long sec)
{
	XDate tmp;
	tmp._timestamp = this->_timestamp + sec;
	tmp.update();
	
	return tmp;

}



XDate XDate::operator-(unsigned long long sec)
{
	XDate tmp;
	tmp._timestamp = this->_timestamp - sec;
	tmp.update();

	return tmp;
}



bool XDate::operator==(const XDate &obj)
{
	bool bRet = false;
	
	if(_timestamp == obj._timestamp)
	{
		bRet = true; 
	}
	
	return bRet;
}



bool XDate::operator!=(const XDate &obj)
{
	bool bRet = false;
	
	if(_timestamp != obj._timestamp)
	{
		bRet = true; 
	}
	
	return bRet;
}



bool XDate::operator<(const XDate &obj)
{
	bool bRet = false;
	
	if(_timestamp < obj._timestamp)
	{
		bRet = true; 
	}
	
	return bRet;
}


bool XDate::operator>(const XDate &obj)
{
	bool bRet = false;
	
	if(_timestamp > obj._timestamp)
	{
		bRet = true; 
	}
	
	return bRet;

}


bool XDate::operator<=(const XDate &obj)
{
	bool bRet = false;
	
	if(_timestamp <= obj._timestamp)
	{
		bRet = true; 
	}
	
	return bRet;

}


bool XDate::operator>=(const XDate &obj)
{
	bool bRet = false;
	
	if(_timestamp >= obj._timestamp)
	{
		bRet = true; 
	}
	
	return bRet;
}


NS_BASE_END


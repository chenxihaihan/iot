
#include "base_socket_oper.h"
#include "base_net.h"
#include "base_time.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


NS_BASE_BEGIN



//�ڳ�ʱʱ����ѭ������ָ�����ȵ�����
int Socket_Oper::recv_n(int fd, char *buf, unsigned int &len, const unsigned int timeout)
{
	int nRet = 1;

	unsigned int buf_size = len;
	len = 0;

	//����Ϊ������
	set_non_bolck(fd);

	unsigned long long begin_usec = get_microsecond(); //��ѭ��ʼʱ��
	unsigned long long end_usec = 0; //��ѭ����ʱ��
	
	while(true)
	{
		
		//����ѭ������ʱ��
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("recv_n timeout.\n");
				return 2;
			}
		}
			
		int ret = ::recv(fd, buf+len, buf_size, 0);
		if(ret > 0)
		{
			len += ret;
			buf_size -= ret;
			if(buf_size == 0)
			{
				//�յ�ָ�����ȵ�����
				return 1;
			}
		}
		else if(ret == 0)
		{
			//recv����0�����ֿ���: (1)�Զ�fd�ر�(2)����recv ��size=0
			//printf("socket(%d) is close\n", fd);
			return 0;
		}
		else
		{
			//û�յ����ݻ��߱��жϾͽ������ֽ��ղ���
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}

			else
			{
				printf("recv_n(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		end_usec = get_microsecond();

	}
	
	
	return nRet;

}




//�ڳ�ʱʱ���н�����һ��ָ�����ȵ�����
int Socket_Oper::recv(int fd, char *buf, unsigned int &len, const unsigned int timeout)
{
	int nRet = 1;

	unsigned int buf_size = len;
	len = 0;

	//����Ϊ������
	set_non_bolck(fd);

	unsigned long long begin_usec = get_microsecond(); //��ѭ��ʼʱ��
	unsigned long long end_usec = 0; //��ѭ����ʱ��
	
	while(true)
	{
		
		//����ѭ������ʱ��
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("recv timeout.\n");
				return 2;
			}
		}
			
		int ret = ::recv(fd, buf+len, buf_size, 0);
		if(ret > 0)
		{
			len += ret;
			buf_size -= ret;

			return 1;

		}
		else if(ret == 0)
		{
			//recv����0�����ֿ���: (1)�Զ�fd�ر�(2)����recv ��size=0
			//printf("socket(%d) is close\n", fd);
			return 0;
		}
		else
		{
			//û�յ����ݻ��߱��жϾͽ������ֽ��ղ���
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}

			else
			{
				printf("recv(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		end_usec = get_microsecond();

	}

	return nRet;
	

}





int Socket_Oper::send_n(int fd, const char *buf, unsigned int &len, int flags, const unsigned int timeout)
{
	int nRet = 0;

	unsigned int buf_size = len;
	len = 0;

	//����Ϊ������
	set_non_bolck(fd);

	unsigned long long begin_usec = get_microsecond(); //��ѭ��ʼʱ��
	unsigned long long end_usec = 0; //��ѭ����ʱ��
	
	while(true)
	{
		//����ѭ������ʱ��
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("send_n timeout.\n");
				return 2;
			}
		}
			
		int ret = ::send(fd, buf+len, buf_size, flags);
		if(ret >= 0)
		{
			len += ret;
			buf_size -= ret;
			if(buf_size == 0)
			{
				//����ָ�����ȵ�����
				return 0;
			}
		}
		else
		{
			//û�յ����ݻ��߱��жϾͽ������ֽ��ղ���
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}
			else
			{
				printf("send_n(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		end_usec = get_microsecond();

	}
	
	return nRet;

}




int Socket_Oper::send(int fd, const char *buf, unsigned int &len, int flags, const unsigned int timeout)
{
	int nRet = 0;

	unsigned int buf_size = len;
	len = 0;

	//����Ϊ������
	set_non_bolck(fd);

	unsigned long long begin_usec = get_microsecond(); //��ѭ��ʼʱ��
	unsigned long long end_usec = 0; //��ѭ����ʱ��
	
	while(true)
	{
		//����ѭ������ʱ��
		if(end_usec != 0)
		{
			unsigned long long span_usec = end_usec - begin_usec;
			if(span_usec >= timeout)
			{
				printf("send timeout.\n");
				return 2;
			}
		}
			
		int ret = ::send(fd, buf+len, buf_size, flags);
		if(ret >= 0)
		{
			len += ret;
			buf_size -= ret;

			return 0;
		}
		else
		{
			//û�յ����ݻ��߱��жϾͽ������ֽ��ղ���
			if((errno == EAGAIN)||(errno == EINTR)) 	
			{	
				//no oper
			}
			else
			{
				printf("send(%d) failed, errno:%d, errmsg:%s\n", 
						fd, errno, strerror(errno));
				return ret;
			}
		}

		end_usec = get_microsecond();

	}
	
	return nRet;

}





int Socket_Oper::send_s(int fd, const char *buf, unsigned int &len, int flags)
{
	int total = (int)len;
	int ret = ::send(fd, buf, total, flags);
	if(ret >= total)
	{
		return 0;
	}
	else if((ret < total) && (ret >= 0))
	{
		len = (unsigned int)ret;
		printf("send(%d) timeout, total:%u, len:%u\n", fd, total, len);
		return 2;
	}
	else if(ret < 0)
	{
		printf("send(%d) failed, errno:%d, errmsg:%s\n", fd, errno, strerror(errno));
	}
	
	return -1;

}




int Socket_Oper::connect(int fd, std::string ip, unsigned short port, const unsigned int timeout)
{
	int nRet = 0;
	
	long long sec = 0;
	long long usec = 0;
	if(timeout > 0)
	{
		sec = timeout/1000000;
		usec = timeout%1000000;

		set_non_bolck(fd);
	} 

	struct sockaddr_in addr;
	memset(&addr, 0x0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	inet_aton(ip.c_str(), &(addr.sin_addr));
	addr.sin_port = htons(port);
	socklen_t len = sizeof(addr);
	
	/*
	#include <sys/socket.h>
	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
	����ɹ�����0�����򷵻�-1
	*/
	nRet = ::connect(fd, (sockaddr*)&addr, len);
	if(nRet < 0)
	{
		if (errno == EINPROGRESS)
		{
			fd_set write_set;
			FD_ZERO(&write_set);
			FD_SET(fd, &write_set);
			
			while(true)
			{
				struct timeval _timeout;	
				_timeout.tv_sec = sec;	
				_timeout.tv_usec = usec;
				
				nRet = select(fd+1, NULL, &write_set, NULL, &_timeout);
				if(nRet > 0)
				{
					int retval = 0;
					socklen_t len = sizeof(retval);
					nRet = getsockopt(fd, SOL_SOCKET, SO_ERROR, &retval, &len);
					if (nRet != 0)
					{
						printf("getsockopt failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
						return nRet;
					}
					
					if (retval != 0)
					{
						printf("getsockopt SOL_SOCKET SO_ERROR, retval:%d\n", retval);
						return -1;
					}
					else
					{
						printf("connect(%s:%u) success during timeout.\n", ip.c_str(), port); 
						return 0;
					}
					
				}
				else if(nRet == 0)
				{
					printf("connect timeout, errno:%d, errmsg:%s\n", errno, strerror(errno));
					return 2;
				}
				else
				{
					if(errno == EINTR)
					{
						printf("connect is interrupt, errno:%d, errmsg:%s\n", errno, strerror(errno));
						continue;
					}
					else
					{
						printf("connect failed, errno:%d, errmsg:%s\n", errno, strerror(errno));
						return nRet;
					}
				}

			}		
			
		}
		else
		{
			printf("connect(%s:%u) failed, errno:%d, errmsg:%s\n", 
				ip.c_str(), port, errno, strerror(errno));
		}

	}
	else
	{
		printf("connect(%s:%u) success.\n", ip.c_str(), port);
	}

	return nRet;

}




int Socket_Oper::connect_s(int fd, std::string ip, unsigned short port, const unsigned int timeout)
{
	int nRet = 0;
	
	struct sockaddr_in addr;
	memset(&addr, 0x0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	inet_aton(ip.c_str(), &(addr.sin_addr));
	addr.sin_port = htons(port);
	socklen_t len = sizeof(addr);
	
	/*
	#include <sys/socket.h>
	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
	����ɹ�����0�����򷵻�-1
	*/
	nRet = ::connect(fd, (sockaddr*)&addr, len);
	if(nRet < 0)
	{
		//printf("connect(%s:%u) failed, errno:%d, errmsg:%s\n", ip.c_str(), port, errno, strerror(errno));
	}
	else
	{
		printf("connect(%s:%u) success.\n", ip.c_str(), port);
	}

	return nRet;

}


NS_BASE_END



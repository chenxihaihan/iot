//-----------------------------------------------------------------------------
//  ���ƣ�socktool.cpp
//  ���ܣ�CSockTool������Դ�ļ�
//  ��Ȩ��
//  �汾��1.0
//  ���ߣ�g++
//  ���룺��
//  ��������
//  ��ʷ������
//-----------------------------------------------------------------------------

#include "socktool.h"
/*
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
*/
	
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <limits.h>
#include <linux/rtc.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>  
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/select.h> 
#include <sys/types.h>
#include <unistd.h>
#include <iostream> 


namespace MTNMsgApi_NP{
	
#define TIMEDELTA_MS(time1,time0) \
  ((time1.tv_sec-time0.tv_sec)*1000+(time1.tv_usec-time0.tv_usec)/1000)
  	
// �ȴ�nTimeout����
void CSockTool::wait( int nTimeout )
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = nTimeout*1000;
  ::select(0,0,0,0,&tv);
}

// �ȴ�nTimeout΢��
void CSockTool::wait2( int nTimeout )
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = nTimeout;
  ::select(0,0,0,0,&tv);
}

// ����fdΪ������ģʽ
bool CSockTool::setNonBlocking( int fd, bool bNoBlock )
{
  int nOpts;

  nOpts = fcntl(fd, F_GETFL);
  if( nOpts < 0 )
    return false;

  if( bNoBlock )
    nOpts |= O_NONBLOCK;
  else
    nOpts &= ~O_NONBLOCK;
  	
  if( fcntl(fd, F_SETFL, nOpts)<0 )
    return false;
  
  return true;
}

// ����socket������Ϊ��������
int CSockTool::listen( const char* pAddress, int nPort, int nMaxBuf )
{
  int nSock = socket(AF_INET, SOCK_STREAM, 0);
  if( nSock == -1 )
    return -1;

  // ���ü���socket
  struct sockaddr_in serverAddr;
  char* ip;
  int nReuse = 1;

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  if( !pAddress || !*pAddress ) // δָ����ַ
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  else
  {
    if( !inet_aton(pAddress, &(serverAddr.sin_addr)) ) // ��Ч���ַ
    {
      hostent* pHost = gethostbyname(pAddress);
      if( !pHost ) // ��Ч�ĵ�ַ
        goto fail;
      ip = inet_ntoa(*(struct in_addr *)*pHost->h_addr_list); 
      if( !inet_aton(ip, &(serverAddr.sin_addr)) ) // ��Ч���ַ
        goto fail;
    }
  }
  
  serverAddr.sin_port = htons(nPort);
  if( ::setsockopt(nSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&nReuse, sizeof(nReuse)) == -1 )
    goto fail;
  if( ::bind(nSock, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1 )
    goto fail;
  if( ::listen(nSock, nMaxBuf) == -1 )
    goto fail;

  if( !CSockTool::setNonBlocking(nSock) )
    goto fail;
    
  return nSock;

fail:
  CSockTool::close(nSock);
  return -1;
}

// �ȴ����ӣ�nListenSock�Ƿ������ģ����ص�ҲΪ��������
int CSockTool::waitConnect( int nListenSock, int nTimeout )
{
  fd_set rdfds;
  FD_ZERO(&rdfds);
  FD_SET(nListenSock, &rdfds);
 
  int nSel;
  if( nTimeout > 0 )
  {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = nTimeout*1000;
    while( (nSel = ::select(nListenSock+1, &rdfds, 0, 0, &tv)) == -1 && errno == EINTR );
  }
  else
    while( (nSel = ::select(nListenSock+1, &rdfds, 0, 0, 0)) == -1 && errno == EINTR );
    
  return nSel;
}

// socket�Ƿ������ģ����ص�ҲΪ��������
int CSockTool::accept( int nListenSock, struct sockaddr* pClientAddr, socklen_t* pClientLen )
{
  int nSock;
  while( (nSock = ::accept(nListenSock, pClientAddr, pClientLen)) == -1 && errno == EINTR );
  if( nSock == -1 || !setNonBlocking(nSock) ) // ����
  {
    if( nSock != -1 )
    {
      CSockTool::close(nSock);
    }
    return -1;
  }
  return nSock;
}

// ����������˵�����
int CSockTool::connect( const char* pServerAddress, int nServerPort, int nTimeout, const char* pLocalAddress, int nLocalPort )
{
  int nSock = socket(AF_INET, SOCK_STREAM, 0);
  if( nSock == -1 )
    return -1;

  // ���÷����ַ
  struct sockaddr_in serverAddr, clientAddr;
  char* ip;
  int nRet = -1;
  int nReuse = 1;

  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  if( !pServerAddress || !*pServerAddress ) // δָ����ַ
    goto fail;
  if( !inet_aton(pServerAddress, &(serverAddr.sin_addr)) ) // ��Ч���ַ
  {
    hostent* pHost = gethostbyname(pServerAddress);
    if( !pHost ) // ��Ч�ĵ�ַ
      goto fail;
    ip = inet_ntoa(*(struct in_addr *)*pHost->h_addr_list); 
    if( !inet_aton(ip, &(serverAddr.sin_addr)) ) // ��Ч���ַ
      goto fail;
  }
  serverAddr.sin_port = htons(nServerPort);

  // ���ÿͻ���
  if( pLocalAddress && *pLocalAddress && nLocalPort > 0 )
  {
	  if( ::setsockopt(nSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&nReuse, sizeof(nReuse)) == -1 )
	    goto fail;

    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    if( !inet_aton(pLocalAddress, &(clientAddr.sin_addr)) ) // ��Ч���ַ
    {
      hostent* pHost = gethostbyname(pLocalAddress);
      if( !pHost ) // ��Ч�ĵ�ַ
        goto fail;
      ip = inet_ntoa(*(struct in_addr *)*pHost->h_addr_list); 
      if( !inet_aton(ip, &(clientAddr.sin_addr)) ) // ��Ч���ַ
        goto fail;
    }

    clientAddr.sin_port = htons(nLocalPort);  
    if( ::bind(nSock, (sockaddr *)&clientAddr, sizeof(clientAddr)) == -1 )
    	goto fail; // EADDRINUSE
  }

  if( !CSockTool::setNonBlocking(nSock) )
    goto fail;

  // ������������
  int nTry, nTimes;
  nTry = 0;
  nTimes = nTimeout*100;
  while( nTry < nTimes && ::connect(nSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1 )
  {
  	if( errno == EADDRNOTAVAIL || errno == ENOTCONN )
  	{
  		nTry++;
  		CSockTool::wait2(10);
  		continue;
    }
    if( nTimeout <= 0 || errno != EINPROGRESS )
    {
    	//printf("connect: try = %d  errno = %d  %s\n", nTry, errno, strerror(errno));
      goto fail;
    }
    if( CSockTool::waitWrite(nSock, nTimeout) <= 0 )
    {
    	nRet = -2;
    	goto fail;
    }
    // ������ɣ��ж��Ƿ�ɹ�
    int nError;
    socklen_t nLen = sizeof(nError);
    getsockopt(nSock, SOL_SOCKET, SO_ERROR, &nError, &nLen);
    if( nError )
      goto fail;
  }
    
  return nSock;

fail:
  CSockTool::close(nSock);
  return nRet;  
}

// �Ƿ���Զ���
int CSockTool::waitRead( int nSock, int nTimeout )
{
  fd_set rdfds;
  FD_ZERO(&rdfds);
  FD_SET(nSock, &rdfds);
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = nTimeout*1000;
  int nSel;
  while( (nSel = ::select(nSock+1, &rdfds, 0, 0, &tv)) == -1 && (errno == EINTR || errno == EAGAIN) );
  
  return nSel;
}
  
// ����������socket
int CSockTool::read( int nSock, char* pBuf, int nLen, int nTimeout )
{
  struct timeval time0, time1;
  gettimeofday(&time0, 0);
  int nRet = CSockTool::waitRead(nSock, nTimeout);
  if( nRet <= 0 ) // �����û����
  {
    return nRet;
  }
 
  // ��֮ǰһ��������
  int nReadIndex = 0;
  int nRead;
  while( true )
  {
    nRead = ::read(nSock, &pBuf[nReadIndex], nLen);
    if(nRead == -1)
    {
		if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EAGAIN )
		{
			//non-blocking  and  no  connections are present to be accepted.
		}
		else
		{
			//othre errorno.
			//printf("CSockTool::read() read error= %s\n",strerror(errno));	
		}
		CSockTool::wait(1000);
		continue;
    }
    else if( nRead == 0 ) // socket���ر�
    {
		break;      
    }
    else if( nRead > 0 ) // ����������
    {
      nLen -= nRead;
      nReadIndex += nRead;
      if( nLen == 0 ) // �����
        break;
        
      // �ȴ�����      
      gettimeofday(&time1, 0);
      nTimeout -= TIMEDELTA_MS(time1,time0);

      if( nTimeout <= 0 )
        break;
      
      nRet = CSockTool::waitRead(nSock, nTimeout);     
      if( nRet > 0 ) // ������
      {
        time0 = time1;
        continue;
      }
      if( nRet == 0 ) // û����
        break;
      return -1; // ����
      
      
    }
    else // socketʧ��
    { 
      return -1;
    }
  }
  return nReadIndex;
}

// �Ƿ����д��
int CSockTool::waitWrite( int nSock, int nTimeout )
{
  fd_set wrfds;
  FD_ZERO(&wrfds);
  FD_SET(nSock, &wrfds);
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = nTimeout*1000;
  int nSel;
  while( (nSel = ::select(nSock+1, 0, &wrfds, 0, &tv)) == -1 && errno == EINTR );
  
  return nSel;
}

// д��������socket
int CSockTool::write( int nSock, const char* pBuf, int nLen, int nTimeout )
{
  struct timeval time0, time1;
  gettimeofday(&time0, 0);
  int nRet = CSockTool::waitWrite(nSock, nTimeout);
  if( nRet <= 0 ) // �����û����
    return nRet;
 
  // д֮ǰһ������д
  int nWriteIndex = 0;
  int nWrite;
  while( true )
  {
    while( (nWrite = ::write(nSock, &pBuf[nWriteIndex], nLen)) == -1 && errno == EINTR );
    if( nWrite > 0 ) // д������
    {
      nLen -= nWrite;
      nWriteIndex += nWrite;
      if( nLen == 0 ) // д���
        break;
        
      // �ȴ��ռ�
      gettimeofday(&time1, 0);
      nTimeout -= TIMEDELTA_MS(time1,time0);

      if( nTimeout <= 0 )
        break;
      
      nRet = CSockTool::waitWrite(nSock, nTimeout);
      if( nRet > 0 ) // �пռ�
      {
        time0 = time1;  
        continue;
      }
      if( nRet == 0 ) // ��ʱ
        break;
      // select ����
      return -1;
    }
    else if( nWrite == 0 ) // û�����ݿ�д
    {
      break;      
    }
    else // socketʧ��
    { 
      return -1;
    }
  }
  return nWriteIndex;
}

void CSockTool::close( int nSock )
{
	::shutdown(nSock, SHUT_RDWR);
	while( ::close(nSock) == -1 && errno == EINTR );
}

}//namespace end













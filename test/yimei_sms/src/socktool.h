#ifndef __SOCKTOOL_H__
#define __SOCKTOOL_H__

#include <sys/socket.h>
#include <errno.h>

//-----------------------------------------------------------------------------
/// <summary>
/// socket������
/// </summary>
//-----------------------------------------------------------------------------
// �ܵ�����ʹ��select�ж��Ƿ�ɶ��Ϳ�д

namespace MTNMsgApi_NP{
	
	class CSockTool
	{
	  public:
	  	    
	    // �ȴ�nTimeout����
	    static void wait( int nTimeout );
	
	    // �ȴ�nTimeout΢��
	    static void wait2( int nTimeout );
	    
	    // ����fdΪ������ģʽ
	    static bool setNonBlocking( int fd, bool bNoBlock = true );
	    
	    // ����socket������Ϊ��������
	    // -1 ʧ�ܣ�����socket
	    static int listen( const char* pAddress, int nPort, int nMaxBuf = 200 );
	    
	    // �ȴ����ӣ�nListenSock�Ƿ������ģ����ص�ҲΪ��������
	    // nTimeoutΪ-1��ʾ���õȴ� ����
	    // ����-1 ʧ�ܣ�0 ��ʱ��>0 �ɹ�
	    static int waitConnect( int nListenSock, int nTimeout = -1 );
	    
	    // nListenSock�Ƿ������ģ����ص�ҲΪ��������
	    static int accept( int nListenSock, struct sockaddr* pClientAddr, socklen_t *pClientLen );
	
	    // ����������˵����ӣ����ص�Ϊ�������� ����
	    // ��ָ����pLocalAddress��nLocalPortʱ���󶨱���socket��ָ���ĵ�ַ�Ͷ˿�
	    static int connect( const char* pServerAddress, int nServerPort, int nTimeout = 10000, const char* pLocalAddress = 0, int nLocalPort = 0 );
	    
	    // �Ƿ���Զ��룬nSockΪ�������� ����
	    // 0 ��ʱ��-1 ����>0 ���Զ�
	    static int waitRead( int nSock, int nTimeout = 10000 );
	    
	    // ����������socket ����
	    static int read( int nSock, char* pBuf, int nLen, int nTimeout = 10000 );
	    
	    // �Ƿ����д����nSockΪ�������� ����
	    // 0 ��ʱ��-1 ����>0 ����д
	    static int waitWrite( int nSock, int nTimeout = 10000 );
	    
	    // д��������socket ����
	    static int write( int nSock, const char* pBuf, int nLen, int nTimeout = 10000 );
	    
	    // shutdown���ر�socket
	    static void close( int nSock );
	};

}//namespace end.

#endif // __SOCKTOOL_H__






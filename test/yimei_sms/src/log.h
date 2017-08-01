#ifndef _LOG_H_CREATE_BY_HELPING_
#define _LOG_H_CREATE_BY_HELPING_

#include<sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h> 
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <linux/unistd.h>
#include <unistd.h>


using namespace std;

namespace MTNMsgApi_NP
{
	
	#define MAXARRLEN 200
	#define MAXBUFLEN 1024
	#define MAXLOGLEN 4096
	
	#define LOGPATH "Path"
	#define LOGFILE "File"
	#define LOGMASK "Mask"
	#define LOGSIZE "Size"
	
	enum enumMessage{M1=0x0001,NT=0x0002,WA=0x0004,ER=0x0008,FA=0x0010,
	                 D1=0x0020,D2=0x0040,D3=0x0080,D4=0x0100,D5=0x0200};  /// ��־�ȼ�
	
	
	//-------------------------------------------------------------------------------------------------
	/// <summary> 
	/// ��CLog�Ķ���
	/// </summary>
	//-------------------------------------------------------------------------------------------------
	class CLog{
	public:
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
	    /// class CLog�Ĺ��캯��
	    /// </summary> 	
	    //-------------------------------------------------------------------------------------------------
		CLog();   
	
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
	    /// class CLog����������
	    /// </summary> 	
	    //-------------------------------------------------------------------------------------------------
		~CLog();   
		
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
		/// ��һ���Ѿ����ڵ���־�ļ����ߴ���һ���µ�
	    /// </summary> 
		/// <param name=" pLogDir ">
	    /// �������־�ļ��Ĵ��Ŀ¼
	    /// </param>
	    /// <param name=" pLogPrefix">
	    /// �������־�ļ����Ƶ�ǰ׺
	    /// </param>
		/// <param name=" nMaxFileLen">
	    /// �������־�ļ�����󳤶�
	    /// </param>
		/// <param name=" nLevelMask ">
	    /// �������־�ȼ�
	    /// </param>
	    /// <returns>
	    /// �ɹ�ʱ����1��ʧ��ʱ����0��
	    /// </returns>
	    //-------------------------------------------------------------------------------------------------
		int  openLog(char* pLogDir, char* pLogPrefix, int nMaxFileLen, int nLevelMask);
	
	
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
		/// д��־����־�ļ��У�����Ҫ�л����ؿ�һ���ļ���д
	    /// </summary> 
		/// <param name=" nLevel ">
	    /// �������־�ȼ�
	    /// </param>
		/// <param name=" pFmt">
	    /// ������Զ����ʽ����Ϣ
	    /// </param>
		/// <returns>
	    /// �ɹ�ʱ����1��ʧ��ʱ����0��
	    /// </returns>
	    //-------------------------------------------------------------------------------------------------
		int  writeLog(enum enumMessage nLevel, char* pFmt, ...);
	
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
		/// ��������
	    /// </summary> 
		/// <param name=" nMaskLevel ">
	    /// ���������
	    /// </param>	
		/// <returns>
	    /// �ɹ�ʱ����1��ʧ��ʱ����0��
	    /// </returns>
	    //-------------------------------------------------------------------------------------------------
	    int setMask(int nMaskLevel);
	
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
		/// ������־�ļ���󳤶�
	    /// </summary> 
		/// <param name=" nMaxlen ">
	    /// �������־�ļ����ĳ���ֵ
	    /// </param>	
		/// <returns>
	    /// �ɹ�ʱ����1��ʧ��ʱ����0��
	    /// </returns>
	    //-------------------------------------------------------------------------------------------------
		int setMaxLen(int nMaxLen);
	
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
		/// �ر���־�ļ�
	    /// </summary> 
		//-------------------------------------------------------------------------------------------------
		void closeLog();	
	
	private:
		//-------------------------------------------------------------------------------------------------
	    /// <summary> 
		/// �õ���ʽ����ʱ����
	    /// </summary> 
		/// <param name=" pOutBuffer ">
	    /// ���������ڴ��ʱ�������
	    /// </param>
	    /// <param name=" nType">
	    /// ��������ͣ���ͬ�����������ͬ�ĸ�ʽ��0��YYYY-MM-DD HH24:MI:SS.MS�� 1��MMDDMI��
	    /// </param>
		/// <returns>
	    /// ��
	    /// </returns>
	    //-------------------------------------------------------------------------------------------------
		void getTime(char * pOutBuffer,int nType);
	
	private:	
		char          m_arrDir[MAXARRLEN];     /// ��־�ļ��ı���Ŀ¼���û����ã�
		char          m_arrPrefix[MAXARRLEN];  /// ��־�ļ����Ƶ�ǰ׺���û����ã�
		int           m_nMaxLen;               /// ��־�ļ�����󳤶ȣ��û����ã�
		int           m_nMask;                 /// ��־�ȼ����û����ã�	
		
		FILE*         m_pLogFp_MS;             /// ��־�ļ����ļ����
		FILE*         m_pLogFp_WR;             /// ��־�ļ����ļ����
		
		char          m_arrFname_MS[MAXARRLEN];   /// ��־�ļ�������
		char          m_arrFname_WR[MAXARRLEN];   /// ��־�ļ�������
		
		int           m_nNeedChange_MS;           /// ��־�ļ���С������󳤶�ʱ��Ҫ�л��ı��		
		int           m_nNeedChange_WR;           /// ��־�ļ���С������󳤶�ʱ��Ҫ�л��ı��		
		
		int           m_nLogLen_MS;               /// ��־�ļ��ĵ�ǰ��С
		int           m_nLogLen_WR;               /// ��־�ļ��ĵ�ǰ��С
	
		time_t        m_nOpenTime;             /// ��־�ļ��Ĵ���ʱ��
		time_t        m_nLastMod;              /// ��־�ļ�������޸�ʱ��
		
	#ifndef NO_PTHREAD
		pthread_mutex_t   ms_mutex ;
	#endif
	};
}
#endif






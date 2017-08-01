//---------------------------------------------------------------------------
//    ���ƣ�log.cpp
//    ���ܣ�CLog���Դ�ļ�
//    ��Ȩ��
//    �汾��1.0
//    ���ߣ�Visual Studio 2005
//    ���룺��
//    ��������
//    ��ʷ���汾1.0������
//---------------------------------------------------------------------------

#include "log.h"

//#define NO_PTHREAD

namespace MTNMsgApi_NP
{

CLog::CLog()
{	
	// ��ʼ����ĳ�Ա����		
	m_nMaxLen      = MAXBUFLEN;
	m_nMask        = 0x03FF;
	m_arrDir[0]    = 0;
	m_arrPrefix[0] = 0;	

	m_pLogFp_MS       = NULL;
	m_pLogFp_WR       = NULL;
	m_nLogLen_MS      = 0; 
	m_nLogLen_WR      = 0; 
	m_nNeedChange_MS  = 0;
	m_nNeedChange_WR  = 0;
	m_arrFname_MS[0]  = 0;
	m_arrFname_WR[0]  = 0;
	
	m_nOpenTime    = 0;
	m_nLastMod     = 0;
#ifndef NO_PTHREAD
	pthread_mutex_init(&ms_mutex,NULL);	
#endif
	// ����
}

CLog::~CLog()
{
	if(m_pLogFp_MS)
		fclose(m_pLogFp_MS);
	m_pLogFp_MS = NULL;

	if(m_pLogFp_WR)
		fclose(m_pLogFp_WR);
	m_pLogFp_WR = NULL;

#ifndef NO_PTHREAD
	pthread_mutex_destroy(&ms_mutex);
#endif
}

//-------------------------------------------------------------------------------------------------
//    ���ƣ�CLog::openLog()
//    ���ܣ�����־�����������򴴽���־
//    ���룺1. char* pLogDir        ( �������־�ļ��ı���·�� )
//          2. char* pLogPrefix     ( �������־�ļ���ǰ׺ )
//          3. int nMaxFileLen     ( �������־�ļ�����󳤶�)
//          4. int nLevelMask       ( �������־�ȼ� )//          
//    �����int
//          1. 1(�����ɹ�)
//          2. 0(����ʧ��)
//    �汾��1.0
// ---------------------------------------------------------------------------------------------
int CLog::openLog(char* pLogDir, char* pLogPrefix, int nMaxFileLen, int nLevelMask)
{	
	
	/*���ղ���*/
	if(pLogDir != NULL)
		strcpy(m_arrDir,pLogDir);

	if(pLogPrefix != NULL)
		strcpy(m_arrPrefix,pLogPrefix);
	else
	{
		printf("filename can not be null!\n");
		return 0;
	}

	m_nMaxLen  = nMaxFileLen;
	m_nMask    = nLevelMask;    
    /*END ���ղ���*/

	/*���������ļ�������*/
	int nDirLen;                        /// Ŀ¼����
	char arrFileName_MS[MAXBUFLEN];     /// ���ڴ���ļ���������	
	char arrFileName_WR[MAXARRLEN];      /// ���ڴ�ŵ�ǰʱ�������	
	arrFileName_MS[0] = 0;
	arrFileName_WR[0] = 0;		

	
	if((nLevelMask & 0x03FF) == 0)
	{
		printf("this level is not exist , please check the level_mask you set\n");
		return 0;
	}
	
	if(pLogDir && strlen(pLogDir)>0)
	{
		nDirLen = strlen(pLogDir);		
		if(pLogDir[nDirLen-1] == '/')
		{
			snprintf(arrFileName_MS,MAXBUFLEN-1,"%sMS_%s.txt",m_arrDir,m_arrPrefix);
			snprintf(arrFileName_WR,MAXBUFLEN-1,"%sWR_%s.txt",m_arrDir,m_arrPrefix);
		}
		else
		{
			snprintf(arrFileName_MS,MAXBUFLEN-1,"%s/MS_%s.txt",m_arrDir,m_arrPrefix);
			snprintf(arrFileName_WR,MAXBUFLEN-1,"%s/WR_%s.txt",m_arrDir,m_arrPrefix);
		}
	}
	else if(!pLogDir || strlen(pLogDir) == 0)
	{
		snprintf(arrFileName_MS,MAXBUFLEN-1,"MS_%s.txt",m_arrPrefix);
		snprintf(arrFileName_WR,MAXBUFLEN-1,"WR_%s.txt",m_arrPrefix);
	}
	
	strcpy(m_arrFname_MS, arrFileName_MS);
	strcpy(m_arrFname_WR, arrFileName_WR);
    /*END ���������ļ�������*/

	/*�������ļ�*/
	if(m_pLogFp_MS == NULL)
	{
		m_pLogFp_MS = fopen(arrFileName_MS,"r+");		
		if(!m_pLogFp_MS)
		{
			m_pLogFp_MS = fopen(arrFileName_MS,"w+");	
			if(!m_pLogFp_MS)
			{
				printf("open file : %s failed!!\n",arrFileName_MS);
				m_pLogFp_MS = NULL;
				return 0;
			}
			
			fseek(m_pLogFp_MS,0,SEEK_END);
			m_nLogLen_MS = ftell(m_pLogFp_MS);
			//return 1; 
		}
		else
		{
			fseek(m_pLogFp_MS,0,SEEK_END);
			m_nLogLen_MS = ftell(m_pLogFp_MS);
			//return 1; 
		}
	}
	

	if(m_pLogFp_WR == NULL)
	{
		m_pLogFp_WR = fopen(arrFileName_WR,"r+");		
		if(!m_pLogFp_WR)
		{
			m_pLogFp_WR = fopen(arrFileName_WR,"w+");	
			if(!m_pLogFp_WR)
			{
				printf("open file : %s failed!!\n",arrFileName_WR);
				m_pLogFp_WR = NULL;
				return 0;
			}
			
			fseek(m_pLogFp_WR,0,SEEK_END);
			m_nLogLen_WR = ftell(m_pLogFp_WR);
			//return 1; 
		}
		else
		{
			fseek(m_pLogFp_WR,0,SEEK_END);
			m_nLogLen_WR = ftell(m_pLogFp_WR);
			//return 1; 
		}	
	}

	/*END �������ļ�*/	
	return 1; 
}

//-------------------------------------------------------------------------------------------------
//    ���ƣ�CLog::writeLog()
//    ���ܣ�д��־������Ҫ�л�����С������󳤶�ʱ�������´����ļ�
//    ���룺1. int nLevel            ( �������־�ȼ�)
//          2. char* pFmt            ( ����ĸ�ʽ����Ϣ)
//    �����int
//          1. 1(�����ɹ�)
//          2. 0(����ʧ��)
//    �汾��1.0
// ---------------------------------------------------------------------------------------------
//    ����Ϊ�����Զ������ĵ���XML��ʽ˵�� ��
// ---------------------------------------------------------------------------------------------
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
int CLog::writeLog( enum enumMessage nLevel, char* pFmt, ...)
{
	if(!(nLevel & m_nMask))
		return 0;

	bool bM1 = false;
	//FILE *pTry = NULL;
	int nDirLen = strlen(m_arrDir);	
	char arrTimeRec[MAXARRLEN];      /// ���ڼ�¼ʱ�������	
	char arrTimeRec1[MAXARRLEN];
	char arrMessage[MAXARRLEN];      /// ��¼��־�ȼ���Ϣ������	
	char arrBuffer[MAXLOGLEN];       /// ��¼�����־��Ϣ������
	char arrFmat[MAXLOGLEN];         /// ���ո�ʽ������������
	char arrTimeNewFile[MAXARRLEN];  /// ��Ҫ�޸�����ʱ��Ӧ��ʱ�� 
	char arrNewFileName[MAXARRLEN];  /// �޸ĺ���ļ���
	//char arrNewFileName_WR[MAXARRLEN];  /// �޸ĺ���ļ���
	arrFmat[0]         = 0;
	arrTimeRec[0]      = 0;
	arrTimeRec1[0]     = 0;
	arrMessage[0]      = 0;
	arrBuffer[0]       = 0;
	arrTimeNewFile[0]  = 0;
	arrNewFileName[0]  = 0;	
	//arrNewFileName_WR[0]  = 0;	

	switch(nLevel)
	{
		case M1:
			strcpy(arrMessage,"M1");
			break;
		case NT:
			strcpy(arrMessage,"NT");
			break;
		case WA:
			strcpy(arrMessage,"WA");
			break;
		case ER:
			strcpy(arrMessage,"ER");
			break;
		case FA:
			strcpy(arrMessage,"FA");
			break;
		case D1:
			strcpy(arrMessage,"D1");
			break;
		case D2:
			strcpy(arrMessage,"D2");
			break;
		case D3:
			strcpy(arrMessage,"D3");
			break;
		case D4:
			strcpy(arrMessage,"D4");
			break;
		case D5:
			strcpy(arrMessage,"D5");
			break;		
		default:
			return 0;
	}  	

	if(nLevel == M1)
		bM1 = true;

	//ʵ�ֿɱ����		
	va_list argptr;
	va_start(argptr, pFmt);
	vsnprintf(arrFmat,MAXLOGLEN-1,pFmt,argptr);
	va_end(argptr);
	//����	

	getTime(arrTimeRec,0); 
	getTime(arrTimeRec1,2);

	if(!bM1)
		snprintf(arrBuffer,MAXLOGLEN-1,"%s %s %d.%u %s\n",
		     arrTimeRec, arrMessage,getpid(),(unsigned int)pthread_self(),arrFmat);
	else
		snprintf(arrBuffer,MAXLOGLEN-1,"%s %s %s\n",
		     arrTimeRec1, arrMessage,arrFmat);

	
	int nBufferLength = strlen(arrBuffer);
	

	// ����ļ��Ѿ�������󳤶ȣ������¿�һ���ļ�д������ֱ��д�뵽�ļ���

	int nLogLenNow = 0;
	
#ifndef  NO_PTHREAD
		pthread_mutex_lock (&ms_mutex);
#endif

		if(bM1)
			nLogLenNow = m_nLogLen_MS;
		else
			nLogLenNow = m_nLogLen_WR;

		if(nLogLenNow+nBufferLength > m_nMaxLen)
		{
			if(bM1)
			{
				if(m_pLogFp_MS)
					fclose(m_pLogFp_MS);
				m_pLogFp_MS = NULL;
			}
			else
			{
				if(m_pLogFp_WR)
					fclose(m_pLogFp_WR);
				m_pLogFp_WR = NULL;
			}

			if(bM1)
				m_nNeedChange_MS ++;
			else
				m_nNeedChange_WR ++;

			getTime(arrTimeNewFile,1);
			
			if(nDirLen > 0)
			{
				if(m_arrDir[nDirLen-1] == '/')
				{
					if(bM1)
						snprintf(arrNewFileName,MAXARRLEN-1,"%sMS_%s_%s_%d.txt",
						    m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_MS);
					else
						snprintf(arrNewFileName,MAXARRLEN-1,"%sWR_%s_%s_%d.txt",
						    m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_WR);
				}
				else
				{
					if(bM1)
						snprintf(arrNewFileName,MAXARRLEN-1,"%s/MS_%s_%s_%d.txt",
						   m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_MS);
					else
						snprintf(arrNewFileName,MAXARRLEN-1,"%s/WR_%s_%s_%d.txt",
						   m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_WR);
				}
			}
			else
			{
				if(bM1)
					snprintf(arrNewFileName,MAXARRLEN-1,"MS_%s_%s_%d.txt",
						  m_arrPrefix,arrTimeNewFile,m_nNeedChange_MS);
				else
					snprintf(arrNewFileName,MAXARRLEN-1,"WR_%s_%s_%d.txt",
						  m_arrPrefix,arrTimeNewFile,m_nNeedChange_WR);
			}


			//pTry = NULL;
			while(1)
			{
				// ���������ļ����Ѿ����ڣ��ٸ���
				//if( (pTry = fopen(arrNewFileName,"r") ) != NULL)  
				if( access(arrNewFileName,F_OK)  != -1)  
				{
					if(bM1)
						m_nNeedChange_MS ++;
					else 
						m_nNeedChange_WR ++;
					
					if(nDirLen > 0)
					{
						if(m_arrDir[nDirLen-1] == '/')
						{
							if(bM1)
								snprintf(arrNewFileName,MAXARRLEN-1,"%sMS_%s_%s_%d.txt",
								m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_MS);
							else
								snprintf(arrNewFileName,MAXARRLEN-1,"%sWR_%s_%s_%d.txt",
								m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_WR);
						}
						else
						{
							if(bM1)
								snprintf(arrNewFileName,MAXARRLEN-1,"%s/MS_%s_%s_%d.txt",
								m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_MS);
							else
								snprintf(arrNewFileName,MAXARRLEN-1,"%s/WR_%s_%s_%d.txt",
								m_arrDir,m_arrPrefix,arrTimeNewFile,m_nNeedChange_WR);
						}
					}
					else
					{
						if(bM1)
							snprintf(arrNewFileName,MAXARRLEN-1,"MS_%s_%s_%d.txt",
							m_arrPrefix,arrTimeNewFile,m_nNeedChange_MS);
						else
							snprintf(arrNewFileName,MAXARRLEN-1,"WR_%s_%s_%d.txt",
							m_arrPrefix,arrTimeNewFile,m_nNeedChange_WR);

					}	
					
                    //fclose(pTry);
					//pTry = NULL;

				}
				else
				{						
					break;
				}
				// END ���������ļ����Ѿ����ڣ��ٸ���
			}
			


			//  �������ļ�
			char  arrFnameNow[MAXARRLEN];
			arrFnameNow[0] = 0;

			if(bM1)
				strcpy(arrFnameNow,m_arrFname_MS);
			else
				strcpy(arrFnameNow,m_arrFname_WR);

			if(0!=rename(arrFnameNow,arrNewFileName))
			{
#ifndef  NO_PTHREAD	
				pthread_mutex_unlock (&ms_mutex);
#endif
				return 0;	
			}
			// end �������ļ�
			
			if(openLog(m_arrDir,m_arrPrefix,m_nMaxLen,m_nMask)==0)
			{		  
#ifndef  NO_PTHREAD	
				pthread_mutex_unlock (&ms_mutex);
#endif
				return 0;
			}
		}

		
	//�������Ļ
	if( (bM1 && !m_pLogFp_MS) || (!bM1 && !m_pLogFp_WR) )
	{
		if(nLevel==WA || nLevel==FA || nLevel==ER)
			cerr<<arrBuffer;
		else
			cout<<arrBuffer;

#ifndef  NO_PTHREAD	
		pthread_mutex_unlock (&ms_mutex);
#endif
		return 1;
	}
	//end �������Ļ

	if(bM1)
	{
		fseek(m_pLogFp_MS,0,SEEK_END);
		if(0==fwrite(arrBuffer,strlen(arrBuffer),1,m_pLogFp_MS))
		{

#ifndef  NO_PTHREAD
			pthread_mutex_unlock (&ms_mutex);
#endif
			printf("write information to file failed!\n");
			return 0;			
		}

		fflush(m_pLogFp_MS);
		m_nLogLen_MS += nBufferLength;

	}
	else
	{
		fseek(m_pLogFp_WR,0,SEEK_END);
		if(0==fwrite(arrBuffer,strlen(arrBuffer),1,m_pLogFp_WR))
		{

#ifndef  NO_PTHREAD
			pthread_mutex_unlock (&ms_mutex);
#endif
			printf("write information to file failed!\n");
			return 0;			
		}

		fflush(m_pLogFp_WR);
		m_nLogLen_WR += nBufferLength;

	}

#ifndef  NO_PTHREAD	
	pthread_mutex_unlock (&ms_mutex);
#endif
	
	return 1;	
}

//-------------------------------------------------------------------------------------------------
//    ���ƣ�CLog::setMask()
//    ���ܣ���������
//    ���룺1. int nMaskLevel ����������룩          
//    �����int ��  1���ɹ��� 0��ʧ�ܣ�
//    �汾��1.0
// ---------------------------------------------------------------------------------------------
//    ����Ϊ�����Զ������ĵ���XML��ʽ˵�� ��
// ---------------------------------------------------------------------------------------------
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
int CLog::setMask(int nMaskLevel)
{
	m_nMask = nMaskLevel;
	return 1;
}

//-------------------------------------------------------------------------------------------------
//    ���ƣ�CLog::setMaxLen()
//    ���ܣ�������־�ļ���󳤶�
//    ���룺1. int nMaxLen ����־�ļ���󳤶ȣ�          
//    �����int ��  1���ɹ��� 0��ʧ�ܣ�
//    �汾��1.0
// ---------------------------------------------------------------------------------------------
//    ����Ϊ�����Զ������ĵ���XML��ʽ˵�� ��
// ---------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// <summary> 
/// ������־�ļ���󳤶�
/// </summary> 
/// <param name=" nMaxLen ">
/// �������־�ļ����ĳ���ֵ
/// </param>	
/// <returns>
/// �ɹ�ʱ����1��ʧ��ʱ����0��
/// </returns>
//-------------------------------------------------------------------------------------------------
int CLog::setMaxLen(int nMaxLen)
{
	m_nMaxLen = nMaxLen;
	return 1;
}

//-------------------------------------------------------------------------------------------------
//    ���ƣ�CLog::closeLog()
//    ���ܣ��ر���־
//    ���룺��          
//    �������
//    �汾��1.0
// ---------------------------------------------------------------------------------------------
//    ����Ϊ�����Զ������ĵ���XML��ʽ˵�� ��
// ---------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// <summary> 
/// �ر���־�ļ�
/// </summary> 
//-------------------------------------------------------------------------------------------------
void CLog::closeLog()
{
	if(m_pLogFp_MS)
		fclose(m_pLogFp_MS);
	m_pLogFp_MS = NULL;

	if(m_pLogFp_WR)
		fclose(m_pLogFp_WR);
	m_pLogFp_WR = NULL;
}


//-------------------------------------------------------------------------------------------------
//    ���ƣ�CLog::getTime()
//    ���ܣ����ݲ������ز�ͬ�ĸ�ʽ��ʱ����
//    ���룺1. char * pOutBuffer ( �����Ĵ��ʱ����ʽ������ )
//          2. int nType         ( �����ʱ����ʽ���� )
//    ������� 
//    �汾��1.0
// ---------------------------------------------------------------------------------------------
//    ����Ϊ�����Զ������ĵ���XML��ʽ˵�� ��
// ---------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// <summary> 
/// �õ���ʽ����ʱ����
/// </summary> 
/// <param name=" pOutBuffer ">
/// ���������ڴ��ʱ�������
/// </param>
/// <param name=" nType">
/// ��������ͣ���ͬ�����������ͬ�ĸ�ʽ��0��MM-DD HH24:MI:SS.MS�� 1��MMDDMI��
/// </param>
/// <returns>
/// ��
/// </returns>
//-------------------------------------------------------------------------------------------------
void CLog::getTime(char * pOutBuffer,int nType)
{
	time_t   nTimeP;   
	struct   tm   *pTm;   
	time(&nTimeP);   
	pTm=localtime(&nTimeP);
    
	// ����type�õ���ͬ��ʽ����ʱ����
	if(nType == 0) 
		snprintf(pOutBuffer,MAXARRLEN-1,"%02d-%02d %02d:%02d:%02d",1+pTm->tm_mon,pTm->tm_mday,pTm->tm_hour,pTm->tm_min,pTm->tm_sec);
	else if(nType == 1) 
		snprintf(pOutBuffer,MAXARRLEN-1,"%02d%02d%02d",1+pTm->tm_mon,pTm->tm_mday,pTm->tm_hour);
	else if(nType == 2) 
		snprintf(pOutBuffer,MAXARRLEN-1,"%04d-%02d-%02d %02d:%02d:%02d", pTm->tm_year+1900,1+pTm->tm_mon,pTm->tm_mday,pTm->tm_hour,pTm->tm_min,pTm->tm_sec);
	// ����
}

}//namespace end





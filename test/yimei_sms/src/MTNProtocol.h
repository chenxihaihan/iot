#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#ifndef MTNPROTOCOL_H
#define MTNPROTOCOL_H 

namespace MTNMsgApi_NP
{
	
	typedef unsigned int  DWORD;
	typedef unsigned char BYTE;   
	
	//#define NULL  (void*)0;
	
	/*
	����ͨ����Э��
	*/
	//=====================================================================
	//                         ����ID   
	//=====================================================================
	#define MESSAGE_HEAD_LEN			         12								 //��Ϣͷ	
	
	#define METONE_FIRST_REGISTRY             0xC001      //�ͻ������ע�ᣨ����1��
	#define METONE_SECOND_REGISTRY            0xC002      //�ͻ��������ҵ��Ϣע��
	#define METONE_QUERY_BALANCE              0xC003      //�ͻ��������ѯ���
	#define METONE_CHARGE_UP                  0xC004      //�ͻ��������ֵ
	#define METONE_SEND_SMS                   0xC015      //�ͻ���������Ͷ���
	#define METONE_SEND_SMS_2                 0xC025      //�ͻ���������Ͷ���2	
	#define METONE_SEND_SMS_EX                0xC035      //�ͻ���������Ͷ���EX	
	#define METONE_SEND_SMS_EX_2              0xC045      //�ͻ���������Ͷ���EX2	
	#define METONE_SEND_SCHEDULED_SMS         0xC016      //�ͻ���������Ͷ�ʱ����
	#define METONE_SEND_SCHEDULED_SMS_2       0xC026      //�ͻ���������Ͷ�ʱ����2
	#define METONE_SEND_SCHEDULED_SMS_EX      0xC036      //�ͻ���������Ͷ�ʱ����Ex
	#define METONE_SEND_SCHEDULED_SMS_EX_2    0xC046      //�ͻ���������Ͷ�ʱ����Ex2
	#define METONE_RECEIVE_SMS                0xC007      //�ͻ���������ն���
	#define METONE_MO_FORWARD_EX              0xC009      //�ͻ��������ͨMOת��������չ
	#define METONE_CANCEL_MO_FORWARD          0xC00A      //�ͻ������ȡ��MOת������
	#define METONE_QUERY_EACH_FEE             0xC00B      //�ͻ��������ѯ�������ͷ���
	#define METONE_LOGOUT_SN                  0xC00C      //�ͻ������ע��ע�����
	#define METONE_REGISTRY_PWD_UPDATE        0xC00E      //�ͻ������ע����������޸�
	#define ERCP_METONE_GET_STATUS_REPORT_EX  0xC012      //�ͻ���������״̬����(��չ)
	
	//=====================================================================
	//                        Э����Ϣͷ 
	//=====================================================================	
	
	const static int CMDID_REG_1          = 1;//ע��
	const static int CMDID_SEND_SMS       = 2;//���Ͷ���
	const static int CMDID_SEND_SCHE_SMS  = 3;//���Ͷ�ʱ����
	const static int CMDID_CHARGE_UP      = 4;//��ֵ
	const static int CMDID_UN_REG         = 5;//ע�����к�
	const static int CMDID_REG_2          = 6;//��ҵ��Ϣע��
	const static int CMDID_PWD_UP         = 7;//�����޸�
	const static int CMDID_CANCAL_TRANS   = 8;//ȡ��ת��
	
	const static int CMDID_REG_TRANS      = 10;//ע��ת��
	const static int CMDID_BALANCE        = 11;//��ѯ���
	const static int CMDID_PRICE          = 12;//��ѯ����
	const static int CMDID_RECV_SMS       = 13;//���ն���
	const static int CMDID_RECV_REPORT    = 14;//����״̬����
	const static int CMDID_HEART_BAG      = 15;//��������
            
	const static int MAX_SMS_BODY         = 10;//�����������
	const static int MAX_REPORT_BODY      = 40;//�����������
	
	const static int MAX_MSG_BODY_LEN     = 16*1024;//������Ϣ��󳤶�
	const static int MAX_SN_COUNT         = 128;//���ڴ��д洢�����SN������
	const static int SLEEP_SECOND_20      = 20;
	const static int SLEEP_SECOND_10      = 10;


	
	class RecvMsgBody
	{
	public:
		char m_strResult[10];
		char m_strExpress[2048];
		char m_strCnt[16*2048];
		char m_strCommand[8];
		char m_strKeyid[15];
		int  m_nResult;
		DWORD m_nCommandId;
		DWORD m_nSequenceNum;
	  bool m_bSet;

		RecvMsgBody()
		{
			memset(m_strResult,0,sizeof(m_strResult));
			memset(m_strExpress,0,sizeof(m_strExpress));
			memset(m_strCnt,0,sizeof(m_strCnt));
			memset(m_strCommand,0,sizeof(m_strCommand));
			memset(m_strKeyid,0,sizeof(m_strKeyid));
			m_nResult = 0;	
			m_nCommandId = 0;
			m_nSequenceNum = 0;
			m_bSet = false;
		};
		// �������캯�� 
		RecvMsgBody(const RecvMsgBody &other) 
		{ 
			memcpy(m_strResult,other.m_strResult,sizeof(m_strResult));
			memcpy(m_strExpress,other.m_strExpress,sizeof(m_strExpress));
			memcpy(m_strCnt,other.m_strCnt,sizeof(m_strCnt));
			memcpy(m_strCommand,other.m_strCommand,sizeof(m_strCommand));
			memcpy(m_strKeyid,other.m_strKeyid,sizeof(m_strKeyid));
			m_nResult = other.m_nResult;
			m_nCommandId = other.m_nCommandId;
			m_nSequenceNum = other.m_nSequenceNum;
			m_bSet = other.m_bSet;
		};
		// ��ֵ���� 
		RecvMsgBody& operator=(const RecvMsgBody &other) 
		{ 
			// (1) ����Ը�ֵ 
			if(this == &other) 
				return *this; 
			// (2) �ͷ�ԭ�е��ڴ���Դ 
			//delete [] m_data; 
			// (3) �����µ��ڴ���Դ������������ 
			//int length = strlen(other.m_data); 
			//m_data = new char[length+1]; 
			//strcpy(m_data, other.m_data); 
			memcpy(m_strResult,other.m_strResult,sizeof(m_strResult));
			memcpy(m_strExpress,other.m_strExpress,sizeof(m_strExpress));
			memcpy(m_strCnt,other.m_strCnt,sizeof(m_strCnt));
			memcpy(m_strCommand,other.m_strCommand,sizeof(m_strCommand));
			memcpy(m_strKeyid,other.m_strKeyid,sizeof(m_strKeyid));
			m_nResult = other.m_nResult;
			m_nCommandId = other.m_nCommandId;
			m_nSequenceNum = other.m_nSequenceNum;
			m_bSet = other.m_bSet;
			// (4)���ر���������� 
			return *this; 
		};
	};	
	//======================================================================
	//                     Э��������Ӧ��Ϣ��
	//======================================================================
	//����״̬�������Ϣ����չ  ��sequenceid
	class RecvReportBody
	{
	public:
		char m_strSequenceId[15];
		char m_strMobile[21];
		char m_strSubmitdate[15];
		char m_strRecieverdate[15];
		char m_strErrorCode[25];
		char m_strMemo[50];
		char m_strServiceCodeAdd[15];
		char m_strReportType[2];

		RecvReportBody()
		{
			memset(m_strSequenceId,0,sizeof(m_strSequenceId));
			memset(m_strMobile,0,sizeof(m_strMobile));
			memset(m_strSubmitdate,0,sizeof(m_strSubmitdate));
			memset(m_strRecieverdate,0,sizeof(m_strRecieverdate));
			memset(m_strErrorCode,0,sizeof(m_strErrorCode));
			memset(m_strMemo,0,sizeof(m_strMemo));
			memset(m_strServiceCodeAdd,0,sizeof(m_strServiceCodeAdd));
			memset(m_strReportType,0,sizeof(m_strReportType));
		};
		// �������캯�� 
		RecvReportBody(const RecvReportBody &other) 
		{ 
			memcpy(m_strSequenceId,other.m_strSequenceId,sizeof(m_strSequenceId));
			memcpy(m_strMobile,other.m_strMobile,sizeof(m_strMobile));
			memcpy(m_strSubmitdate,other.m_strSubmitdate,sizeof(m_strSubmitdate));
			memcpy(m_strRecieverdate,other.m_strRecieverdate,sizeof(m_strRecieverdate));
			memcpy(m_strErrorCode,other.m_strErrorCode,sizeof(m_strErrorCode));
			memcpy(m_strMemo,other.m_strMemo,sizeof(m_strMemo));
			memcpy(m_strServiceCodeAdd,other.m_strServiceCodeAdd,sizeof(m_strServiceCodeAdd));
			memcpy(m_strReportType,other.m_strReportType,sizeof(m_strReportType));
		};
		// ��ֵ���� 
		RecvReportBody& operator=(const RecvReportBody &other) 
		{ 
			// (1) ����Ը�ֵ 
			if(this == &other) 
				return *this; 
			// (2) �ͷ�ԭ�е��ڴ���Դ 
			//delete [] m_data; 
			// (3) �����µ��ڴ���Դ������������ 
			//int length = strlen(other.m_data); 
			//m_data = new char[length+1]; 
			//strcpy(m_data, other.m_data); 
			memcpy(m_strSequenceId,other.m_strSequenceId,sizeof(m_strSequenceId));
			memcpy(m_strMobile,other.m_strMobile,sizeof(m_strMobile));
			memcpy(m_strSubmitdate,other.m_strSubmitdate,sizeof(m_strSubmitdate));
			memcpy(m_strRecieverdate,other.m_strRecieverdate,sizeof(m_strRecieverdate));
			memcpy(m_strErrorCode,other.m_strErrorCode,sizeof(m_strErrorCode));
			memcpy(m_strMemo,other.m_strMemo,sizeof(m_strMemo));
			memcpy(m_strServiceCodeAdd,other.m_strServiceCodeAdd,sizeof(m_strServiceCodeAdd));
			memcpy(m_strReportType,other.m_strReportType,sizeof(m_strReportType));
			// (4)���ر���������� 
			return *this; 
		};		
		
	};	
	//======================================================================
	//                       Э��������Ϣ��
	//======================================================================
	//���ն��ŵ���Ӧ��Ϣ��
	class RecvSMSBody
	{
	public:
		char m_strSMSMobile[21];    //�����ߣ��������ֻ��û�Ҳ��������ҵ������룩
		char m_strAddiSerial[15];   //���з������ĸ��Ӻ���
		char m_strAddSerialRecv[15]; //���з������ĸ��Ӻ���(������)
		char m_strSMSContent[255];  //��������
		char m_strSMSTime[15];      //����ʱ�䣨��ʽ��yyyymmddhhnnss��
		char m_strCHANNELNUMBER[31];

		RecvSMSBody()
		{
			memset(m_strSMSMobile,0,sizeof(m_strSMSMobile));
			memset(m_strAddiSerial,0,sizeof(m_strAddiSerial));
			memset(m_strAddSerialRecv,0,sizeof(m_strAddSerialRecv));
			memset(m_strSMSContent,0,sizeof(m_strSMSContent));
			memset(m_strSMSTime,0,sizeof(m_strSMSTime));
			memset(m_strCHANNELNUMBER,0,sizeof(m_strCHANNELNUMBER));
		};
		// �������캯�� 
		RecvSMSBody(const RecvSMSBody &other) 
		{ 
			memcpy(m_strSMSMobile,other.m_strSMSMobile,sizeof(m_strSMSMobile));
			memcpy(m_strAddiSerial,other.m_strAddiSerial,sizeof(m_strAddiSerial));
			memcpy(m_strAddSerialRecv,other.m_strAddSerialRecv,sizeof(m_strAddSerialRecv));
			memcpy(m_strSMSContent,other.m_strSMSContent,sizeof(m_strSMSContent));
			memcpy(m_strSMSTime,other.m_strSMSTime,sizeof(m_strSMSTime));
			memcpy(m_strCHANNELNUMBER,other.m_strCHANNELNUMBER,sizeof(m_strCHANNELNUMBER));
		};
		// ��ֵ���� 
		RecvSMSBody& operator=(const RecvSMSBody &other) 
		{ 
			// (1) ����Ը�ֵ 
			if(this == &other) 
				return *this; 
			// (2) �ͷ�ԭ�е��ڴ���Դ 
			//delete [] m_data; 
			// (3) �����µ��ڴ���Դ������������ 
			//int length = strlen(other.m_data); 
			//m_data = new char[length+1]; 
			//strcpy(m_data, other.m_data); 
			memcpy(m_strSMSMobile,other.m_strSMSMobile,sizeof(m_strSMSMobile));
			memcpy(m_strAddiSerial,other.m_strAddiSerial,sizeof(m_strAddiSerial));
			memcpy(m_strAddSerialRecv,other.m_strAddSerialRecv,sizeof(m_strAddSerialRecv));
			memcpy(m_strSMSContent,other.m_strSMSContent,sizeof(m_strSMSContent));
			memcpy(m_strSMSTime,other.m_strSMSTime,sizeof(m_strSMSTime));
			memcpy(m_strCHANNELNUMBER,other.m_strCHANNELNUMBER,sizeof(m_strCHANNELNUMBER));
			// (4)���ر���������� 
			return *this; 
		};		
	};			
	//��ҵ��Ϣע����Ϣ��
	typedef	struct MetoneEntityRegistryBody{
		DWORD messageLength;  //��Ϣ���ܳ���(�ֽ�)
		DWORD commandId;      //����ID
		DWORD sequenceNum;    //���кţ�����ѭ������
		char SerialID[30];      //ע����루��MD5��
		char ServiceTypeID[2];  //�������ͱ��루����д�� 
		char HDSerial[33];      //Ӳ�����кţ�MD5��
		char EName[60];         //��ҵ����
		char ELinkMan[20];      //��ϵ��
		char EPhone[20];        //�绰
		char EMobile[15];       //�ƶ��绰
		char EEmail[60];        //����
		char EFax[20];          //����
		char EAddress[60];      //��ַ
		char EPostcode[6];      //�ʱ�
		int GetLen() 
		{
			return (12 + 30 + 2 + 33 + 60 + 20 + 20 + 15 + 60 + 20 + 60 + 6);
		};
	}MetoneEntityRegistryBody;
	
	
	//���ͼ�ʱ������Ϣ��
	typedef	struct MetoneSendSMSBody
	{
		DWORD messageLength;  //��Ϣ���ܳ���(�ֽ�)
		DWORD commandId;      //����ID
		DWORD sequenceNum;    //���кţ�����ѭ������
		DWORD SMSCount;         //������������ȡֵ��Χ1��1000�������ֶο����ظ�
		char SMSPriority[2];
		char SerialID[30];      //ע����루��MD5��
		char HDSerial[33];      //Ӳ�����кţ�MD5��
		char AddiSerial[15];    //���з������ĸ�������
		char SMSContent[1002];   //��������
		
		char sequenceNum1[32];  //yyyymmddhhmmss + ���� + �û��Զ���8λ
		char SMSMobile[15000];  //�����ߣ��������ֻ��û�Ҳ��������ҵ������룬
	                            //����֮��ʹ�ö��Ÿ���),���ȡ�15 * SMSCount
		int GetLen(int count) 
		{
			return (16 + 2 + 30 + 33 + 15 + 1002 +  32 + count);
		};
	}MetoneSendSMSBody;
	
	
	const static int SERVICECODE_LEN = 6;										//���з�����볤��
	const static int XLT_LEN = 10;													//С��ͨ��̳���
	const static int XLT_LONG_LEN = 12;											//С��ͨ�����
	const static int MOBILE_LEN = 11;												//�ֻ����볤��
	const static int MOBILE_LONG_LEN = 13;									//�ֻ����볤��
	
	//������볣��
	const static int SERIAL_ERROR = 100;					//���к���Ϊ�ջ���Ч
	const static int NET_ERROR = 101;							//�������
	const static int OTHER_ERROR = 102;						//��������
	const static int ENTITYINFO_ERROR = 103;			//ע����ҵ������Ϣʧ�ܣ������ע�����ע��ɹ�
	const static int NOTENOUGHINFO_ERROR = 104;		//ע����Ϣ��д������
	const static int PARAMISNULL_ERROR = 105;			//����balanceָ��Ϊ��
	const static int CARDISNULL_ERROR = 106;			//���Ż�����Ϊ�ջ���Ч
	const static int MOBILEISNULL_ERROR = 107;		    //�ֻ�����Ϊ��
	const static int MOBILESPEA_ERROR = 108;			//�ֻ�����ָ���Ų���ȷ
	const static int MOBILEPART_ERROR = 109;			//�����ֻ����벻��ȷ����ɾ���������ֻ����뱻����
	const static int SMSCONTENT_ERROR = 110;			//��������Ϊ�ջ򳬳���70�����֣�
	const static int ADDICODE_ERROR = 111;				//���Ӻ��������8λ��
	const static int SCHTIME_ERROR = 112;					//��ʱʱ��Ϊ�ջ��ʽ����ȷ(YYYYMMDDHHNNSS)
	const static int PWD_ERROR = 113;							//�������������Ϊ��
	const static int FLAG_ERROR = 114;						//�õ���ʶ����
	const static int FEE_ERROR = 201;							//�Ʒ�ʧ��
	const static int SUCCESS = 1;									//ok
	const static int SUCCESS_LEFT = 2;						//�ɹ�,���ж���Ϣ����
	const static int FAILURE = 0;									//ʧ��
	const static int UNKNOW_ERROR = -1;						//δ֪����
	//const static char* SERVICETYPEID = "01";      //�������ͣ����ŷ���
	const static int ADDI_LEN = 10;               //���Ӻ�����󳤶�
	
	const  static int SOCKET_ERROR = -1;
	const  static int INVALID_SOCKET = -1;
	const  static int FUN_CALL_TOO_FAST = 999;  //�������ù���
	const  static int NOT_RECV_SEND_SMS_RESULT = 997;  //���Ͷ��Ű���û�еõ����ŷ��ͽ�����������Ƿ�ɹ�δ֪��
  const  static int SEQNUM_ERROR=120;            //����ID��ʽ��

	



}//namespace end.


#endif




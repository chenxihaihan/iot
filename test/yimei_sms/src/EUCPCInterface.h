/**
	������͸������
*/
#ifndef EUCPCINTERFACE_H
#define EUCPCINTERFACE_H 

namespace MTNMsgApi_NP
{
	
	//���ն���ʱʹ�õĻص�����
	typedef void  MTNRecvContent(char* mobile,char* senderaddi, char* recvaddi,	
									char* ct,char* sd,int* flag); 
	//���ն���ʱʹ�õĻص�����(��ͨ����)
	typedef void  MTNRecvContentCHL(char* mobile,char* senderaddi, char* recvaddi,
								   char* ct,char* sd,char* chl,int* flag);

	//���ն���״̬����ʱʹ�õĻص�����
	typedef void MTNGetStatusReport(char *mobile, char *errorcode,char *senderaddi,char *reptype, int *flag);
	//ע����Ϣ
	
	//���ա����Ͷ��Ž����ʱʹ�õĻص�����
	typedef void  MTNSendResultCallBack(void* sequencenum,int* sendresult);
	
	
	//zft add 2005-11-04
	//����Ϊ����״̬������չ�Ļص�����
	
	//����״̬����ʹ�õĻص�����
	typedef void MTNGetStatusReportEx(char *seqnum, char *mobile, char *errorcode, 
										   char *senderaddi,
										   char *reptype, int *flag);
	
	
	//1.Register 10 para.
	int Register(char* sn,char* pwd,char* EntName,char* LinkMan,char* Phone,char* Mobile,char* Email,char* Fax,char* sAddress,char* Postcode);
	//����ʻ����(length 10)
	//2.GetBalance  2 para.
	int GetBalance(char* sn,char* balance);
	//���ų�ֵ
	//3.ChargeUp  3 para.
	int ChargeUp(char* sn,char* acco,char* pass);
	
	int RegistryTransfer(char* sn,char* phone);
	//ȡ��ת�ƹ���
	int CancelTransfer(char* sn);
	
	int GetPrice(char* sn,char* balance); 
	
	
	//���Ͷ���Ϣ
	//���Ͷ���Ϣ��EUCPƽ̨
	int SendSMS(char* sn,char* mn,char* ct,char* priority);
	int SendSMS2(char* sn,char* mn,char* ct, char* seqnum,char* priority);
	//���Ͷ���Ϣ��EUCPƽ̨,���Դ����Ӻ���
	int SendSMSEx(char* sn,char* mn,char* ct,char* addi,char* priority);
	//���Ͷ���Ϣ��EUCPƽ̨,���Դ����Ӻ���
	int SendSMSEx2(char* sn,char* mn,char* ct,char* addi, char* seqnum,char* priority); 
	//���Ͷ���Ϣ��EUCPƽ̨ ͨ�ú���
	int SendCommSMS(char* sn,char* mn,char* ct,char* addi, char* seqnum,char* priority);
	
	
	//���Ͷ�ʱ����,ʱ���ʽyyyymmddhhnnss
	int SendScheSMS(char* sn,char* mn,char* ct,char* sendtime,char* priority);
	int SendScheSMS2(char* sn,char* mn,char* ct, char* sendtime,char* seqnum,char* priority);
	int SendScheSMSEx(char* sn,char* mn,char* ct,char* sendtime,char* addi,char* priority);
	int SendScheSMSEx2(char* sn,char* mn,char* ct,char* sendtime,char* addi, char* seqnum,char* priority);
	//���Ͷ���Ϣ��EUCPƽ̨ ͨ�ú���
	int SendCommScheSMS(char* sn,char* mn,char* ct,char* sendtime,char* addi, char* seqnum,char* priority);
	
	//���ն���Ϣ
	int ReceiveSMS(char* sn, MTNRecvContent* rcex);
	int ReceiveSMSEx(char* sn, MTNRecvContent* rcex);
	int ReceiveSMSCHL(char* sn, MTNRecvContentCHL* rcex);//��ͨ����
	
	//����״̬����
	int ReceiveStatusReportEx(char* sn, MTNGetStatusReportEx* Stax); 
	
	//ע��ע��
	int UnRegister(char* sn);
	
	//�޸�������кŶ�Ӧ������
	int RegistryPwdUpd(char* sn, char* oldpwd, char* newpwd);
	
	//���ùؼ���
	int  SetKey(char* key);
	
	void  GetSDKVersion(char* ver);

	//
	int SetLog(char*path,char*name,int size,int mask);
	int Close();
	
	
}//namespace end.

#endif




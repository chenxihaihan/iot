
#include "EUCPCInterface.h"

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
	
using namespace std;  

using namespace MTNMsgApi_NP; 

void parseMsg(int result)
{
	if (result == 1)
		printf("�ɹ� Return=0\n");
	else if (result == -1)
		printf("ϵͳ�쳣 Return=-1\n");
	else if (result == -111)
		printf("����������:��ҵ��Ϣע��ʧ��\n");
	else if (result == -124)
		printf("����������:��ѯ���ʧ��\n");
	else if (result == -113)
		printf("����������:��ֵʧ��\n");
	else if (result == -117)
		printf("����������:������Ϣʧ��\n");
	else if (result == -122)
		printf("����������:ע��ʧ��\n");
	else
	{
		printf("��������ֵ:");
		printf("%d\n",result);
	}
	return;
}
/////////////////////////////////////////////////////////////////////////
//���ն�����Ҫ���û��Զ���Ļص�����
// ʾ������Ϊֱ�����������̨
// mobile:�ֻ���
// senderaddi:�����߸��Ӻ�
// recvaddi:�����߸��Ӻ�
// ct:��������
// sd:����
// *flag : ��־��Ϊ 1 ��ʾ�ж���
// ���ж��������Ҫ����,��Ҫ����ҵ���߼�����,�磺�������ݿ⣬д�ļ��ȵ�
////////////////////////////////////////////////////////////////////////
void MTNRecvContentA_test(char * mobile,char* senderaddi, char* recvaddi,char* ct,char* sd,int* flag)
{
	if(*flag)
	{
		printf("mobile:%s\n",mobile);
		printf("senderaddi:%s\n",senderaddi);
		printf("recvaddi:%s\n",recvaddi);
		printf("ct:%s\n",ct);
		printf("sd:%s\n",sd);
		printf("\n\n\n");
	}

	return;
}

/////////////////////////////////////////////////////////////////////////
//���ն���״̬������Ҫ���û��Զ���Ļص�����
// ʾ������Ϊֱ�����������̨
// seqnum:���к�
// mobile:�ֻ���
// errorcode:������
// senderaddi:�����߸��Ӻ�
// reptype:��������
// *flag : ��־��Ϊ 1 ��ʾ�ж���
// ���ж��������Ҫ����,��Ҫ����ҵ���߼�����,�磺�������ݿ⣬д�ļ��ȵ�
////////////////////////////////////////////////////////////////////////
void MTNGetStatusReportEx_test(char *seqnum, char *mobile, char *errorcode, 
										   char *senderaddi,
										   char *reptype, int *flag)
{
	if(*flag)
	{
		printf("seqnum:%s\n",seqnum);	
		printf("mobile:%s\n",mobile);
		printf("errorcode:%s\n",errorcode);
		printf("senderaddi:%s\n",senderaddi);
		printf("reptype:%s\n",reptype);
		printf("\n\n\n");
	}

	return;	
}



//API �����ṹ��							   
struct API_PARA
{
	//ͨ����Ϣ
	char sn[100];          //���к�
	char pwd[50];          //����
	char pwd2[50];         //������
	char cardcode[100];    //��ֵ����
	char cardpwd[50];      //��ֵ������
	char sechdueTime[100]; //��ʱʱ��(������ڵ�ǰʱ���Сʱ��С��һ������)
	
	//��ҵע��
	char EntName[100];   //��ҵ����
	char LinkMan[100];   //��ϵ��
	char Phone[10*1024]; //�ֻ�����(�԰�Ƕ��� "," �ָ������ܳ���1000������)
	char Mobile[100];    //��ϵ���ֻ���(����Ϊ�Ϸ����ֻ�����)
	char Email[100];     //��ϵ��Email(�����ַ��ʽ����Ϸ�)
	char Fax[100];       //��ϵ�˴������(����Ϊ�Ϸ��ĵ绰����)
	char sAddress[100];  //��ϵ�˵�ַ
	char Postcode[100];  //��ϵ����������
	
	char Balance[50];    //���
	char Price[50];      //���ŵ���(0.1Ԫ/��)
	char Verison[50];	 //SDK�汾��
	
	//
	char TransferPhone[200]; //ע��ת�ӵ��ֻ�����
	char key[100];           //����Keyֵ���ַ���
	
	//�ֻ����Ͷ���
	char mn[20*1024]; //�ֻ�����
	char ct[200];     //��������
	char addi[20];    //ͨ����չ����
	int  seqid;       //����ID
	char priority[10];//�������ȼ�(1��5֮�䣬5Ϊ������ȼ�)
}; 



//������ʾ
int showHelp()
{
		cout << "**********Select number for test call**********" << endl;
		cout << "0  for exit"<<endl;
		cout << "1  for Help"<<endl;
		cout << "2  for Get SDK Version" << endl;
		cout << "3  for Set Key" << endl;
		cout << "4  for Register" << endl;
		cout << "5  for Change Pwd" << endl;
		cout << "6  for Charge Up" << endl;
		cout << "7  for Get Balance" << endl;
		cout << "8  for Get Price" << endl;
		cout << "9  for UnRegister" << endl;
		cout << "10 for Registry Transfer" << endl;
		cout << "11 for Cancel Transfer" << endl;
		cout << "********** test send sms**********" << endl;
		cout << "12 for SendSMS()" << endl;
		cout << "13 for SendSMSEx()" << endl;
		cout << "14 for SendScheSMS()" << endl;
		cout << "15 for SendScheSMSEx()" << endl;
		cout << "********** test recv sms**********" << endl;
		cout << "16 for ReceiveSMS()" << endl;
		cout << "17 for ReceiveSMSEx()" << endl;
		cout << "18 for ReceiveStatusReportEx()" << endl;
		cout << "********** test send sms with seqid**********" << endl;
		cout << "19 for SendSMSEx2()" << endl;
		cout << "********** close**********" << endl;
		cout << "20 for Close()" << endl;		
		return 0;
}


//����API����
int setParam(API_PARA &param)
{
	sprintf(param.sn,"%s","8SDK-EMY-6699-RJYNR");
	sprintf(param.pwd,"%s","834693");

	sprintf(param.pwd2,"%s","834693");
	
	sprintf(param.cardcode,"%s","AAA-BBB-CCC");
	sprintf(param.cardpwd,"%s","20080808");	

	sprintf(param.EntName,"%s","1");
	sprintf(param.LinkMan,"%s","1");		
	sprintf(param.Phone,"%s","1");		
	sprintf(param.Mobile,"%s","1");		
	sprintf(param.Email,"%s","1");		
	sprintf(param.Fax,"%s","1");		
	sprintf(param.sAddress,"%s","1");		
	sprintf(param.Postcode,"%s","1");	
	
	sprintf(param.TransferPhone,"%s","13322224444,13566667777,1234");	
	
	
	//�ֻ����Ͷ���		
	sprintf(param.mn,"%s","13480669285");				
	sprintf(param.ct,"%s","Hello,���԰���");
	
	sprintf(param.sechdueTime,"%s","20171023105501");	

	sprintf(param.addi,"%s","123");	
	sprintf(param.priority,"%s","3");		
		
	return 0;
}




//���������
int main()
{
	
#ifdef EUCPC_DEBUG    		
	//SetLog(NULL,"EUCPCLog",1024*1024*10,0x0020);//10M����־����־����WR_EUCPCLog.txt
#endif
	
	API_PARA param;
	memset(&param,0,sizeof(API_PARA));
	
	//���ò���
	setParam(param);

	int ret = 0;//false;
	
	//��ʾ����
	showHelp();
	
	
	while(1)
	{
		cout << "input choice:" ;
		string strTemp;
		cin >> strTemp;
		
		int input_num;
		input_num = atoi(strTemp.c_str());
		//cout << "input_num=" << input_num << endl;
		switch (input_num)
		{
			case 0:
				cout << "realy want quit?(y/n)" << endl;
				cin >> strTemp;
				if(strTemp[0] == 'y' || strTemp[0] == 'Y')
					return 0;
				else
					break;
			case 1:
				showHelp();
				cout << "input choice:" ;
				break;				
			case 2:
				/**
				* ��ѯ���
				* GetSDKVersion(����汾��)
				* ������汾���ַ�����:����20���ֽ�
				*/
				//��ѯ�汾��     
				GetSDKVersion(param.Verison);
				printf("GetSDKVersion() = %s\n",param.Verison);
				break;
			case 3:
				cout << "input Key:";
				cin >> param.key;
				
				/**
				* ����Key
				* SetKey���Զ�����ַ�����
				* ���������к���Ҫ�ڶ�̨������ʹ�ã���Ҫ�ȵ��øú���һ�Σ��ٷ��Ͷ���
				* 
				*/				
				//����Key(���ڶ�̨��������ͬһ�����к�)������� ͳһ�Զ�����ַ���   
				ret = SetKey(param.key);
				parseMsg(ret);
				break;
			case 4:
				/*
				cout << "input register sn:";
				cin >> param.sn;
				cout << "input register pwd:";
				cin >> param.pwd;
				
				
				cout << "input Ent Name:";
				cin >> param.EntName;
				cout << "input Link Man:";
				cin >> param.LinkMan;
				cout << "input Mobile:";
				cin >> param.Mobile;
				cout << "input Email:";
				cin >> param.Email;
				cout << "input Fax:";
				cin >> param.Fax;
				cout << "input Address:";
				cin >> param.sAddress;
				cout << "input Post code:";
				cin >> param.Postcode;
				*/
				
				/**
				 * ���к�ע��
				 * �ڼ�����Ϸ����ű�����ע���������к�
				 * ע�⣺�˺���ֻ��ִ��һ�μ��ɣ�ֱ���������кŻ�����ļ������ʱ����Ҫ����ע�ᡣ
				 * Register(���к�,����,��ҵ����,��ϵ������,��ϵ�绰,��ϵ�ֻ�,�����ʼ�, ��ϵ����,��˾��ַ, ��������)
				 * �����У� 
				 *�����кš�  ͨ������������Ա��ȡ
				 *�����롿    ͨ������������Ա��ȡ������Ϊ6���ֽ� 
				 *����ҵ���ơ����60�ֽڣ���������
				 *����ϵ��������(���20�ֽ�)����������
				 *����ϵ�绰��(���20�ֽ�)����������
				 *����ϵ�ֻ���(���15�ֽ�)����������
				 *�������ʼ���(���60�ֽ�)����������
				 *����ϵ���桿(���20�ֽ�)����������
				 *����˾��ַ��(���60�ֽ�)����������
				 *���������롿(���6�ֽ�)����������			
				*/				
				ret = Register(param.sn,param.pwd,
								param.EntName,
								param.LinkMan,param.Phone,
								param.Mobile,param.Email,param.Fax,
								param.sAddress, param.Postcode);
				parseMsg(ret);
				break;	
			case 5:
				cout << "input old pwd:";
				cin >> param.pwd;
				cout << "input newpwd:";
				cin >> param.pwd2;
					
				/**
				* �޸����к�����
				* RegistryPwdUpd(������к�,������, ������);
				* �����롿����Ϊ6���ֽ�
				*/									
				//�޸�����   �������������к� ������   ������
				ret = RegistryPwdUpd(param.sn,param.pwd,param.pwd2);
				parseMsg(ret);
				break;	
				
			case 6:			
				cout << "input card code:";
				cin >> param.cardcode;
				cout << "input card pwd:";
				cin >> param.cardpwd;
							
				/**
				* ��ֵ
				* chargeUp(������кţ���ֵ������, ����) 
				* ��ͨ������������Ա��ȡ���ź����� 
				* ����ֵ�����š�����Ϊ20���ֽ�����
				* �����롿����Ϊ6���ֽ�
				* ������㣬���Ͷ��Ż�ʧ��
				*/	
				//��ֵ  �������������к�   �������š���   ����								
				ret = ChargeUp(param.sn,param.cardcode,param.cardpwd);
				parseMsg(ret);
				break;	
				
			case 7:
				memset(param.Balance,0,sizeof(param.Balance));
				
				/**
				* ��ѯ���
				* ���㣬���Ͷ��Ż�ʧ��
				* GetBalance(������к�,���);
				* ������ַ���������10���ֽ�
				*/	
				//��ѯ���   �������������к�  ���			
				ret = GetBalance(param.sn,param.Balance);
				parseMsg(ret);
				if(0 != strlen(param.Balance))
					printf("balance:%s\n",param.Balance);
				else
					printf("balance: null\n");
				break;	
				
			case 8:
				/**
				* ��ѯ���ŵ�������
				* GetPrice(������к�,�۸�);
				* ���۸��ַ���������20���ֽ�
				*/			
				//��ѯ���� �������������к�,�۸�
				ret = GetPrice(param.sn,param.Price);
				parseMsg(ret);
				if(0 != strlen(param.Price))
					printf("price per sms:%s\n",param.Price);
				else
					printf("price per sms: null\n");
				break;	
			case 9:
				/**
				* ע��������к�
				* UnRegister(������к�)
				* ����Ҫ������һ̨�����Ϸ�����ʱ����Ҫ�Ƚ�������������к�ע�������ڱ�Ļ�����ע��󣬲��ܼ���ʹ��
				*/			
				//ע��    �������������к�
				ret = UnRegister(param.sn);
				parseMsg(ret);
				break;	
				
			case 10:
				cout << "input RegistryTransfer phone(split with ','):";
				cin >> param.TransferPhone;
				
				/**
				* ע��ת��
				* �����Ҫ�����ж���ת�����ֻ��Ͻ��գ���Ҫ���øú�����
				* RegistryTransfer��������кţ��ֻ��ţ�
				* ���ֻ��š����10�����ֻ������԰�Ƕ��ŷָ�
				*/
				//ע��ת�ӵ��ֻ���  �������������к�    �ֻ���
				ret = RegistryTransfer(param.sn,param.TransferPhone);
			    parseMsg(ret);
				break;	
			case 11:
				/**
				* ȡ��ע��ת��
				* �����Ҫȡ�����ж���ת�����ֻ��ϣ���Ҫ���øú�����
				* CancelTransfer��������кţ�
				* 
				*/			
				//ȡ��ע��ת�ӵ��ֻ���  �������������к�
				ret = CancelTransfer(param.sn);
				parseMsg(ret);
				break;	
			case 12:
				/**
				* ���ͼ�ʱ���� 
				* sendSMS(������к�, �ֻ�����,��������,���ȼ�)
				* 
				* [�ֻ�����]Ϊ�ֻ������ַ���,�԰�Ƕ��ŷָ�,���С��200���ֻ�����
				* [��������]Ϊ��������,��󳤶�Ϊ500��
				* (�����ֺ�����Ӣ��),��������ƽ̨�����ʵ��ͨ���Զ����,�Ʒ���ʵ�ʲ������Ϊ׼.�����Ƽ����ų���70������
				* [���ȼ�]�������ȼ�����Χ1~5����ֵԽ�����ȼ�Խ��
				*/		
				//��ʱ���� �������������к�,�ֻ���,�������� ,���ȼ�
				ret = SendSMS(param.sn,param.mn,param.ct,param.priority);
		    	parseMsg(ret);
		    	break;
			case 13:
				/**
				 * ���ͼ�ʱ����(�����Ӻ��룬����չ��)
				 * SendSMSEx(������к�,�ֻ�����,��������, ���Ӻ���,���ȼ�)
				 * 
				 * [�ֻ�����]Ϊ�ֻ������ַ���,�԰�Ƕ��ŷָ�,���С��200���ֻ�����
				 * [��������]Ϊ��������,��󳤶�Ϊ500��
				 * (�����ֺ�����Ӣ��),��������ƽ̨�����ʵ��ͨ���Զ����,�Ʒ���ʵ�ʲ������Ϊ׼.�����Ƽ����ų���70������ 
				 * [���Ӻ���]�û��Զ���ĸ��Ӻ��룬һ��С��6���ֽ�
				 * [���ȼ�]�������ȼ�����Χ1~5����ֵԽ�����ȼ�Խ��
				 */
				//��ʱ����   �������������к�,�ֻ���,��������,���Ӻ���  ,���ȼ�
				ret = SendSMSEx(param.sn,param.mn,param.ct,param.addi ,param.priority);
		    	parseMsg(ret);
		    	break;
			case 14:
				/**
				* ���Ͷ�ʱ����   
				* SendScheSMS(������к�, �ֻ�����,��������, ��ʱʱ��,���ȼ�)
				* 
				* [��ʱʱ��]��ʽΪ:����������������ʱʱ�ַ�����,����:20090504111010 ����2009��5��4�� 11ʱ10��10��,ʱ����ڵ�ǰʱ�䣬С��һ������
				* [�ֻ�����]Ϊ�ֻ������ַ���,�԰�Ƕ��ŷָ�,���С��200���ֻ�����
				* [��������]Ϊ��������,��󳤶�Ϊ500��(�����ֺ�����Ӣ��),��������ƽ̨�����ʵ��ͨ���Զ����
				* ,�Ʒ���ʵ�ʲ������Ϊ׼.�����Ƽ����ų���70������ 
				* [���ȼ�]�������ȼ�����Χ1~5����ֵԽ�����ȼ�Խ��
				*/
				//��ʱ����   �������������к�,�ֻ���,��������   ,��ʱʱ��      ,���ȼ�
				ret = SendScheSMS(param.sn,param.mn,param.ct,param.sechdueTime ,param.priority);
		    	parseMsg(ret);
		    	break;
			case 15:
				/**
				* ���Ͷ�ʱ����(�����Ӻ��룬����չ��) 
				* SendScheSMSEx(������к�, �ֻ�����,��������, ��ʱʱ��,���Ӻ���,���ȼ�)
				* 
				* [��ʱʱ��]��ʽΪ:����������������ʱʱ�ַ�����,����:20090504111010 ����2009��5��4�� 11ʱ10��10��,ʱ����ڵ�ǰʱ�䣬С��һ������
				* [�ֻ�����]Ϊ�ֻ������ַ���,�԰�Ƕ��ŷָ�,���С��200���ֻ�����
				* [��������]Ϊ��������,��󳤶�Ϊ500��(�����ֺ�����Ӣ��),��������ƽ̨�����ʵ��ͨ���Զ����
				* ,�Ʒ���ʵ�ʲ������Ϊ׼.�����Ƽ����ų���70������ 
				* [���Ӻ���]�û��Զ���ĸ��Ӻ��룬һ��С��6λ
				* [���ȼ�]�������ȼ�����Χ1~5����ֵԽ�����ȼ�Խ��
				*/			
				//��ʱ����    �������������к�,�ֻ���,��������   ,��ʱʱ��     ,���Ӻ���       ,���ȼ�
				ret = SendScheSMSEx(param.sn,param.mn,param.ct,param.sechdueTime ,param.addi ,param.priority);
		    	parseMsg(ret);
		    	break;
			case 16:
				/**
				* �õ����ж���
				* ReceiveSMS(������кţ��Զ���Ľ��ջص�����)
				* ���Զ���Ľ��ջص������������յ��Ķ��ţ���ͨ���ص������Ĳ������룬�ں����ڲ����Խ��д�������������ݿ⣬д�ļ��ȡ�
				*/
				//���ն���  �������������к� ,�Զ���Ľ��ջص�����
				ret = ReceiveSMS(param.sn,&MTNRecvContentA_test);
		    	parseMsg(ret);
		    	break;	
			case 17:
				/**
				* �õ����ж���(�����Ӻ���)
				* ReceiveSMSEx(������кţ��Զ���Ľ��ջص�����)
				* ���Զ���Ľ��ջص������������յ��Ķ��ź͸��Ӻ��룬��ͨ���ص������Ĳ������룬�ں����ڲ����Խ��д�������������ݿ⣬д�ļ��ȡ�
				*  
				*/
				//���ն���(�����Ӻ���)  �������������к�,�Զ���Ļص�����
				ret = ReceiveSMSEx(param.sn,&MTNRecvContentA_test);
		    	parseMsg(ret);
		    	break;	
		   case 18:
				/**
				* �õ�����״̬����(�����Ӻ���)
				* ReceiveSMSEx(������кţ��Զ���Ľ��ջص�����)
				* ���Զ���Ľ��ջص������������յ��Ķ��ź͸��Ӻ��룬��ͨ���ص������Ĳ������룬�ں����ڲ����Խ��д�������������ݿ⣬д�ļ��ȡ�
				*  
				*/
				//���ն���(�����Ӻ���)  �������������к�,�Զ���Ļص�����
				ret = ReceiveStatusReportEx(param.sn,&MTNGetStatusReportEx_test);
		    	parseMsg(ret);
		    	break;	
			case 19:
				/**
				 * ���ͼ�ʱ����(�����Ӻ��룬����չ��,����ID)
				 * SendSMSEx(������к�,�ֻ�����,��������, ���Ӻ���,���ȼ�)
				 * 
				 * [�ֻ�����]Ϊ�ֻ������ַ���,�԰�Ƕ��ŷָ�,���С��200���ֻ�����
				 * [��������]Ϊ��������,��󳤶�Ϊ500��
				 * (�����ֺ�����Ӣ��),��������ƽ̨�����ʵ��ͨ���Զ����,�Ʒ���ʵ�ʲ������Ϊ׼.�����Ƽ����ų���70������ 
				 * [���Ӻ���]�û��Զ���ĸ��Ӻ��룬һ��С��6���ֽ�
				 * [���ȼ�]�������ȼ�����Χ1~5����ֵԽ�����ȼ�Խ��
				 */
				//��ʱ����   �������������к�,�ֻ���,��������,���Ӻ���  ,���ȼ�
				static int seqid=1;
				seqid++;
				char seqid_buf[20];
				memset(seqid_buf,0,sizeof(seqid_buf));
				sprintf(seqid_buf,"2222888800%d",seqid);
				cout << "seqid_buf:"<<seqid_buf<<endl;
				ret = SendSMSEx2(param.sn,param.mn,param.ct,param.addi ,seqid_buf ,param.priority);
		    	parseMsg(ret);
		    	break;
			    	
			case 20:
				ret = Close();
		    	parseMsg(ret);
		    	break;	
			default:
				cout << "input error!" << endl;
				break;
		}												
	}
	return 0;
}  





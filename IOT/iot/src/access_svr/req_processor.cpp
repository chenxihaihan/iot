
#include "req_processor.h"
#include "req_mgt.h"
#include "comm/common.h"
#include "protocol.h"
#include "base/base_net.h"
#include "base/base_string.h"
#include "base/base_logger.h"
#include "base/base_url_parser.h"
#include "base/base_uid.h"
#include "base/base_utility.h"
#include "base/base_base64.h"
#include "base/base_cryptopp.h"

#include "msg_oper.h"
#include "conf_mgt.h"
#include "client_mgt.h"
#include "conn_mgt_lb.h"
#include "redis_mgt.h"

//geoip
#include "GeoIP.h"
#include "GeoIPCity.h"

extern Logger g_logger;
extern StSysInfo g_sysInfo;
extern Req_Mgt *g_req_mgt;
GeoIP *g_geoip = NULL;

extern Conn_Mgt_LB g_user_mgt_conn;
extern Conn_Mgt_LB g_device_mgt_conn;
extern Conn_Mgt_LB g_family_mgt_conn;


Req_Processor::Req_Processor()
{

}


Req_Processor::~Req_Processor()
{

}



int Req_Processor::do_init(void *args)
{	
	g_geoip = GeoIP_open("./../conf/GeoLiteCity.dat", GEOIP_INDEX_CACHE);
	if (g_geoip == NULL) 
	{
		XCP_LOGGER_INFO(&g_logger, "GeoIP_open ./../conf/GeoLiteCity.dat failed.\n");
	}
	else
	{
		XCP_LOGGER_INFO(&g_logger, "GeoIP_open ./../conf/GeoLiteCity.dat success.\n");
	}

	return 0;
}




int Req_Processor::svc()
{
	int nRet = 0;
	std::string err_info = "";
	
	//��ȡreq ���Ҵ���
	Request_Ptr req;
	nRet = g_req_mgt->get_req(req);
	if(nRet != 0)
	{
		return 0;
	}

	XCP_LOGGER_INFO(&g_logger, "prepare to process req from app, %s\n", req->to_string().c_str());

	//---------------
	
	//����iot_req
	std::string uuid = "";
	std::string encry = "";
	std::string content = "";
	nRet = XProtocol::iot_req(req->_req, uuid, encry, content, err_info);
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "iot_req faield, ret:%d err_info:%s, req:%s\n", nRet, err_info.c_str(), req->to_string().c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, "null", 0, req->_msg_tag, ERR_INVALID_REQ, err_info);
		return 0;
	}

	
	XCP_LOGGER_INFO(&g_logger, "iot_req, uuid:%s, encry:%s, content:%s\n", 
		uuid.c_str(), encry.c_str(), content.c_str());		

	//---------------

	std::string new_req = "";
	if(encry == "true" && !content.empty())
	{
		new_req = "";
		std::string buf = "";

		//base64����
		X_BASE64 base64;
		char *tmp = NULL;
		if(!base64.decrypt(content.c_str(), tmp))
		{
			XCP_LOGGER_INFO(&g_logger, "base64 faield, content:%s\n", content.c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, "null", 0, req->_msg_tag, ERR_INVALID_REQ, "base64 faield");
			return 0;
		}
		buf = tmp;
		DELETE_POINTER_ARR(tmp);

		//��ȡ��ȫͨ��key
		std::string key = "";
		nRet = PSGT_Redis_Mgt->get_security_channel(uuid, key, err_info);
		if(nRet != 0)
		{
			XCP_LOGGER_INFO(&g_logger, "get_security_channel failed, ret:%d, err_info:%s, uuid:%s\n", 
				nRet, err_info.c_str(), uuid.c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, "null", 0, req->_msg_tag, ERR_INVALID_REQ, err_info);
			return 0;
		}

		XCP_LOGGER_INFO(&g_logger, "get_security_channel success, uuid:%s, key:%s\n", uuid.c_str(), key.c_str());		

		//aes cbc ����
		X_AES_CBC aes;
		if(!aes.decrypt(key, buf, new_req))
		{
			XCP_LOGGER_INFO(&g_logger, "aes cbc failed, ret:%d, key:%s\n", nRet, key.c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, "null", 0, req->_msg_tag, ERR_INVALID_REQ, "aes cbc failed");
			return 0;
		}
		
	}
	else
	{
		new_req = content;
	}
	

	//---------------

	XCP_LOGGER_INFO(&g_logger, "prepare to process new req(%u): %s\n", new_req.size(), new_req.c_str());
	req->_req = new_req;
	
	std::string method = "";
	unsigned long long timestamp = 0;
	unsigned int req_id = 0;
	nRet = XProtocol::req_head(req->_req, method, timestamp, req_id, err_info);
	if(nRet != 0)
	{
		XCP_LOGGER_INFO(&g_logger, "it is invalid req, ret:%d err_info:%s, req:%s\n", nRet, err_info.c_str(), req->to_string().c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, "null", 0, req->_msg_tag, ERR_INVALID_REQ, err_info);
		return 0;
	}

	//����req ��Ϣ
	req->_req_id = req_id;
	req->_app_stmp = timestamp;
	req->_req = XProtocol::add_msg_tag(req->_req, req->_msg_tag);

	if((method == CMD_REGISTER_USER) ||
		(method == CMD_LOGIN_PWD) || 
		(method == CMD_LOGIN_CODE) ||
		(method == CMD_PHONE_CODE)||
		(method == CMD_AUTH_ROUTER)||	
		(method == CMD_AUTH) ||
		(method == CMD_SET_PWD)||
		(method == CMD_RESET_PWD)||
		(method == CMD_GET_USER_PROFILE)||
		(method == CMD_UPDATE_USER_PROFILE)||
		(method == CMD_REFRESH_USER_TOKEN))  
	{
		//�û�����
		XCP_LOGGER_INFO(&g_logger, "--- user mgt:%s ---\n", method.c_str());

		//��ȡuser mgt conn
		Conn_Ptr user_conn;
		if(!g_user_mgt_conn.get_conn(user_conn))
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, ERR_NO_CONN_FOUND, "no user mgt conn.");
			return 0;
		}
		
		std::string rsp = "";
		nRet = user_conn->inner_message(req->_msg_tag, req->_req, rsp, err_info);
		if(nRet != 0)
		{
			XCP_LOGGER_INFO(&g_logger, "get rsp from user mgt svr failed, ret:%d err_info:%s\n", nRet, err_info.c_str());
			Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, nRet, err_info);
			return 0;
			
		}

		//���ظ�ǰ��
		Msg_Oper::send_msg(req->_fd, req->_session_id, rsp);

		//todo: �Ե�¼����֤�ɹ�����Ҫע�������·�ɲ���
		
	
	}
	else if((method == CMD_CREATE_FAMILY)||
	   (method == CMD_UPDATE_FAMILY)||
	   (method == CMD_GET_FAMILY_INFO)||
	   (method == CMD_GET_FAMILY_LIST)||
	   (method == CMD_APPLY_FAMILY)||
	   (method == CMD_ACCEPT_FAMILY)||
	   (method == CMD_GET_APPLY_LIST)||
	   (method == CMD_REMOVE_MEMBER)||
	   (method == CMD_UPDATE_MEMBER)||
	   (method == CMD_REMOVE_MEMBER)||
	   (method == CMD_GET_MEMBER_INFO)||
	   (method == CMD_GET_MEMBER_LIST))
	{
		//��ͥ����
		XCP_LOGGER_INFO(&g_logger, "--- family mgt:%s ---\n", method.c_str());

		//��ȡfamiliy mgt conn
		Conn_Ptr family_conn;
		if(!g_family_mgt_conn.get_conn(family_conn))
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, ERR_NO_CONN_FOUND, "no family mgt conn.");
			return 0;
		}
		
		std::string rsp = "";
		nRet = family_conn->inner_message(req->_msg_tag, req->_req, rsp, err_info);
		if(nRet == 0)
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, rsp);
		}
		else
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, nRet, err_info);
		}
		
	}
	else if((method == CMD_ADD_ROOM )||
	   (method == CMD_DEL_ROOM)||
	   (method == CMD_UPDATE_ROOM)||
	   (method == CMD_GET_ROOM_LIST)||
	   (method == CMD_UPDATE_ROOM_ORDER)||
	   
	   (method == CMD_BIND_ROUTER)||
	   (method == CMD_UNBIND_ROUTER)||
	   (method == CMD_ROUTER_DEVICES)||
	   (method == CMD_AUTH_ROUTER)||
	   
	   (method == CMD_ADD_DEIVCE)||
	   (method == CMD_DEL_DEVICE)||
	   (method == CMD_UPDATE_DEVICE)||
	   (method == CMD_GET_DEVICE_INFO)||
	   (method == CMD_GET_DEVICE_LIST))
	{
		//��ͥ����
		XCP_LOGGER_INFO(&g_logger, "--- device mgt:%s ---\n", method.c_str());

		//��ȡdevice mgt conn
		Conn_Ptr device_conn;
		if(!g_device_mgt_conn.get_conn(device_conn))
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, ERR_NO_CONN_FOUND, "no family mgt conn.");
			return 0;
		}
		
		std::string rsp = "";
		nRet = device_conn->inner_message(req->_msg_tag, req->_req, rsp, err_info);
		if(nRet == 0)
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, rsp);
		}
		else
		{
			Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, nRet, err_info);
		}
		
		
	}	
	else
	{
		XCP_LOGGER_INFO(&g_logger, "invalid method(%s) from app\n", method.c_str());
		Msg_Oper::send_msg(req->_fd, req->_session_id, method, req_id, req->_msg_tag, ERR_INVALID_REQ, "invalid method.");
	}
	
	return 0;
	
}




//��ȡ�û�����λ��
const char *_mk_NA(const char *p)
{
    return p ? p : "N/A";
}



std::string Req_Processor::get_geography(const std::string &ip)
{
	std::string geography = ip;

	if(!g_geoip)
	{
		return geography;
	}  

	//ͨ���������Ʒ���GeoIPRecord�����ڲ����ɷ��أ� ��ҪӦ��ͨ��GeoIPRecord_delete�����ͷ�
	GeoIPRecord *gir = NULL;
	gir = GeoIP_record_by_name(g_geoip, ip.c_str());
	if(gir) 
	{
		//ͨ������IP��ȡIp��Ӧ����������Ϣret[0]Ϊ���뿪ʼ��ַ��ret[1]Ϊ���������ַ
		char **ret = NULL;
		ret = GeoIP_range_by_ip(g_geoip, ip.c_str());
		
		//ͨ�����Һ͵����Ż�ȡʱ����Ϣ
		const char *time_zone = NULL;
		time_zone = GeoIP_time_zone_by_country_and_region(gir->country_code, gir->region);

		//������Ϣ���100Byte
		char buf[1024] = {0};
		snprintf(buf, 1024, "host:%s;country_code:%s;region:%s;region_name:%s;city:%s", 
		ip.c_str(),
		_mk_NA(gir->country_code), //���Ҵ���
	    _mk_NA(gir->region),   //������������
		_mk_NA(GeoIP_region_name_by_code(gir->country_code, gir->region)),	//��ȡ�����������ƣ�����ʡ����
	    _mk_NA(gir->city)	//��������
	    );

		geography = buf;
			
	   	//�ͷ��ڴ�
		GeoIP_range_by_ip_delete(ret);
		GeoIPRecord_delete(gir);
		
	}
	else
	{
		XCP_LOGGER_INFO(&g_logger, "GeoIP_record_by_name failed, ip:%s\n", ip.c_str());
	}

	return geography;

}



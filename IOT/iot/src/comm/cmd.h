
/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: 89617663@qq.com
 */
 
#ifndef _CMD_H
#define _CMD_H


//����
const std::string CMD_HB = "hb";                                                                        //����
const std::string CMD_REGISTER = "register";                                                            //ע�����
const std::string CMD_UNREGISTER = "unregister";                                                        //ע������


//���ؾ���
const std::string CMD_LB = "lb";                                                                        //���ؾ���


//���÷���
const std::string CMD_GET_SERVER_ACCESS = "get_server_access";                                          //��ȡ������������Ϣ
const std::string CMD_GET_USER_RIGHT = "get_user_right";                                                //��ȡ�û�Ȩ���б�
const std::string CMD_GET_SYS_CONFIG = "get_sys_config";                                                //��ȡϵͳ����


//ID ������
const std::string CMD_GET_UUID = "get_uuid";                                                            //��ȡUUID


//Key ������
const std::string CMD_CREATE_SECURITY_CHANNEL = "create_security_channel";                              //������ȫͨ��


//�û�����
const std::string CMD_REGISTER_USER = "um_register_user";                                               //ע���û�
const std::string CMD_LOGIN_PWD = "um_login_pwd";                                                       //��¼(����)
const std::string CMD_LOGIN_CODE = "um_login_code";                                                     //��¼(�ֻ���֤��)
const std::string CMD_AUTH = "um_auth";                                                                 //��֤        
const std::string CMD_GET_PHONE_CODE = "um_get_phone_code";                                             //��ȡ�ֻ���֤��
const std::string CMD_CHECK_PHONE_CODE = "um_check_phone_code";                                         //����ֻ���֤��
const std::string CMD_SET_PWD = "um_set_pwd";                                                           //��������
const std::string CMD_RESET_PWD = "um_reset_pwd";                                                       //ͨ���ֻ���֤��������������
const std::string CMD_GET_USER_PROFILE = "um_get_user_profile";                                         //��ȡ�û���Ϣ
const std::string CMD_UPDATE_USER_PROFILE = "um_update_user_profile";                                   //�����û���Ϣ
const std::string CMD_GET_USER_ACCOUNT = "um_get_user_account";                                         //��ȡ�û��˻���Ϣ
const std::string CMD_LOGOUT = "um_logout";                                                             //�˳��ͻ���


//��ͥ����
const std::string CMD_CREATE_FAMILY = "fm_create_family";                                               //������ͥ
const std::string CMD_UPDATE_FAMILY = "fm_update_family";                                               //���¼�ͥ��Ϣ
const std::string CMD_SWITCH_FAMILY = "fm_switch_family";                                               //�л���ͥ
const std::string CMD_GET_FAMILY_INFO = "fm_get_family_info";                                           //��ȡ��ͥ��Ϣ
const std::string CMD_GET_FAMILY_LIST = "fm_get_family_list";                                           //��ȡ��ͥ�б�
const std::string CMD_APPLY_FAMILY = "fm_apply_family";                                                 //��������ͥ(ɾ��)
const std::string CMD_ACCEPT_FAMILY = "fm_accept_family";                                               //ͬ������ͥ(ɾ��)
const std::string CMD_GET_APPLY_LIST = "fm_get_apply_list";                                             //��ȡ��������ͥ�б�(ɾ��)
const std::string CMD_GET_APPLY_CNT = "fm_get_apply_cnt";                                               //��ȡ��������ͥ��(ɾ��)
const std::string CMD_GET_APPLY_CODE = "fm_get_apply_code";                                 			//��ȡ�û�������
const std::string CMD_GET_INVITATION = "fm_get_invitation";                                 			//��ȡ�û����뺯
const std::string CMD_CREATE_MEMBER = "fm_create_member";											    //������ͥ��Ա
const std::string CMD_REMOVE_MEMBER = "fm_remove_member";                                               //�Ƴ���ͥ��Ա
const std::string CMD_UPDATE_MEMBER = "fm_update_member";                                               //���¼�ͥ��Ա��Ϣ
const std::string CMD_GET_MEMBER_INFO = "fm_get_member_info";                                           //��ȡ��ͥ��Ա��Ϣ
const std::string CMD_GET_MEMBER_LIST = "fm_get_member_list";                                           //��ȡ��ͥ��Ա�б�
const std::string CMD_GET_MEMBER_ID_LIST = "fm_get_member_id_list";                                     //��ȡ��ͥ��ԱID �б�
const std::string CMD_CHECK_TALK_CONDITION = "fm_check_talk_condition";                                 //�ж���������
const std::string CMD_FM_BIND_ROUTERP = "fm_bind_router";                                               //��·����(����APP)
const std::string CMD_FM_UNBIND_ROUTER = "fm_unbind_router";                                            //���·����(����APP)


//�������
const std::string CMD_ADD_ROOM = "dm_add_room";                                                         //��ӷ���
const std::string CMD_DEL_ROOM = "dm_del_room";                                                         //ɾ������
const std::string CMD_UPDATE_ROOM = "dm_update_room";                                                   //���·�����Ϣ
const std::string CMD_GET_ROOM_LIST = "dm_get_room_list";                                               //��ȡ�����б�
const std::string CMD_UPDATE_ROOM_ORDER = "dm_update_room_order";                                       //ˢ�·���˳��


//·��������
const std::string CMD_BIND_ROUTER = "dm_bind_router";                                                   //��·����
const std::string CMD_UNBIND_ROUTER = "dm_unbind_router";                                               //���·����
const std::string CMD_AUTH_ROUTER = "dm_auth_router";                                                   //·���� У���¼ 
const std::string CMD_GET_ROUTER_INFO = "dm_get_router_info";                                           //��ȡ·������Ϣ
const std::string CMD_CHECK_ROUTER = "dm_check_router";                                                 //��֤ ·���� ��Ч��


//�豸����
const std::string CMD_ADD_DEIVCE = "dm_add_device";                                                     //����豸
const std::string CMD_DEL_DEVICE = "dm_del_device";                                                     //ɾ���豸
const std::string CMD_UPDATE_DEVICE = "dm_update_device";                                               //ˢ���豸
const std::string CMD_GET_DEVICE_INFO = "dm_get_device_info";                                           //��ȡ�豸��Ϣ
const std::string CMD_GET_DEVICES_BY_ROOM = "dm_get_devices_by_room";                                   //���շ����ȡ�豸�б�
const std::string CMD_GET_DEVICES_BY_FAMILY = "dm_get_devices_by_family";                               //���ռ�ͥ��ȡ�豸�б�
const std::string CMD_REPORT_DEV_STATUS = "dm_report_dev_status";                                       //�ϱ��豸״̬
const std::string CMD_REPORT_DEV_ALERT = "dm_report_dev_alert";                                         //�ϱ��豸�澯
const std::string CMD_GET_DEV_STATUS_LIST = "dm_get_dev_status_list";                                   //��ȡ�豸״̬�б�
const std::string CMD_GET_DEV_ALERT_LIST = "dm_get_dev_alert_list";                                     //�ϱ��豸�澯�б�
const std::string CMD_GET_DEV_KEY_PROPERTY = "dm_get_dev_key_property";                                 //��ȡ�豸�ؼ�����


//MDP
const std::string CMD_MDP_MSG = "mdp_msg";                                                              //·����Ϣ
const std::string CMD_MDP_REGISTER = "mdp_register";                                                    //ע�ᵽmdp


//ͬ��������
const std::string CMD_SYN_REQ = "syn_req";                                                              //����ͬ������
const std::string CMD_SYN_RSP = "syn_rsp";                                                              //����ͬ����Ӧ
const std::string CMD_SYN_REGISTER = "syn_register";                                                    //ע��·����ID
const std::string CMD_SYN_UNREGISTER = "syn_unregister";                                                //ȥע��·����ID

const std::string CMD_SYN_ADD_MEMBER = "syn_add_member"; 
const std::string CMD_SYN_UPDATE_MEMBER = "syn_update_member"; 
const std::string CMD_SYN_DEL_MEMBER = "syn_del_member"; 
const std::string CMD_SYN_UPDATE_FAMILY = "syn_update_family"; 
const std::string CMD_SYN_USER_STATUS = "syn_user_status"; 
const std::string CMD_SYN_APPLY_FAMILY = "syn_apply_family"; 
const std::string CMD_SYN_UPDARE_USER = "syn_update_user";	


//SMS
const std::string CMD_SMS = "sms";                                                                      //����


//���ͷ���
const std::string CMD_PUSH_MSG = "push_msg";                                                            //������Ϣ


//������Ϣ
const std::string CMD_SYS_KICKOFF_CLIENT = "sys_kickoff_client"; 



#endif


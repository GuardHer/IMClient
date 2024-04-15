#pragma once

enum MSG_TYPE {
    MSG_TYPE_UNKNOWN,

    // ͨ����Ϣ
    MSG_TYPE_HEARTBEAT = 1000,
    MSG_TYPE_REGISTER,            // ע��
    MSG_TYPE_LOGIN,              // ��¼
    MSG_TYPE_GETOFRIENDLIST,     // ��ȡ�����б�
    MSG_TYPE_FINDUSER,           // �����û�
    MSG_TYPE_OPERATEFRIEND,      // ���Ѳ���
    MSG_TYPE_USERSTATUSCHANGE,   // �û�״̬�ı�
    MSG_TYPE_UPDATEUSERINFO,    // �����û���Ϣ
    MSG_TYPE_MODIFYPASSWORD,    // �޸�����
    MSG_TYPE_CEATEGROUP,        // ����Ⱥ��
    MSG_TYPE_GETGROUPMEMBERS,    // ��ȡȺ���Ա

    // ������Ϣ
    MSG_TYPE_CHAT = 1100,
    MSG_TYPE_MULTICHAT,
};

enum NET_TYPE {
    NET_DATA_UNKNOWN,

    // ͨ����Ϣ
	NET_DATA_HEARTBEAT = 1000,
	NET_DATA_REGISTER,            // ע��
	NET_DATA_LOGIN,              // ��¼
	NET_DATA_GETOFRIENDLIST,     // ��ȡ�����б�
	NET_DATA_FINDUSER,           // �����û�
	NET_DATA_OPERATEFRIEND,      // ���Ѳ���
	NET_DATA_USERSTATUSCHANGE,   // �û�״̬�ı�
	NET_DATA_UPDATEUSERINFO,    // �����û���Ϣ
	NET_DATA_MODIFYPASSWORD,    // �޸�����
	NET_DATA_CEATEGROUP,        // ����Ⱥ��
	NET_DATA_GETGROUPMEMBERS,    // ��ȡȺ���Ա
    NET_DATA_CHATMSG,            // ������Ϣ
    NET_DATA_MULTICHATMSG,       // Ⱥ����Ϣ
    NET_DATA_CHATCONFIRM_IMAGEMSG, // ͼƬ��Ϣȷ��
};

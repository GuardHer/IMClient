#pragma once

enum MSG_TYPE {
    MSG_TYPE_UNKNOWN,

    // 通用消息
    MSG_TYPE_HEARTBEAT = 1000,
    MSG_TYPE_REGISTER,            // 注册
    MSG_TYPE_LOGIN,              // 登录
    MSG_TYPE_GETOFRIENDLIST,     // 获取好友列表
    MSG_TYPE_FINDUSER,           // 查找用户
    MSG_TYPE_OPERATEFRIEND,      // 好友操作
    MSG_TYPE_USERSTATUSCHANGE,   // 用户状态改变
    MSG_TYPE_UPDATEUSERINFO,    // 更新用户信息
    MSG_TYPE_MODIFYPASSWORD,    // 修改密码
    MSG_TYPE_CEATEGROUP,        // 创建群组
    MSG_TYPE_GETGROUPMEMBERS,    // 获取群组成员

    // 聊天消息
    MSG_TYPE_CHAT = 1100,
    MSG_TYPE_MULTICHAT,
};

enum NET_TYPE {
    NET_DATA_UNKNOWN,

    // 通用消息
	NET_DATA_HEARTBEAT = 1000,
	NET_DATA_REGISTER,            // 注册
	NET_DATA_LOGIN,              // 登录
	NET_DATA_GETOFRIENDLIST,     // 获取好友列表
	NET_DATA_FINDUSER,           // 查找用户
	NET_DATA_OPERATEFRIEND,      // 好友操作
	NET_DATA_USERSTATUSCHANGE,   // 用户状态改变
	NET_DATA_UPDATEUSERINFO,    // 更新用户信息
	NET_DATA_MODIFYPASSWORD,    // 修改密码
	NET_DATA_CEATEGROUP,        // 创建群组
	NET_DATA_GETGROUPMEMBERS,    // 获取群组成员
    NET_DATA_CHATMSG,            // 聊天消息
    NET_DATA_MULTICHATMSG,       // 群聊消息
    NET_DATA_CHATCONFIRM_IMAGEMSG, // 图片消息确认
};

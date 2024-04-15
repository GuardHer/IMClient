#pragma once

#include <QTcpSocket>
#include <QThread>
#include <QByteArray>
#include <vector>
#include <queue>

#include "IMClient.h"
#include "Protocol.h"
#include "base/Logging.h"


class SendMsgThread : public QObject
{
	Q_OBJECT
public:
	SendMsgThread();
	 ~SendMsgThread();

public:
	void AddMsg(Request::ptr pMsg);
	void DeleteAllMsg();

	void Start();
	void Stop();

protected:
	void run();

private:
	void HandleMsg(Request::ptr pMsg);   // 处理消息
	void HandleRegisterMsg(Request::ptr pLegister);  // 处理注册消息
	void HandleLoginMsg(Request::ptr pLoginRequest); // 处理登录消息
	void HandleUserBasicInfoMsg(Request::ptr pUserBasicInfoRequset);  //	处理用户基本信息消息
	void HandleGroupBasicInfoMsg(Request::ptr pGroupBasicInfoRequset); // 处理群组基本信息消息

	void HandleChatMsg(Request::ptr pSendChatMsg); // 处理聊天消息 

	void HandleAddFriendMsg(Request::ptr pAddFriendMsg);  // 处理添加好友消息
	void HandleAddGroupMsg(Request::ptr pAddFriendMsg);   // 处理添加群组消息
	void HandleFindUser(Request::ptr pFindUserMsg);  // 处理查找用户消息

	void HandleCreateGroupMsg(Request::ptr pCreateNewGroupRequest);  // 处理创建群组消息
	void HandleFindFriendMsg(Request::ptr pFindFriendRequest);  // 处理查找好友消息
	void HandleOperateFriendMsg(Request::ptr pOperateFriendRequest);  // 处理操作好友消息
	bool HandleHeartBeatMsg(Request::ptr pHeartBeatMsgRequest);  // 处理心跳消息
	void HandleUpdateUserInfoMsg(Request::ptr pUpdateUserInfoRequest);  // 处理更新用户信息消息
	void HandleUpdateFriendInfoMsg(Request::ptr Msg);  // 处理更新好友信息消息
	void HandleUpdateGroupInfoMsg(Request::ptr Msg);   // 处理更新群组信息消息
	void HandleUpdateGroupMemberInfoMsg(Request::ptr Msg);  // 处理更新群组成员信息消息
	void HandleUpdateGroupMemberListMsg(Request::ptr Msg);  // 处理更新群组成员列表消息
	void HandleModifyPasswordMsg(Request::ptr ModifyPasswordRequest);  // 处理修改密码消息

public:
	
signals:
	void SendMsgToServer(Request::ptr cmd);

private:

	//thread
	std::thread thread_;  // 线程
	std::queue<Request::ptr> tasks_;  // 消息队列
	std::mutex queue_mutex_;  // 消息队列锁
	std::condition_variable condition_variable_;  // 条件变量
	std::atomic<bool> stop_{false};  // 停止标志, atomic: 原子操作

	int seq_;  // 序列号
};


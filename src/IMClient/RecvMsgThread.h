#pragma once

#include <string>
#include <QThread>
#include <QTcpSocket>
#include <queue>
#include <mutex>
#include "IMClient.h"
#include "Protocol.h"
#include "base/Logging.h"


struct CacheNotifyMsg
{
public:
	CacheNotifyMsg(std::string& data) { this->strNotifyMsg = data; };
	std::string strNotifyMsg;
};


class RecvMsgThread : public QObject
{	
	Q_OBJECT
public:	
	RecvMsgThread();
	~RecvMsgThread(void);

public:
	void AddMsgData(Request::ptr cmd);
	void DeleteAllMsgData();

	void Start();
	void Stop();

protected:
	void run();

private:
	bool checkData(const QByteArray &data);

private:
	void HandleMsg(Request::ptr cmd);   // 处理消息
	void HandleRegisterMsg(Request::ptr pLegister);  // 处理注册消息
	void HandleLoginMsg(Request::ptr pLoginRequest); // 处理登录消息
	void HandleUserBasicInfoMsg(Request::ptr pUserBasicInfoRequset);  //	处理用户基本信息消息
	void HandleGroupBasicInfoMsg(Request::ptr pGroupBasicInfoRequset); // 处理群组基本信息消息

	void HandleChatMsg(Request::ptr pSendChatMsg); // 处理聊天消息 

	void HandleAddFriendMsg();  // 处理添加好友消息
	void HandleAddGroupMsg();   // 处理添加群组消息

	void HandleCreateGroupMsg(Request::ptr pCreateNewGroupRequest);  // 处理创建群组消息
	void HandleFindFriendMsg(Request::ptr pFindFriendRequest);  // 处理查找好友消息
	void HandleOperateFriendMsg(Request::ptr pOperateFriendRequest);  // 处理操作好友消息
	bool HandleHeartBeatMsg(Request::ptr pHeartBeatMsgRequest);  // 处理心跳消息
	void HandleUpdateUserInfoMsg(Request::ptr pUpdateUserInfoRequest);  // 处理更新用户信息消息
	void HandleUpdateFriendInfoMsg();  // 处理更新好友信息消息
	void HandleUpdateGroupInfoMsg();   // 处理更新群组信息消息
	void HandleUpdateGroupMemberInfoMsg();  // 处理更新群组成员信息消息
	void HandleUpdateGroupMemberListMsg();  // 处理更新群组成员列表消息
	void HandleModifyPasswordMsg(Request::ptr ModifyPasswordRequest);  // 处理修改密码消息

	void HandleSendMsgToServer(Request::ptr data);  // 处理发送消息到服务器

public slots:
	void OnSendMsgToServer(Request::ptr cmd);
	void OnRead(Request::ptr cmd);

signals:
	void loginSuccess();  // 登录成功
	void registerSuccess();  // 注册成功
	void recvedMessage(const QString& from, const QString& content, MessageType::Type type);  // 接收到消息

private:
	std::thread thread_;
	std::queue<Request::ptr> tasks_;
	std::mutex queue_mutex_;
	std::condition_variable condition_variable_;
	std::atomic<bool> stop_{false};

	//std::unique_ptr<IMClient> pClient_;
	IMClient* pClient_;

};


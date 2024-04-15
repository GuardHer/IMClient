#include "SendMsgThread.h"


SendMsgThread::SendMsgThread()
	: stop_(false)
	, seq_(0)
{
}

SendMsgThread::~SendMsgThread()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex_);
		stop_ = true;
	}
	condition_variable_.notify_all();
	
	if (thread_.joinable()) {
		thread_.join();
	}
	LOG_DEBUG << "SendMsgThread Stop";
}

void SendMsgThread::run()
{
	LOG_DEBUG << "SendMsgThread Start";
	while (true) {
		Request::ptr pNetData;
		{
			std::unique_lock<std::mutex> guard(queue_mutex_);
			condition_variable_.wait(guard, [this]() { return stop_ || !tasks_.empty(); });
			if (stop_ && tasks_.empty()) {
				return;
			}
			pNetData = tasks_.front();
			tasks_.pop();
		}
		HandleMsg(pNetData);
	}
}



void SendMsgThread::AddMsg(Request::ptr pMsg)
{
	LOG_DEBUG << "SendMsgThread::AddMsg: " << pMsg->getCmdEnum();
	std::lock_guard<std::mutex> guard(queue_mutex_);
	tasks_.push(pMsg);
	condition_variable_.notify_one(); 
}

void SendMsgThread::DeleteAllMsg()
{
	std::lock_guard<std::mutex> guard(queue_mutex_);
	while (!tasks_.empty()) {
		Request::ptr task = tasks_.front();
		tasks_.pop();

		// 删除任务对象
		task = nullptr;
		task.reset();
		
	}
}

void SendMsgThread::Start()
{
	thread_ = std::thread(&SendMsgThread::run, this);
}

void SendMsgThread::Stop()
{
	stop_ = true;
}

void SendMsgThread::HandleMsg(Request::ptr pMsg)
{
	if (pMsg == nullptr) return;
	LOG_DEBUG << "SendMsgThread::HandleMsg: ";

	switch (pMsg->getCmdEnum())
	{
	case Command::MSG_TYPE_SENDMSGTOSER_REGISTER:  // 注册
		HandleRegisterMsg(pMsg);
		break;
	case Command::MSG_TYPE_SENDMSGTOSER_LOGIN:  // 登录
		HandleLoginMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_GETOFRIENDLIST:   // 获取好友列表
		HandleUserBasicInfoMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_GETGROUPS:  // 获取群列表
		HandleGroupBasicInfoMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_FINDUSER:  // 查找用户
		HandleFindUser(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_ADDFRIEND:  // 添加好友
		HandleAddFriendMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_CHAT:  // 发送聊天消息
		HandleChatMsg(pMsg);

		break;

	case Command::MSG_TYPE_SENDMSGTOSER_HEARTBEAT:  // 心跳
		HandleHeartBeatMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_UPDATEUSERINFO:  // 更新用户信息
		HandleUpdateUserInfoMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_MODIFYPASSWORD:  // 修改密码
		HandleModifyPasswordMsg(pMsg);

		break;
	case Command::MSG_TYPE_SENDMSGTOSER_CEATEGROUP:  // 创建群
		HandleCreateGroupMsg(pMsg);

		break;
	default:
		break;
	}
}

void SendMsgThread::HandleRegisterMsg(Request::ptr pRegisterRequset)
{
	// 发送数据
	emit SendMsgToServer(pRegisterRequset);
}

void SendMsgThread::HandleLoginMsg(Request::ptr pLoginRequest)
{
	LOG_DEBUG << "SendMsgThread SendLoginMessage!";
	// 发送数据
	emit SendMsgToServer(pLoginRequest);
}

void SendMsgThread::HandleUserBasicInfoMsg(Request::ptr pUserBasicInfoRequset)
{
	// 发送数据
	emit SendMsgToServer(pUserBasicInfoRequset);
}

void SendMsgThread::HandleGroupBasicInfoMsg(Request::ptr pGroupBasicInfoRequset)
{
	// 发送数据
	emit SendMsgToServer(pGroupBasicInfoRequset);
}

void SendMsgThread::HandleChatMsg(Request::ptr pSendChatMsg)
{
	// 发送数据
	emit SendMsgToServer(pSendChatMsg);
}

void SendMsgThread::HandleAddFriendMsg(Request::ptr pAddFriendMsg)
{
	// 发送数据
	emit SendMsgToServer(pAddFriendMsg);
}

void SendMsgThread::HandleAddGroupMsg(Request::ptr pAddGroupMsg)
{
	// 发送数据
	emit SendMsgToServer(pAddGroupMsg);
}

void SendMsgThread::HandleFindUser(Request::ptr pFindUserMsg)
{
	// 发送数据
	emit SendMsgToServer(pFindUserMsg);
}

void SendMsgThread::HandleCreateGroupMsg(Request::ptr pCreateNewGroupRequest)
{
	// 发送数据
	emit SendMsgToServer(pCreateNewGroupRequest);
}

void SendMsgThread::HandleFindFriendMsg(Request::ptr pFindFriendRequest)
{
	// 发送数据
	emit SendMsgToServer(pFindFriendRequest);
}

void SendMsgThread::HandleOperateFriendMsg(Request::ptr pOperateFriendRequest)
{
	// 发送数据
	emit SendMsgToServer(pOperateFriendRequest);
}

bool SendMsgThread::HandleHeartBeatMsg(Request::ptr pHeartBeatMsgRequest)
{
	return false;
}

void SendMsgThread::HandleUpdateUserInfoMsg(Request::ptr pUpdateUserInfoRequest)
{
	// 发送数据
	emit SendMsgToServer(pUpdateUserInfoRequest);
}

void SendMsgThread::HandleUpdateFriendInfoMsg(Request::ptr pUpdateFriendInfoMsg)
{
	// 发送数据
	emit SendMsgToServer(pUpdateFriendInfoMsg);
}

void SendMsgThread::HandleUpdateGroupInfoMsg(Request::ptr pUpdateGroupInfoMsg)
{
	// 发送数据
	emit SendMsgToServer(pUpdateGroupInfoMsg);
}

void SendMsgThread::HandleUpdateGroupMemberInfoMsg(Request::ptr pUpdateGroupMemberInfoMsg)
{
	// 发送数据
	emit SendMsgToServer(pUpdateGroupMemberInfoMsg);
}

void SendMsgThread::HandleUpdateGroupMemberListMsg(Request::ptr pUpdateGroupMemberListMsg)
{
	// 发送数据
	emit SendMsgToServer(pUpdateGroupMemberListMsg);
}

void SendMsgThread::HandleModifyPasswordMsg(Request::ptr ModifyPasswordRequest)
{
	// 发送数据
	emit SendMsgToServer(ModifyPasswordRequest);
}



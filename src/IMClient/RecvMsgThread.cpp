#include "RecvMsgThread.h"

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qjsonvalue.h>

#include "base/Singleton.h"

RecvMsgThread::RecvMsgThread()
	: stop_(false), pClient_(nullptr)
{
}

RecvMsgThread::~RecvMsgThread(void)
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex_);
		stop_ = true;
	}
	condition_variable_.notify_all();

	if (thread_.joinable()) {
		thread_.join();
	}

	// 断开连接
	if (pClient_ != nullptr)
	{
		pClient_->disconnectFromServer();
		delete pClient_;
		pClient_ = nullptr;
	}

	LOG_DEBUG << "RecvMsgThread Stop";
}


void RecvMsgThread::run()
{
	{
		LOG_DEBUG << "RecvMsgThread Start";
		pClient_ = &Singleton<IMClient>::instance();
		pClient_->initSocket();
		pClient_->connectToServer("172.24.83.224", 9999);
		pClient_->initConnection();
		QObject::connect(pClient_, &IMClient::read, this, &RecvMsgThread::OnRead);
	}
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

bool RecvMsgThread::checkData(const QByteArray& data)
{
	if (data.size() < MINBYTES)
	{
		LOG_ERROR << "IMClient Socket Data Size Less Than 24!";
		return false;
	}

	// 检查数据头 读取4字节的数据头 uint32_t
	uint32_t magic = 0;
	memcpy(&magic, data.constData(), sizeof(uint32_t));
	if (magic != MAGIC)
	{
		LOG_ERROR << "IMClient Socket Data Magic Error!";
		return false;
	}

	// 检查数据长度 读取4字节的数据长度 uint32_t, 从第13个字节开始
	uint32_t dataLen = 0;
	memcpy(&dataLen, data.constData() + 12, sizeof(uint32_t));
	if (data.size() != dataLen + MINBYTES)
	{
		LOG_ERROR << "IMClient Socket Data Length Error!";
		return false;
	}

	// 检查数据校验码 读取8字节的数据校验码 uint64_t, 末尾8字节，效验码计算的时候没有计算效验码的8个字节
	uint64_t crc = 0;
	memcpy(&crc, data.constData() + data.size() - 8, sizeof(uint64_t));
	QByteArray dataByteArray = data.left(data.size() - 8);
	uint64_t crcValue = crc64(0, dataByteArray);
	if (crc != crcValue)
	{
		LOG_ERROR << "IMClient Socket Data CRC Error!";
		return false;
	}

	return true;
}

void RecvMsgThread::AddMsgData(Request::ptr cmd)
{
	// 添加消息到队列
	std::lock_guard<std::mutex> guard(queue_mutex_);
	tasks_.emplace(cmd);
	condition_variable_.notify_one();
}

void RecvMsgThread::DeleteAllMsgData()
{
	std::unique_lock<std::mutex> guard(queue_mutex_);
	while (!tasks_.empty()) {
		tasks_.pop();
	}
}

void RecvMsgThread::Start()
{
	thread_ = std::thread(&RecvMsgThread::run, this);
}

void RecvMsgThread::Stop()
{
	stop_ = true;
}

void RecvMsgThread::HandleMsg(Request::ptr cmd)
{
	if (cmd == nullptr) return;
	LOG_DEBUG << "RecvMsgThread::HandleMsg: " << cmd->getCmdEnum();

	switch (cmd->getCmdEnum())
	{
	case Command::CommandEnum::MSG_TYPE_REGISTER:  // 注册
		HandleRegisterMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_LOGIN:  // 登录
		HandleLoginMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_GETOFRIENDLIST:   // 获取好友列表
		HandleUserBasicInfoMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_GETGROUPS:  // 获取群列表
		HandleGroupBasicInfoMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_FINDUSER:  // 查找用户

		break;
	case Command::CommandEnum::MSG_TYPE_ADDFRIEND:  // 添加好友
		HandleAddFriendMsg();
		break;
	case Command::CommandEnum::MSG_TYPE_CHAT:  // 发送聊天消息
		HandleChatMsg(cmd);

		break;

	case Command::CommandEnum::MSG_TYPE_HEARTBEAT:  // 心跳
		HandleHeartBeatMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_UPDATEUSERINFO:  // 更新用户信息
		HandleUpdateUserInfoMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_MODIFYPASSWORD:  // 修改密码
		HandleModifyPasswordMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_CEATEGROUP:  // 创建群
		HandleCreateGroupMsg(cmd);

		break;
	case Command::CommandEnum::MSG_TYPE_SENDMSGTOSER_LOGIN:
		//|| Command::CommandEnum::MSG_TYPE_SENDMSGTOSER_REGISTER
		//|| Command::CommandEnum::MSG_TYPE_SENDMSGTOSER_CHAT:  // 发送消息
		HandleSendMsgToServer(cmd);
		break;
	default:
		break;
	}
}

void RecvMsgThread::OnSendMsgToServer(Request::ptr cmd)
{
	// 发送消息到服务器
	LOG_DEBUG << "RecvMsgThread::OnSendMsgToServer";
	AddMsgData(cmd);
}


void RecvMsgThread::HandleRegisterMsg(Request::ptr pLegister)
{
}

void RecvMsgThread::HandleLoginMsg(Request::ptr pLoginRequest)
{
	// {"userid": "123456", "code": 200, "msg": "success"}
	std::string response = pLoginRequest->getData();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	QString userId = jsonObject.value("userid").toString();
	int code = jsonObject.value("code").toInt();
	QString msg = jsonObject.value("msg").toString();

	if (code == 200)
	{
		LOG_DEBUG << "Login Success!";
		emit loginSuccess();
	}
	else
	{
		LOG_DEBUG << "Login Failed!";
	}
}

void RecvMsgThread::HandleUserBasicInfoMsg(Request::ptr pUserBasicInfoRequset)
{
	// {"userid": "123456", "code": 200, "msg": "success", "friends": ["123456", "465116156", "3243423", "314124"]}
	std::string response = pUserBasicInfoRequset->getData();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	QString userId = jsonObject.value("userid").toString();
	int code = jsonObject.value("code").toInt();
	QString msg = jsonObject.value("msg").toString();
	QJsonArray friends = jsonObject.value("friends").toArray();

	if (code == 200)
	{
		LOG_DEBUG << "Get User Basic Info Success!";
		// TODO: 更新好友列表
	}
	else
	{
		LOG_DEBUG << "Get User Basic Info Failed!";
	}
}

void RecvMsgThread::HandleGroupBasicInfoMsg(Request::ptr pGroupBasicInfoRequset)
{
	// {"userid": "123456", "code": 200, "msg": "success", "groups": ["123456", "465116156", "3243423", "314124"]}
	std::string response = pGroupBasicInfoRequset->getData();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	QString userId = jsonObject.value("userid").toString();
	int code = jsonObject.value("code").toInt();
	QString msg = jsonObject.value("msg").toString();
	QJsonArray groups = jsonObject.value("groups").toArray();

	if (code == 200)
	{
		LOG_DEBUG << "Get Group Basic Info Success!";
		// TODO: 更新群组列表
	}
	else
	{
		LOG_DEBUG << "Get Group Basic Info Failed!";
	}
}

void RecvMsgThread::HandleChatMsg(Request::ptr pSendChatMsg)
{
	// {"from": "123456", "to": "123456", "content": "hello", "type": 1, "code": 200}
	std::string response = pSendChatMsg->getData();
	QJsonDocument jsonDocument = QJsonDocument::fromJson(QString::fromStdString(response).toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	int code = jsonObject.value("code").toInt();
	QString from = jsonObject.value("from").toString();
	QString to = jsonObject.value("to").toString();
	QString content = jsonObject.value("content").toString();
	int type = jsonObject.value("type").toInt();
	MessageType::Type enumValue = static_cast<MessageType::Type>(type);

	if (code == 200)
	{
		LOG_DEBUG << "Recved message from: " << from;
		emit recvedMessage(from, content, enumValue);
	}
}

void RecvMsgThread::HandleAddFriendMsg()
{
	LOG_DEBUG << "RecvMsgThread::HandleAddFriendMsg";
}

void RecvMsgThread::HandleAddGroupMsg()
{
}

void RecvMsgThread::HandleCreateGroupMsg(Request::ptr pCreateNewGroupRequest)
{
}

void RecvMsgThread::HandleFindFriendMsg(Request::ptr pFindFriendRequest)
{
}

void RecvMsgThread::HandleOperateFriendMsg(Request::ptr pOperateFriendRequest)
{
}

bool RecvMsgThread::HandleHeartBeatMsg(Request::ptr pHeartBeatMsgRequest)
{
	return false;
}

void RecvMsgThread::HandleUpdateUserInfoMsg(Request::ptr pUpdateUserInfoRequest)
{
}

void RecvMsgThread::HandleUpdateFriendInfoMsg()
{
}

void RecvMsgThread::HandleUpdateGroupInfoMsg()
{
}

void RecvMsgThread::HandleUpdateGroupMemberInfoMsg()
{
}

void RecvMsgThread::HandleUpdateGroupMemberListMsg()
{
}

void RecvMsgThread::HandleModifyPasswordMsg(Request::ptr ModifyPasswordRequest)
{
}

void RecvMsgThread::HandleSendMsgToServer(Request::ptr data)
{
	// 发送消息到服务器
	LOG_DEBUG << "magic: " << data->getMagic() << " crc: " << data->getCrc();

	pClient_->send(data->getDataByteArray());
}

void RecvMsgThread::OnRead(Request::ptr cmd)
{
	//if (!checkData(cmd->getDataByteArray())) return;
	AddMsgData(cmd);
}

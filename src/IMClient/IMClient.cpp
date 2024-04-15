#include "IMClient.h"
#include "base/Logging.h"

#include <functional>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std::placeholders;

bool IMClient::Init(const std::string& ip, short port, hv::EventLoopPtr loop)
{
	client_ = std::make_shared<hv::TcpClient>(loop);
	connfd_ = client_->createsocket(9999, ip.c_str());
	client_->onConnection = std::bind(&IMClient::onConnect, this, _1);
	client_->onMessage = std::bind(&IMClient::onMessage, this, _1, _2);
	client_->onWriteComplete = std::bind(&IMClient::onWriteComplete, this, _1, _2);

	reconn_setting_t reconn;
	reconn_setting_init(&reconn);     // 初始化reconn
	reconn.min_delay = 1000;          // 最小重连间隔
	reconn.max_delay = 10000;         // 最大重连间隔
	reconn.delay_policy = 2;          // 延迟策略
	client_->setReconnect(&reconn);   // 设置重连参数

	client_->start();

	LOG_DEBUG << "IMClient::Init : loop_tid:" << loop->tid() << " connfd:" << connfd_;

	return true;
}

void IMClient::sendMessage(ByteArray* buffer)
{
	if (client_->isConnected()) {
		std::string sendbuf = buffer->retrieveAllAsString();
		client_->send(sendbuf);
	}
	LOG_DEBUG << "isWriteComplete : " << isWriteComplete_;
}

void IMClient::sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq)
{
	ByteArray byteArray;
	byteArray.appendUint32(MAGIC);
	byteArray.appendUint32(seq);
	byteArray.appendUint32(cmd);
	byteArray.appendUint32(buffer.size());
	byteArray.append(buffer);
	byteArray.appendUint64(crc64(0, buffer));

	sendMessage(&byteArray);
}

void IMClient::close()
{
	if (this->isConnect())
	client_->closesocket();
}


/// 连接回调
void IMClient::onConnect(const hv::SocketChannelPtr& channel)
{
	std::string peeraddr = channel->peeraddr();
	if (channel->isConnected()) {
		LOG_DEBUG << "connected to " << peeraddr << " connfd=" << channel->fd();
	}

	//client_->send("hello, world!");
}

/// 消息回调
void IMClient::onMessage(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
	LOG_DEBUG << "recv message!";

	char* data = (char*)buf->data();
	size_t size = buf->size();
	ByteArray byteArray;
	byteArray.append(data, size);
	this->processPacket(channel, &byteArray);
}

/// 写完成回调
void IMClient::onWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
	isWriteComplete_ = true;
}


bool IMClient::checkCRC64(const std::string& data, uint64_t crc)
{
	return crc == crc64(0, data);
}

std::pair<uint32_t, std::string> IMClient::checkPacket(ByteArray* buf)
{
	std::pair<uint32_t, std::string> result;

	/// 读取数据包
	uint32_t    magic = buf->readUint32();
	uint32_t    seq = buf->readUint32();
	uint32_t    cmd = buf->readUint32();
	uint32_t    dataLen = buf->readUint32();
	std::string messgae = buf->retrieveAsString(dataLen);
	uint64_t    crc = buf->readUint64();

	/// 验证数据包
	if (magic != MAGIC)            result.first = 0;
	if (!checkCRC64(messgae, crc)) result.first = 0;

	/// 返回数据包
	result.first = cmd;
	result.second = messgae;

	return result;
}

bool IMClient::processPacket(const hv::SocketChannelPtr& channel, ByteArray* buf)
{
	/// 检测数据包
	std::pair<uint32_t, std::string> result = checkPacket(buf);
	/// 处理数据包
	if (result.first == 0) return false;
	/// 分发数据包
	LOG_DEBUG << "processPacket cmd: " << result.first << " , data: " << result.second;
	switch (result.first)
	{
	case Command::MSG_TYPE_HEARTBEAT:
		break;
	case Command::MSG_TYPE_REGISTER:
		HandleRegisterResponse(result.second);
		break;
	case Command::MSG_TYPE_LOGIN:
		HandleLoginResponse(result.second);
		break;
	case Command::MSG_TYPE_GETOFRIENDLIST:
		break;
	case Command::MSG_TYPE_GETGROUPS:
		break;
	case Command::MSG_TYPE_FINDUSER:
		break;
	case Command::MSG_TYPE_ADDFRIEND:
		break;
	case Command::MSG_TYPE_OPERATEFRIEND:
		break;
	case Command::MSG_TYPE_USERSTATUSCHANGE:
		break;
	case Command::MSG_TYPE_UPDATEUSERINFO:
		break;
	case Command::MSG_TYPE_MODIFYPASSWORD:
		break;
	case Command::MSG_TYPE_CEATEGROUP:
		break;
	case Command::MSG_TYPE_GETGROUPMEMBERS:
		break;
	case Command::MSG_TYPE_CHAT:
		HandleRecvChatMsg(result.second);
		break;
	case Command::MSG_TYPE_CHATACK:
		HandleSendChatMsgResponse(result.second);
		break;
	case Command::MSG_TYPE_MULTICHAT:
		break;
	case Command::MSG_TYPE_MULTICHATACK:
		break;
	default:
		break;

	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////

void IMClient::HandleLoginResponse(const std::string& data)
{
	/* {"address":"MainSt","birthday":662054400,"code":200,"customface":"customface1","customfacefmt":"","facetype":1,"gender":1,"mail":"user1@example.com","msg":"ok","nickname":"User One","phonenumber":"123-456-7890","signature":"Signature 1","userid":1291035260} */

	/// 解析数据
	QJsonDocument document = QJsonDocument::fromJson(QString::fromStdString(data).toUtf8());
	QJsonObject json = document.object();
	int code = json.value("code").toInt();

	/// 发送信号
	if (code == 200) {
		emit sigLoginResponse(true, json.value("userid").toString().toStdString());
	}
	else {
		emit sigLoginResponse(false, "");
	}
}

void IMClient::HandleRegisterResponse(const std::string& data)
{
}

void IMClient::HandleSendChatMsgResponse(const std::string& data)
{
	/*  {"code": 200, "to": "132456789", "seq": "123"} */

	/// 解析数据
	QJsonDocument document = QJsonDocument::fromJson(QString::fromStdString(data).toUtf8());
	QJsonObject json = document.object();
	int code = json.value("code").toInt();
	uint32_t seq = json.value("seq").toInt();
	std::string toUserId = json.value("to").toString().toStdString();

	LOG_DEBUG << "HandleSendChatMsgResponse : " << code << " , " << toUserId << " , " << seq;

	/// 发送信号
	emit sigSendChatMsgResponse(code == 200, toUserId, seq);
}

void IMClient::HandleRecvChatMsg(const std::string& data)
{
	/* {"from": "123456", "type": "3", "content": "hello"} */

	/// 解析数据
	QJsonDocument document = QJsonDocument::fromJson(QString::fromStdString(data).toUtf8());
	QJsonObject json = document.object();
	std::string from = json.value("from").toString().toStdString();
	std::string type = json.value("type").toString().toStdString();
	QByteArray content = json.value("content").toString().toUtf8();

	LOG_DEBUG << "data: " << data;

	/// 发送信号
	emit sigRecvChatMsg(from, std::stoi(type), content);

}



void IMClient::onClose()
{
	LOG_DEBUG << "IMClient::onClose";
	close();
}

#pragma once

#include <QObject>
#include "ClientSession.h"
#include "base/noncopyable.h"
#include "base/Singleton.h"
#include "base/crc64.h"
#include "ByteArray.h"
#include "Protocol.h"
#include "MessageType.h"

#include "hloop.h"
#include "htime.h"
#include "TcpClient.h"
#include "EventLoop.h"


class IMClient final : public QObject, noncopyable
{
    Q_OBJECT
public:
	IMClient() = default;
	~IMClient() = default;

	// 初始化
	bool Init(const std::string& ip, short port, hv::EventLoopPtr loop);

public:
	// 发送消息, 外部调用
	void sendMessage(ByteArray* buffer);
	void sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq);

	bool isConnect() const { return client_->isConnected(); }
	bool isWriteComplete() const { return isWriteComplete_; }

	void close();
	
	
private:

	/// CRC64
	bool checkCRC64(const std::string& data, uint64_t crc);

	/// 检测数据包是否完整, 返回命令类型和数据包
	std::pair<uint32_t, std::string> checkPacket(ByteArray* buf);

	/// 处理数据包
	bool processPacket(const hv::SocketChannelPtr& channel, ByteArray* buf);

	/// 连接成功
	void onConnect(const hv::SocketChannelPtr& channel);

	/// 收到消息
	void onMessage(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

	/// 发送消息完成
	void onWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

private:
	void HandleLoginResponse(const std::string& data);
	void HandleRegisterResponse(const std::string& data);
	void HandleSendChatMsgResponse(const std::string& data);
	void HandleRecvChatMsg(const std::string& data);

protected:


private:
	std::shared_ptr<hv::TcpClient> client_;     // TCP客户端
	int connfd_{};							    // 连接句柄

	std::atomic<bool> isWriteComplete_ {true};  // 是否发送完成
	std::mutex mutex_;                          // 互斥锁


signals:
	/// 登录响应信号
	void sigLoginResponse(bool success, const std::string& userId);
	/// 注册响应信号
	void sigRegisterResponse(bool success, const std::string& userId);
	/// 发送消息响应信号
	void sigSendChatMsgResponse(bool success, const std::string& toUserId, uint32_t seq);
	/// 收到消息信号
	void sigRecvChatMsg(const std::string& from, int type, const QByteArray& data);

public slots:
	void onClose();
    
};


/*
* LoginRequest: {"userid": "123456","password": "123456"}
* RegisterRequest: {"userid": "123456", "password": "123456", "username": "jock", "email": "123", "phone": "132"}
* SendChatMsg: {"from": "123456", "to": "654321", "type": "3", "content": "hello"}
*/






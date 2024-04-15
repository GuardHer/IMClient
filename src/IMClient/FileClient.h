#pragma once

#include <QObject>
#include "ByteArray.h"

#include "hloop.h"
#include "htime.h"
#include "TcpClient.h"
#include "EventLoop.h"


class FileClient final : public QObject
{
	Q_OBJECT
public:
	FileClient();
	~FileClient();

	// 初始化
	bool Init(const std::string& ip, short port, hv::EventLoopPtr loop);

public:
	// 发送消息, 外部调用
	void sendMessage(ByteArray* buffer);
	void sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq);

	bool isConnect() const { return client_->isConnected(); }
	
	void Close();
	void Connect();
	void Clear();

	// json
	std::string getStringValueFromJson(const QJsonObject& obj, const std::string& key);


private:
	/// 发送数据, 内部调用, run in loop
	void Send(const std::shared_ptr<hv::TcpClient> client, const std::string& data);

	/// CRC64
	bool checkCRC64(const std::string& data, uint64_t crc);

	/// 检测数据包是否完整, 返回命令类型和数据包
	std::pair<uint32_t, std::string> checkPacket(const std::string& data);

	/// 处理数据包
	bool processPacket(const hv::SocketChannelPtr& channel, const std::string& data);

	/// 连接成功
	void onConnect(const hv::SocketChannelPtr& channel);

	/// 收到消息
	void onMessage(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

	/// 发送消息完成
	void onWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

private:
	void HandleLoginResponse (const std::string& data);
	void HandleLogoutResponse(const std::string& data);
	void HandleUploadResponse(const std::string& data);
	void HandleDownloadResponse(const hv::SocketChannelPtr& channel, const std::string& data);

	void HandleLoadError(const hv::SocketChannelPtr& channel, const std::string& data);
	void HandleLoadOk(const hv::SocketChannelPtr& channel, const std::string& data);

protected:


private:
	std::shared_ptr<hv::TcpClient> client_;     // TCP客户端
	int connfd_{};							    // 连接句柄
	const int kBufSize_;						// 缓冲区大小
	uint32_t recvlen_{ 0 };						// 接收文件大小
	uint32_t currlen_{ 0 };						// 当前接收文件大小
	uint32_t cmd_{ 0 };							// 命令
	uint32_t seq_{ 0 };							// 序列号
	std::vector<char> recvbuf_;					// 接收文件缓冲区
	std::string ip_{};							// ip
	short port_{};								// 端口

signals:
	void sigUploadResponse(const std::string& from, const std::string& to, uint32_t seq, bool isSuccess);
	void sigDownloadResponse(const std::string& from, const std::string& to, const std::string& fileName, uint32_t seq, bool isSuccess);

public slots:
	void onClose();
};

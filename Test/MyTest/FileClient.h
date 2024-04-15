#pragma once

#include <QObject>
#include "ByteArray.h"

#include "hloop.h"
#include "htime.h"
#include "TcpClient.h"
#include "EventLoop.h"

static const uint32_t MAGIC = 0x12345678;

enum FileResponseCode
{
	FILE_RESP_OK = 200,       // 成功
	FILE_RESP_ERROR = 205,    // 失败
	FILE_RESP_LOGIN,          // 登录
	FILE_RESP_LOGOUT,         // 登出
	FILE_RESP_UPLOAD,         // 上传文件
	FILE_RESP_DOWNLOAD,       // 下载文件
};

enum FileCmd
{
	FILE_CMD_LOGIN = 300,      // 登录
	FILE_CMD_LOGOUT,           // 登出
	FILE_CMD_UPLOAD,     // 开始上传文件
	FILE_CMD_DOWNLOAD,   // 开始下载文件

};



class FileClient final : public QObject
{
	Q_OBJECT
public:
	FileClient();
	~FileClient() = default;

	// 初始化
	bool Init(const std::string& ip, short port, hv::EventLoopPtr loop);

public:
	// 发送消息, 外部调用
	void sendMessage(ByteArray* buffer);
	void sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq);

	bool isConnect() const { return client_->isConnected(); }
	void Close();
	void Connect();
	// json
	std::string getStringValueFromJson(const QJsonObject& obj, const std::string& key);


private:

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

	/// 
	void Clear();

private:
	void HandleLoginResponse(const std::string& data);
	void HandleLogoutResponse(const std::string& data);
	void HandleUploadResponse(const std::string& data);
	void HandleDownloadResponse(const hv::SocketChannelPtr& channel, const std::string& data);

	void HandleLoadError(const hv::SocketChannelPtr& channel, const std::string& data);
	void HandleLoadOk(const hv::SocketChannelPtr& channel, const std::string& data);

protected:


private:
	std::shared_ptr<hv::TcpClient> client_;     // TCP客户端
	int connfd_{};							    // 连接句柄
	std::mutex mutex_;                          // 互斥锁

	const int kBufSize_;						// 缓冲区大小
	
	uint32_t recvlen_{0};							// 接收文件大小
	uint32_t currlen_{0};							// 当前接收文件大小
	uint32_t cmd_{0};								// 命令
	std::vector<char> recvbuf_;					// 接收文件缓冲区

	std::string ip_;
	short port_;

signals:
	void onUploadResponse(const std::string& code);
	void onDownloadResponse(const std::string& filename, const std::string& filesize, const std::string& md5, const std::string& data);

public slots:

};
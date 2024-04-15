#include "FileClient.h"

#include "hlog.h"
#include "crc64.h"
#include "Singleton.h"
#include "FileManager.h"

#include <functional>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std::placeholders;

FileClient::FileClient()
	: client_(nullptr), kBufSize_(64 * 1024), connfd_(-1), cmd_(-1), recvlen_(0), currlen_(0), ip_(""), port_(0)
{
}


bool FileClient::Init(const std::string& ip, short port, hv::EventLoopPtr loop)
{

	client_.reset(new hv::TcpClient(loop));
	connfd_                  = client_->createsocket(port, ip.c_str());
	client_->onConnection    = std::bind(&FileClient::onConnect, this, _1);
	client_->onMessage       = std::bind(&FileClient::onMessage, this, _1, _2);
	client_->onWriteComplete = std::bind(&FileClient::onWriteComplete, this, _1, _2);

	reconn_setting_t reconn;
	reconn_setting_init(&reconn);        // 初始化reconn
	reconn.min_delay    = 1000;          // 最小重连间隔
	reconn.max_delay    = 10000;         // 最大重连间隔
	reconn.delay_policy = 2;             // 延迟策略
	client_->setReconnect(&reconn);      // 设置重连参数

	this->ip_   = ip;
	this->port_ = port;
	this->Clear();

	client_->start();

	qDebug() << "FileClient::Init : loop_tid:" << loop->tid() << connfd_;

	return true;
}

void FileClient::onConnect(const hv::SocketChannelPtr& channel)
{
	std::string peeraddr = channel->peeraddr();
	if (channel->isConnected()) {
		qDebug() << "FileClient::onConnect : " << QString::fromStdString(peeraddr);
	}
	else {
		qDebug() << "FileClient::onConnect : " << QString::fromStdString(peeraddr) << " disconnected";
	}

}

void FileClient::onMessage(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
	
	qDebug() << "FileClient::onMessage len : " << buf->size();
	ByteArray buffer;
	buffer.append(buf->data(), buf->size());
	// 窥视前4个字节
	uint32_t magic = buffer.peekInt32();
	if (magic == MAGIC) {
		uint32_t magic = buffer.readUInt32();
		uint32_t cmd = buffer.readUInt32();
		uint32_t len = buffer.readUInt32();

		this->recvbuf_.clear();
		this->recvlen_ = len + 8;  //
		this->cmd_ = cmd;
	}

	uint32_t readableBytes = buffer.readableBytes();
	std::string data = buffer.retrieveAsString(readableBytes);
	// 将数据添加到recvBuf_
	currlen_ += readableBytes;
	qDebug() << "readableBytes: " << readableBytes << ", currentLen_: " << currlen_;
	recvbuf_.insert(recvbuf_.end(), data.begin(), data.end());

	if (currlen_ < recvlen_) return;     // 数据未接收完
	
	std::string dataStr(recvbuf_.begin(), recvbuf_.end());
	currlen_ = 0;
	recvlen_ = 0;
	recvbuf_.clear();
	if (!processPacket(channel, dataStr)) {
		qDebug() << "Process error, dataStr";
	}
}

void FileClient::onWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
	qDebug() << "FileClient::onWriteComplete : " << buf->size();
}

void FileClient::Clear()
{
	connfd_ = -1;
	cmd_ = -1;
	recvbuf_.clear();
	recvlen_ = 0;
	currlen_ = 0;
}


void FileClient::sendMessage(ByteArray* buffer)
{
	if (client_->isConnected()) {
		std::string sendbuf = buffer->retrieveAllAsString();

		// 分段发送，每次发送64k， 定时器发送
		size_t len = sendbuf.size();
		size_t pos = 0;
		while (len > 0) {
			size_t sendlen = len > kBufSize_ ? kBufSize_ : len;
			client_->send(sendbuf.c_str() + pos, sendlen);
			len -= sendlen;
			pos += sendlen;
			// 每次发送间隔1ms
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

	}
	else {
		qDebug() << "FileClient::sendMessage : not connected";
	}
}

void FileClient::sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq)
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

void FileClient::Close()
{
	if (client_->isConnected()) {
		client_->closesocket();
		client_->stop();
		
		connfd_ = -1;
		cmd_ = -1;
		recvbuf_.clear();
		recvlen_ = 0;
		currlen_ = 0;
		// 退出loop
		//client_->loop()->stop();
	}
	
}

void FileClient::Connect()
{ 
	if (connfd_ == -1) this->Init(ip_, port_, client_->loop());
}


bool FileClient::checkCRC64(const std::string& data, uint64_t crc)
{

	return crc == crc64(0, data);
}

std::pair<uint32_t, std::string> FileClient::checkPacket(const std::string& data)
{
	std::pair<uint32_t, std::string> result;

	/// 读取数据包
	uint32_t    cmd = this->cmd_;
	std::string crcStr = data.substr(data.size() - 8, 8);   // 读取crc, 8个字节
	uint64_t crc = 0;
	memcpy(&crc, crcStr.c_str(), 8);                        // 8字节crc
	std::string fileData = data.substr(0, data.size() - 8); // 读取数据


	/// 验证数据包
	if (!checkCRC64(fileData, crc)) result.first = 0;

	/// 返回数据包
	result.first = cmd;
	result.second = fileData;

	this->cmd_ = 0;
	return result;
}

bool FileClient::processPacket(const hv::SocketChannelPtr& channel, const std::string& data)
{
	/// 检测数据包
	std::pair<uint32_t, std::string> result = checkPacket(data);
	/// 处理数据包
	if (result.first == 0) return false;
	/// 分发数据包
	qDebug() << "processPacket cmd: " << result.first;
	switch (result.first) {
	case FileResponseCode::FILE_RESP_LOGIN:
		HandleLoginResponse(result.second);
		break;
	case FileResponseCode::FILE_RESP_LOGOUT:
		HandleLogoutResponse(result.second);
		break;
	case FileResponseCode::FILE_RESP_UPLOAD:
		HandleUploadResponse(result.second);
		break;
	case FileResponseCode::FILE_RESP_DOWNLOAD:
		HandleDownloadResponse(channel, result.second);
		break;
	case FileResponseCode::FILE_RESP_ERROR:
		HandleLoadError(channel, result.second);
		break;
	case FileResponseCode::FILE_RESP_OK:
		HandleLoadOk(channel, result.second);
		break;
	default:
		break;
	}

	return true;

}


void FileClient::HandleLoginResponse(const std::string& data)
{

	hlogd("FileClient::HandleLoginResponse : ", data);
}

void FileClient::HandleLogoutResponse(const std::string& data)
{
	hlogd("FileClient::HandleLogoutResponse : ", data);
}


void FileClient::HandleUploadResponse(const std::string& data)
{
	
}

void FileClient::HandleDownloadResponse(const hv::SocketChannelPtr& channel, const std::string& data)
{
	// {"code": "200", "msg": "start download ok", "filename": "xxx", "data": "xxx"}

	std::string code;
	std::string msg;
	std::string filename;
	std::string filedata;

	// 解析json
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(QString::fromStdString(data).toUtf8(), &jsonError);
	if (jsonError.error   != QJsonParseError::NoError || !doucment.isObject()) {
		hloge("FileClient::HandleDownloadResponse : ", "jsonError.error: ", jsonError.errorString());
		return;
	}

	QJsonObject obj = doucment.object();
	code            = getStringValueFromJson(obj, "code");
	msg             = getStringValueFromJson(obj, "msg");
	filename        = getStringValueFromJson(obj, "filename");
	filedata        = getStringValueFromJson(obj, "data");

	if (code != "200") return;

	// 保存文件
	int ret = Singleton<FileManager>::instance().SaveFile(filename, filedata, true);
	
	if (ret) {
		qDebug() << "FileClient::HandleDownloadResponse : " << QString::fromStdString(filename) << " download success";
	}
	else {
		qDebug() << "FileClient::HandleDownloadResponse : " << QString::fromStdString(filename) << " download failed";
	}

}


void FileClient::HandleLoadError(const hv::SocketChannelPtr& channel, const std::string& data)
{
	// {"code": "xxx", "msg": "xxx"}
	// 解析json
	std::string msg;
	std::string code;
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(QString::fromStdString(data).toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError || !doucment.isObject()) {
		hloge("FileClient::HandleLoadError : ", "jsonError.error: ", jsonError.errorString());
		return;
	}

	QJsonObject obj = doucment.object();
	code = getStringValueFromJson(obj, "code");
	msg = getStringValueFromJson(obj, "msg");

	// 关闭连接
	qDebug() << "load error msg: " << QString::fromStdString(msg) << ", code: " << QString::fromStdString(code);
	Close();
}

void FileClient::HandleLoadOk(const hv::SocketChannelPtr& channel, const std::string& data)
{
	// 解析json
	std::string msg;
	std::string code;
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(QString::fromStdString(data).toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError || !doucment.isObject()) {
		hloge("FileClient::HandleLoadError : ", "jsonError.error: ", jsonError.errorString());
		return;
	}

	QJsonObject obj = doucment.object();
	code = getStringValueFromJson(obj, "code");
	msg = getStringValueFromJson(obj, "msg");

	// 关闭连接
	qDebug() << "load ok msg: " << QString::fromStdString(msg) << ", code: " << QString::fromStdString(code);
	Close();
}

std::string FileClient::getStringValueFromJson(const QJsonObject& obj, const std::string& key)
{
	std::string value;
	if (obj.contains(QString::fromStdString(key))) {
		QJsonValue jsonValue = obj.value(QString::fromStdString(key));
		if (jsonValue.isString()) {
			value = jsonValue.toString().toStdString();
		}
	}
	return value;
}

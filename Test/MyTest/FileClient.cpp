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
	reconn_setting_init(&reconn);        // ��ʼ��reconn
	reconn.min_delay    = 1000;          // ��С�������
	reconn.max_delay    = 10000;         // ����������
	reconn.delay_policy = 2;             // �ӳٲ���
	client_->setReconnect(&reconn);      // ������������

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
	// ����ǰ4���ֽ�
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
	// ��������ӵ�recvBuf_
	currlen_ += readableBytes;
	qDebug() << "readableBytes: " << readableBytes << ", currentLen_: " << currlen_;
	recvbuf_.insert(recvbuf_.end(), data.begin(), data.end());

	if (currlen_ < recvlen_) return;     // ����δ������
	
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

		// �ֶη��ͣ�ÿ�η���64k�� ��ʱ������
		size_t len = sendbuf.size();
		size_t pos = 0;
		while (len > 0) {
			size_t sendlen = len > kBufSize_ ? kBufSize_ : len;
			client_->send(sendbuf.c_str() + pos, sendlen);
			len -= sendlen;
			pos += sendlen;
			// ÿ�η��ͼ��1ms
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
		// �˳�loop
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

	/// ��ȡ���ݰ�
	uint32_t    cmd = this->cmd_;
	std::string crcStr = data.substr(data.size() - 8, 8);   // ��ȡcrc, 8���ֽ�
	uint64_t crc = 0;
	memcpy(&crc, crcStr.c_str(), 8);                        // 8�ֽ�crc
	std::string fileData = data.substr(0, data.size() - 8); // ��ȡ����


	/// ��֤���ݰ�
	if (!checkCRC64(fileData, crc)) result.first = 0;

	/// �������ݰ�
	result.first = cmd;
	result.second = fileData;

	this->cmd_ = 0;
	return result;
}

bool FileClient::processPacket(const hv::SocketChannelPtr& channel, const std::string& data)
{
	/// ������ݰ�
	std::pair<uint32_t, std::string> result = checkPacket(data);
	/// �������ݰ�
	if (result.first == 0) return false;
	/// �ַ����ݰ�
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

	// ����json
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

	// �����ļ�
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
	// ����json
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

	// �ر�����
	qDebug() << "load error msg: " << QString::fromStdString(msg) << ", code: " << QString::fromStdString(code);
	Close();
}

void FileClient::HandleLoadOk(const hv::SocketChannelPtr& channel, const std::string& data)
{
	// ����json
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

	// �ر�����
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

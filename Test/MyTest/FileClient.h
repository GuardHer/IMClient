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
	FILE_RESP_OK = 200,       // �ɹ�
	FILE_RESP_ERROR = 205,    // ʧ��
	FILE_RESP_LOGIN,          // ��¼
	FILE_RESP_LOGOUT,         // �ǳ�
	FILE_RESP_UPLOAD,         // �ϴ��ļ�
	FILE_RESP_DOWNLOAD,       // �����ļ�
};

enum FileCmd
{
	FILE_CMD_LOGIN = 300,      // ��¼
	FILE_CMD_LOGOUT,           // �ǳ�
	FILE_CMD_UPLOAD,     // ��ʼ�ϴ��ļ�
	FILE_CMD_DOWNLOAD,   // ��ʼ�����ļ�

};



class FileClient final : public QObject
{
	Q_OBJECT
public:
	FileClient();
	~FileClient() = default;

	// ��ʼ��
	bool Init(const std::string& ip, short port, hv::EventLoopPtr loop);

public:
	// ������Ϣ, �ⲿ����
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

	/// ������ݰ��Ƿ�����, �����������ͺ����ݰ�
	std::pair<uint32_t, std::string> checkPacket(const std::string& data);

	/// �������ݰ�
	bool processPacket(const hv::SocketChannelPtr& channel, const std::string& data);

	/// ���ӳɹ�
	void onConnect(const hv::SocketChannelPtr& channel);

	/// �յ���Ϣ
	void onMessage(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

	/// ������Ϣ���
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
	std::shared_ptr<hv::TcpClient> client_;     // TCP�ͻ���
	int connfd_{};							    // ���Ӿ��
	std::mutex mutex_;                          // ������

	const int kBufSize_;						// ��������С
	
	uint32_t recvlen_{0};							// �����ļ���С
	uint32_t currlen_{0};							// ��ǰ�����ļ���С
	uint32_t cmd_{0};								// ����
	std::vector<char> recvbuf_;					// �����ļ�������

	std::string ip_;
	short port_;

signals:
	void onUploadResponse(const std::string& code);
	void onDownloadResponse(const std::string& filename, const std::string& filesize, const std::string& md5, const std::string& data);

public slots:

};
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

	// ��ʼ��
	bool Init(const std::string& ip, short port, hv::EventLoopPtr loop);

public:
	// ������Ϣ, �ⲿ����
	void sendMessage(ByteArray* buffer);
	void sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq);

	bool isConnect() const { return client_->isConnected(); }
	
	void Close();
	void Connect();
	void Clear();

	// json
	std::string getStringValueFromJson(const QJsonObject& obj, const std::string& key);


private:
	/// ��������, �ڲ�����, run in loop
	void Send(const std::shared_ptr<hv::TcpClient> client, const std::string& data);

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

private:
	void HandleLoginResponse (const std::string& data);
	void HandleLogoutResponse(const std::string& data);
	void HandleUploadResponse(const std::string& data);
	void HandleDownloadResponse(const hv::SocketChannelPtr& channel, const std::string& data);

	void HandleLoadError(const hv::SocketChannelPtr& channel, const std::string& data);
	void HandleLoadOk(const hv::SocketChannelPtr& channel, const std::string& data);

protected:


private:
	std::shared_ptr<hv::TcpClient> client_;     // TCP�ͻ���
	int connfd_{};							    // ���Ӿ��
	const int kBufSize_;						// ��������С
	uint32_t recvlen_{ 0 };						// �����ļ���С
	uint32_t currlen_{ 0 };						// ��ǰ�����ļ���С
	uint32_t cmd_{ 0 };							// ����
	uint32_t seq_{ 0 };							// ���к�
	std::vector<char> recvbuf_;					// �����ļ�������
	std::string ip_{};							// ip
	short port_{};								// �˿�

signals:
	void sigUploadResponse(const std::string& from, const std::string& to, uint32_t seq, bool isSuccess);
	void sigDownloadResponse(const std::string& from, const std::string& to, const std::string& fileName, uint32_t seq, bool isSuccess);

public slots:
	void onClose();
};

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

	// ��ʼ��
	bool Init(const std::string& ip, short port, hv::EventLoopPtr loop);

public:
	// ������Ϣ, �ⲿ����
	void sendMessage(ByteArray* buffer);
	void sendMessage(uint32_t cmd, const std::string& buffer, uint32_t seq);

	bool isConnect() const { return client_->isConnected(); }
	bool isWriteComplete() const { return isWriteComplete_; }

	void close();
	
	
private:

	/// CRC64
	bool checkCRC64(const std::string& data, uint64_t crc);

	/// ������ݰ��Ƿ�����, �����������ͺ����ݰ�
	std::pair<uint32_t, std::string> checkPacket(ByteArray* buf);

	/// �������ݰ�
	bool processPacket(const hv::SocketChannelPtr& channel, ByteArray* buf);

	/// ���ӳɹ�
	void onConnect(const hv::SocketChannelPtr& channel);

	/// �յ���Ϣ
	void onMessage(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

	/// ������Ϣ���
	void onWriteComplete(const hv::SocketChannelPtr& channel, hv::Buffer* buf);

private:
	void HandleLoginResponse(const std::string& data);
	void HandleRegisterResponse(const std::string& data);
	void HandleSendChatMsgResponse(const std::string& data);
	void HandleRecvChatMsg(const std::string& data);

protected:


private:
	std::shared_ptr<hv::TcpClient> client_;     // TCP�ͻ���
	int connfd_{};							    // ���Ӿ��

	std::atomic<bool> isWriteComplete_ {true};  // �Ƿ������
	std::mutex mutex_;                          // ������


signals:
	/// ��¼��Ӧ�ź�
	void sigLoginResponse(bool success, const std::string& userId);
	/// ע����Ӧ�ź�
	void sigRegisterResponse(bool success, const std::string& userId);
	/// ������Ϣ��Ӧ�ź�
	void sigSendChatMsgResponse(bool success, const std::string& toUserId, uint32_t seq);
	/// �յ���Ϣ�ź�
	void sigRecvChatMsg(const std::string& from, int type, const QByteArray& data);

public slots:
	void onClose();
    
};


/*
* LoginRequest: {"userid": "123456","password": "123456"}
* RegisterRequest: {"userid": "123456", "password": "123456", "username": "jock", "email": "123", "phone": "132"}
* SendChatMsg: {"from": "123456", "to": "654321", "type": "3", "content": "hello"}
*/






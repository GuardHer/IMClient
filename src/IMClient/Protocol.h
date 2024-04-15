#pragma once
/**
 * @file protocol.h
 * @brief �Զ���Э��
 */

#include <memory>
#include <QByteArray>

static const uint32_t MAGIC = 0x12345678;
static const uint32_t MINBYTES = 24;

class Command {
public:
	enum CommandEnum {
		MSG_TYPE_UNKNOWN = 1,

		// ͨ����Ϣ
		MSG_TYPE_HEARTBEAT = 1000,
		MSG_TYPE_REGISTER,            // ע��
		MSG_TYPE_LOGIN,               // ��¼
		MSG_TYPE_GETOFRIENDLIST,      // ��ȡ�����б�
		MSG_TYPE_GETGROUPS,           // ��ȡȺ���б�
		MSG_TYPE_FINDUSER,            // �����û�
		MSG_TYPE_ADDFRIEND,           // ��Ӻ���
		MSG_TYPE_OPERATEFRIEND,       // ���Ѳ���
		MSG_TYPE_USERSTATUSCHANGE,    // �û�״̬�ı�
		MSG_TYPE_UPDATEUSERINFO,      // �����û���Ϣ
		MSG_TYPE_MODIFYPASSWORD,      // �޸�����
		MSG_TYPE_CEATEGROUP,          // ����Ⱥ��
		MSG_TYPE_GETGROUPMEMBERS,     // ��ȡȺ���Ա

		// ������Ϣ 
		MSG_TYPE_CHAT = 1100, 	      // ����
		MSG_TYPE_CHATACK,             // ���Ļ�ִ
		MSG_TYPE_MULTICHAT,           // Ⱥ��
		MSG_TYPE_MULTICHATACK,        // Ⱥ�Ļ�ִ

		// ��������Ϣ
		MSG_TYPE_SENDMSGTOSER_LOGIN = 1200,      // ��¼
		MSG_TYPE_SENDMSGTOSER_REGISTER,          // ע��
		MSG_TYPE_SENDMSGTOSER_GETOFRIENDLIST,    // ��ȡ�����б�
		MSG_TYPE_SENDMSGTOSER_GETGROUPS,         // ��ȡȺ���б�
		MSG_TYPE_SENDMSGTOSER_FINDUSER,          // �����û�
		MSG_TYPE_SENDMSGTOSER_ADDFRIEND,         // ��Ӻ���
		MSG_TYPE_SENDMSGTOSER_OPERATEFRIEND,     // ���Ѳ���
		MSG_TYPE_SENDMSGTOSER_USERSTATUSCHANGE,  // �û�״̬�ı�
		MSG_TYPE_SENDMSGTOSER_UPDATEUSERINFO,    // �����û���Ϣ
		MSG_TYPE_SENDMSGTOSER_MODIFYPASSWORD,    // �޸�����
		MSG_TYPE_SENDMSGTOSER_CEATEGROUP,        // ����Ⱥ��
		MSG_TYPE_SENDMSGTOSER_GETGROUPMEMBERS,   // ��ȡȺ���Ա
		MSG_TYPE_SENDMSGTOSER_CHAT,              // ����
		MSG_TYPE_SENDMSGTOSER_MULTICHAT,         // Ⱥ��
		MSG_TYPE_SENDMSGTOSER_HEARTBEAT,         // ����
	};
};

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

class PacketMessage {
public:
	typedef std::shared_ptr<PacketMessage> ptr;
	enum PacketMessageType {
		REQUEST = 1,   // ����
		RESPONSE = 2,  // ��Ӧ
		NOTIFY = 3     // ֪ͨ
	};
	virtual ~PacketMessage() {}

	virtual bool serializeToByteArray( QByteArray& bytearray) = 0;
	virtual bool parseFromByteArray( QByteArray& bytearray) = 0;

	virtual std::string toString() const = 0;
	virtual const std::string& getName() const = 0;
	virtual int32_t getType() const = 0;
};

class Request {
public:
	typedef std::shared_ptr<Request> ptr;
	Request();
	Request(Command::CommandEnum cmd, const std::string& strData);
	Request(QByteArray& data);

	uint32_t getSeq() const { return m_seq; }
	uint32_t getCmd() const { return m_cmd; }
	Command::CommandEnum getCmdEnum() const { return m_cmd; }
	uint32_t getDataLen() const { return m_dataLen; }
	const std::string& getData() const { return m_msg; }
	uint32_t getMagic() const { return m_magic; }
	uint64_t getCrc() const { return m_crc; }
	QByteArray getDataByteArray() const { return m_data; }

	void setSeq(uint32_t v) { m_seq = v; }
	void setCmd(Command::CommandEnum v) { m_cmd = v; }
	void setDataLen(uint32_t v) { m_dataLen = v; }
	void setData(const std::string& v) { m_msg = v; }
	void setCrc(uint32_t v) { m_crc = v; }

	uint64_t calculateChecksum(const QByteArray& data);
	uint64_t calculateChecksum(const std::string& data);

	bool serializeToByteArray() ;
	bool serializeToByteArray(QByteArray& bytearray) ;
	bool parseFromByteArray(QByteArray& bytearray) ;

private:
	size_t m_size;   // ���� 24�ֽ�
	QByteArray m_data;

protected:
	// �������ʽ
	const uint32_t m_magic;     // ħ�� 4�ֽ�
	uint32_t m_seq;             // ���к� 4�ֽ�
	Command::CommandEnum m_cmd; // ���� 4�ֽ�
	uint32_t m_dataLen;         // ���ݳ��� 4�ֽ�    
	std::string m_msg;          // ����
	uint64_t m_crc;             // У���� 8�ֽ�

};

class Response : public PacketMessage {
public:
	typedef std::shared_ptr<Response> ptr;

	Response();

	uint32_t getSn() const { return m_seq; }
	uint32_t getCmd() const { return m_cmd; }
	uint32_t getResult() const { return m_result; }
	const std::string& getResultStr() const { return m_resultStr; }

	void setSn(uint32_t v) { m_seq = v; }
	void setCmd(uint32_t v) { m_cmd = v; }
	void setResult(uint32_t v) { m_result = v; }
	void setResultStr(const std::string& v) { m_resultStr = v; }

	virtual bool serializeToByteArray( QByteArray& bytearray) override;
	virtual bool parseFromByteArray( QByteArray& bytearray) override;
protected:
	const uint32_t m_magic;      // ħ�� 4�ֽ�
	uint32_t m_seq;			     // ���к� 4�ֽ�
	uint32_t m_cmd;              // ���� 4�ֽ�
	uint32_t m_result;           // ��� 4�ֽ�
	std::string m_resultStr;     // �������
	uint64_t m_crc;              // У���� 4�ֽ�
};

class Notify : public PacketMessage {
public:
	typedef std::shared_ptr<Notify> ptr;
	Notify();

	uint32_t getNotify() const { return m_notify; }
	void setNotify(uint32_t v) { m_notify = v; }

	virtual bool serializeToByteArray( QByteArray& bytearray) override;
	virtual bool parseFromByteArray( QByteArray& bytearray) override;
protected:
	uint32_t m_notify;
};

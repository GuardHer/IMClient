#pragma once
/**
 * @file protocol.h
 * @brief 自定义协议
 */

#include <memory>
#include <QByteArray>

static const uint32_t MAGIC = 0x12345678;
static const uint32_t MINBYTES = 24;

class Command {
public:
	enum CommandEnum {
		MSG_TYPE_UNKNOWN = 1,

		// 通用消息
		MSG_TYPE_HEARTBEAT = 1000,
		MSG_TYPE_REGISTER,            // 注册
		MSG_TYPE_LOGIN,               // 登录
		MSG_TYPE_GETOFRIENDLIST,      // 获取好友列表
		MSG_TYPE_GETGROUPS,           // 获取群组列表
		MSG_TYPE_FINDUSER,            // 查找用户
		MSG_TYPE_ADDFRIEND,           // 添加好友
		MSG_TYPE_OPERATEFRIEND,       // 好友操作
		MSG_TYPE_USERSTATUSCHANGE,    // 用户状态改变
		MSG_TYPE_UPDATEUSERINFO,      // 更新用户信息
		MSG_TYPE_MODIFYPASSWORD,      // 修改密码
		MSG_TYPE_CEATEGROUP,          // 创建群组
		MSG_TYPE_GETGROUPMEMBERS,     // 获取群组成员

		// 聊天消息 
		MSG_TYPE_CHAT = 1100, 	      // 单聊
		MSG_TYPE_CHATACK,             // 单聊回执
		MSG_TYPE_MULTICHAT,           // 群聊
		MSG_TYPE_MULTICHATACK,        // 群聊回执

		// 处理发送消息
		MSG_TYPE_SENDMSGTOSER_LOGIN = 1200,      // 登录
		MSG_TYPE_SENDMSGTOSER_REGISTER,          // 注册
		MSG_TYPE_SENDMSGTOSER_GETOFRIENDLIST,    // 获取好友列表
		MSG_TYPE_SENDMSGTOSER_GETGROUPS,         // 获取群组列表
		MSG_TYPE_SENDMSGTOSER_FINDUSER,          // 查找用户
		MSG_TYPE_SENDMSGTOSER_ADDFRIEND,         // 添加好友
		MSG_TYPE_SENDMSGTOSER_OPERATEFRIEND,     // 好友操作
		MSG_TYPE_SENDMSGTOSER_USERSTATUSCHANGE,  // 用户状态改变
		MSG_TYPE_SENDMSGTOSER_UPDATEUSERINFO,    // 更新用户信息
		MSG_TYPE_SENDMSGTOSER_MODIFYPASSWORD,    // 修改密码
		MSG_TYPE_SENDMSGTOSER_CEATEGROUP,        // 创建群组
		MSG_TYPE_SENDMSGTOSER_GETGROUPMEMBERS,   // 获取群组成员
		MSG_TYPE_SENDMSGTOSER_CHAT,              // 单聊
		MSG_TYPE_SENDMSGTOSER_MULTICHAT,         // 群聊
		MSG_TYPE_SENDMSGTOSER_HEARTBEAT,         // 心跳
	};
};

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

class PacketMessage {
public:
	typedef std::shared_ptr<PacketMessage> ptr;
	enum PacketMessageType {
		REQUEST = 1,   // 请求
		RESPONSE = 2,  // 响应
		NOTIFY = 3     // 通知
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
	size_t m_size;   // 基础 24字节
	QByteArray m_data;

protected:
	// 请求包格式
	const uint32_t m_magic;     // 魔数 4字节
	uint32_t m_seq;             // 序列号 4字节
	Command::CommandEnum m_cmd; // 命令 4字节
	uint32_t m_dataLen;         // 数据长度 4字节    
	std::string m_msg;          // 数据
	uint64_t m_crc;             // 校验码 8字节

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
	const uint32_t m_magic;      // 魔数 4字节
	uint32_t m_seq;			     // 序列号 4字节
	uint32_t m_cmd;              // 命令 4字节
	uint32_t m_result;           // 结果 4字节
	std::string m_resultStr;     // 结果描述
	uint64_t m_crc;              // 校验码 4字节
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

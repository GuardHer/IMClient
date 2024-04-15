
#include "ClientSession.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

uint64_t ClientSession::getCRC64(const std::string& data)
{
	return crc64(0, data);
}


void ClientSession::packChat(ByteArray* byteArray, const std::string& from, const std::string& to, MessageType::Type type, const QByteArray& data, uint32_t seq)
{
	// {"from": "123456", "to": "654321", "type": "3", "content": "hello"}

	byteArray->retrieveAll();                            // 清空数据

	byteArray->appendUint32(MAGIC);                      // magic
	byteArray->appendUint32(seq);                        // seq
	byteArray->appendUint32(Command::MSG_TYPE_CHAT);     // cmd

	QJsonObject json;
	json.insert("from", QString::fromStdString(from));
	json.insert("to", QString::fromStdString(to));
	json.insert("type", QString::number(type));
	json.insert("content", QString::fromStdString(data.toStdString()));

	QJsonDocument document;
	document.setObject(json);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	std::string strJson(byte_array.constData(), byte_array.size());
	byteArray->appendUint32(strJson.size());              // len
	byteArray->append(strJson);                           // data
	byteArray->appendUint64(getCRC64(strJson));           // crc
}

void ClientSession::packMultiChat(ByteArray* byteArray, const std::string& from, const std::string& to, MessageType::Type type, const std::string& data)
{
	// {"from": "123456", "to": ["654321", "654322"], "type": "3", "content": "hello"}
	byteArray->retrieveAll();                            // 清空数据

	byteArray->appendUint32(MAGIC);                       // magic
	byteArray->appendUint32(0);                           // seq
	byteArray->appendUint32(Command::MSG_TYPE_MULTICHAT);     // cmd

	QJsonObject json;
	json.insert("from", QString::fromStdString(from));
	QJsonArray toList;
	toList.append(QString::fromStdString(to));
	json.insert("to", toList);
	json.insert("type", QString::number(type));
	json.insert("content", QString::fromStdString(data));

	QJsonDocument document;
	document.setObject(json);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	std::string strJson(byte_array.constData(), byte_array.size());
	byteArray->appendUint32(strJson.size());              // len
	byteArray->append(strJson);                           // data
	byteArray->appendUint64(getCRC64(strJson));           // crc
}


void ClientSession::packHeartbeat(ByteArray* byteArray)
{
}

void ClientSession::packRegister(ByteArray* byteArray, const std::string& userId, const std::string& password)
{
	// {"userid
}
void ClientSession::packLogin(ByteArray* byteArray, const std::string& userId, const std::string& password)
{
	byteArray->retrieveAll();                            // 清空数据

	byteArray->appendUint32(MAGIC);                       // magic
	byteArray->appendUint32(0);                           // seq
	byteArray->appendUint32(Command::MSG_TYPE_LOGIN);     // cmd

	QJsonObject json;
	json.insert("userid", QString::fromStdString(userId));
	json.insert("password", QString::fromStdString(password));

	QJsonDocument document;
	document.setObject(json);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	std::string strJson(byte_array.constData(), byte_array.size());
	byteArray->appendUint32(strJson.size());              // len
	byteArray->append(strJson);                           // data
	byteArray->appendUint64(getCRC64(strJson));           // crc
}

void ClientSession::packGetFriendList(ByteArray* byteArray)
{
}

void ClientSession::packGetGroups(ByteArray* byteArray)
{
}

void ClientSession::packFindUser(ByteArray* byteArray, const std::string& self, const std::string& other)
{
}

void ClientSession::packAddFriend(ByteArray* byteArray)
{
}

void ClientSession::packOperateFriend(ByteArray* byteArray)
{
}

void ClientSession::packUserStatusChange(ByteArray* byteArray)
{
}

void ClientSession::packUpdateUserInfo(ByteArray* byteArray)
{
}

void ClientSession::packModifyPassword(ByteArray* byteArray)
{
}

void ClientSession::packCreateGroup(ByteArray* byteArray)
{
}

void ClientSession::packGetGroupMembers(ByteArray* byteArray)
{
}

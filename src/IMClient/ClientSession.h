#pragma once

#include <string>
#include <QObject>
#include "base/noncopyable.h"
#include "base/Singleton.h"
#include "ByteArray.h"
#include "Protocol.h"
#include "base/crc64.h"
#include "MessageType.h"

class ClientSession : public QObject , noncopyable
{
	Q_OBJECT
public:
	friend class Singleton<ClientSession>;

public:
	void packHeartbeat(ByteArray* byteArray);
	void packRegister(ByteArray* byteArray, const std::string& userId, const std::string& password);
	void packLogin(ByteArray* byteArray, const std::string& userId, const std::string& password);
	void packGetFriendList(ByteArray* byteArray);
	void packGetGroups(ByteArray* byteArray);
	void packFindUser(ByteArray* byteArray, const std::string& selfId, const std::string& otherId);
	void packAddFriend(ByteArray* byteArray);
	void packOperateFriend(ByteArray* byteArray);
	void packUserStatusChange(ByteArray* byteArray);
	void packUpdateUserInfo(ByteArray* byteArray);
	void packModifyPassword(ByteArray* byteArray);
	void packCreateGroup(ByteArray* byteArray);
	void packGetGroupMembers(ByteArray* byteArray);
	void packChat(ByteArray* byteArray, const std::string& from, const std::string& to, MessageType::Type type, const QByteArray& data, uint32_t seq);
	void packMultiChat(ByteArray* byteArray, const std::string& from, const std::string& to, MessageType::Type type, const std::string& data);

	uint64_t getCRC64(const std::string& data);

private:
	explicit ClientSession() = default;
	~ClientSession() = default;

	

private:

};
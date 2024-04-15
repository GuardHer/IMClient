/*****************************************************************//**
 * @file   Message.h
 * @brief  
 * @author 12910
 * @date   January 2024 26
 *********************************************************************/

#pragma once
#include <QString>
#include <QByteArray>
#include <QDateTime>

#include "MessageType.h"


class Message
{
public:
	Message(const QString& talker, bool isSender, MessageType::Type type);
	Message() = default;
	virtual ~Message() = default;

	bool operator==(const Message& other) const {
		// 在这里实现比较逻辑，比如判断 talkerId 是否相等
		return this->getTalker() == other.getTalker();
	}


public:
	virtual QString getContent() const;              // 获取消息内容
	virtual QString getTalker() const;               // 获取消息发送者
	virtual bool getIsSender() const;                // 获取是否是发送者
	virtual bool getIsFriend() const;                // 获取是否是好友
	virtual MessageType::Type getType() const;             // 获取消息类型
	virtual QByteArray getByteContent() const;       // 获取消息内容，字节流
	virtual QDateTime getTime() const;               // 获取消息发送时间
	virtual QString getStrTime() const;              // 获取消息发送时间，字符串格式
	virtual int getStatus() const;                   // 获取消息状态

	virtual void setContent(const QString& content); // 设置消息内容
	virtual void setTalker(const QString& talker);   // 设置消息发送者
	virtual void setIsSender(bool isSender);         // 设置是否是发送者
	virtual void setIsFriend(bool isFriend);         // 设置是否是好友
	virtual void setType(MessageType::Type type);          // 设置消息类型
	virtual void setByteContent(const QByteArray& byteContent); // 设置消息内容，字节流
	virtual void setTime(const QDateTime& time);     // 设置消息发送时间
	virtual void setStatus(int status);              // 设置消息状态


private:
	QString strContent;       //消息内容
	QByteArray byteContent;   //消息内容，字节流
	QString strTalker;        //消息发送者
	bool IsSender;            //是否是发送者
	bool IsFriend;            //是否是好友
	MessageType::Type Type;		  //消息类型
	QDateTime Time;           //消息发送时间
	int Status;               //消息状态
};

class FriendMessage : public Message
{

};

class GroupMessage : public Message
{

};

class SystemMessage : public Message
{

};

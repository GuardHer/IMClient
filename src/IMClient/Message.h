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
		// ������ʵ�ֱȽ��߼��������ж� talkerId �Ƿ����
		return this->getTalker() == other.getTalker();
	}


public:
	virtual QString getContent() const;              // ��ȡ��Ϣ����
	virtual QString getTalker() const;               // ��ȡ��Ϣ������
	virtual bool getIsSender() const;                // ��ȡ�Ƿ��Ƿ�����
	virtual bool getIsFriend() const;                // ��ȡ�Ƿ��Ǻ���
	virtual MessageType::Type getType() const;             // ��ȡ��Ϣ����
	virtual QByteArray getByteContent() const;       // ��ȡ��Ϣ���ݣ��ֽ���
	virtual QDateTime getTime() const;               // ��ȡ��Ϣ����ʱ��
	virtual QString getStrTime() const;              // ��ȡ��Ϣ����ʱ�䣬�ַ�����ʽ
	virtual int getStatus() const;                   // ��ȡ��Ϣ״̬

	virtual void setContent(const QString& content); // ������Ϣ����
	virtual void setTalker(const QString& talker);   // ������Ϣ������
	virtual void setIsSender(bool isSender);         // �����Ƿ��Ƿ�����
	virtual void setIsFriend(bool isFriend);         // �����Ƿ��Ǻ���
	virtual void setType(MessageType::Type type);          // ������Ϣ����
	virtual void setByteContent(const QByteArray& byteContent); // ������Ϣ���ݣ��ֽ���
	virtual void setTime(const QDateTime& time);     // ������Ϣ����ʱ��
	virtual void setStatus(int status);              // ������Ϣ״̬


private:
	QString strContent;       //��Ϣ����
	QByteArray byteContent;   //��Ϣ���ݣ��ֽ���
	QString strTalker;        //��Ϣ������
	bool IsSender;            //�Ƿ��Ƿ�����
	bool IsFriend;            //�Ƿ��Ǻ���
	MessageType::Type Type;		  //��Ϣ����
	QDateTime Time;           //��Ϣ����ʱ��
	int Status;               //��Ϣ״̬
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

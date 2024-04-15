#include "Message.h"

Message::Message(const QString& talker, bool isSender, MessageType::Type type)
        : strTalker(talker), IsSender(isSender), Type(type), Status(0), IsFriend(true)
{
}

QString Message::getContent() const
{
    return strContent;
}

QString Message::getTalker() const
{
    return strTalker;
}

bool Message::getIsSender() const
{
    return IsSender;
}

bool Message::getIsFriend() const
{
    return IsFriend;
}

MessageType::Type Message::getType() const
{
    return Type;
}

QByteArray Message::getByteContent() const
{
    return byteContent;
}

QDateTime Message::getTime() const
{
    return Time;
}

QString Message::getStrTime() const
{
    return Time.toString("yyyy-MM-dd hh:mm:ss");
}

int Message::getStatus() const
{
    return Status;
}

void Message::setContent(const QString& content)
{
    strContent.clear();
    strContent = content;
}

void Message::setTalker(const QString& talker)
{
    strTalker = talker;
}

void Message::setIsSender(bool isSender)
{
    IsSender = isSender;
}

void Message::setIsFriend(bool isFriend)
{
	IsFriend = isFriend;
}

void Message::setType(MessageType::Type type)
{
    Type = type;
}

void Message::setByteContent(const QByteArray& byteContent)
{
    this->byteContent.clear();
	this->byteContent = byteContent;
}

void Message::setTime(const QDateTime& time)
{
    this->Time = time;
}

void Message::setStatus(int status)
{
    Status = status;
}

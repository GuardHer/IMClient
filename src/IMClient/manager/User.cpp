#include "User.h"

void User::setUserId(const QString& userid)
{
	userid_ = userid;
}
void User::setUserName(const QString& username)
{
	if (username.isEmpty())
		username_ = userid_;
	else
		username_ = username;
}
void User::setPassword(const QString& password)
{
	password_ = password;
}
void User::setNickName(const QString& nickname)
{
	nickname_ = nickname;
}
void User::setFaceType(const QString& facetype)
{
	facetype_ = facetype;
}
void User::setCustomFace(const QString& customface)
{
	customface_ = customface;
}
void User::setCustomFaceFmt(const QString& customfacefmt)
{
	customfacefmt_ = customfacefmt;
}
void User::setGender(const QString& gender)
{
	gender_ = gender;
}
void User::setBirthday(const QString& birthday)
{
	birthday_ = birthday;
}
void User::setSignature(const QString& signature)
{
	signature_ = signature;
}
void User::setAddress(const QString& address)
{
	address_ = address;
}
void User::setPhoneNumber(const QString& phonenumber)
{
	phonenumber_ = phonenumber;
}
void User::setMail(const QString& mail)
{
	mail_ = mail;
}
void User::setOwnerId(const QString& ownerid)
{
	ownerid_ = ownerid;
}
void User::setStatus(Status status)
{
	status_ = status;
}

void User::setStatus(int status)
{
	status_ = intToStatus(status);
}


void User::setLastMsg(const QString& lastmsg)
{
	lastMsg_ = lastmsg;
}

QString User::getLastMsg() const
{
	return lastMsg_;
}
void User::setStatusString(const QString& statusString)
{
	status_ = stringToStatus(statusString);
}
void User::setRegisterTime(const QString& registertime)
{
}
QString User::getUserId() const
{
	return userid_;
}
QString User::getUserName() const
{
	return username_;
}
QString User::getPassword() const
{
	return password_;
}
QString User::getNickName() const
{
	return nickname_;
}
QString User::getFaceType() const
{
	return facetype_;
}
QString User::getCustomFace() const
{
	return customface_;
}
QString User::getCustomFaceFmt() const
{
	return customfacefmt_;
}
QString User::getGender() const
{
	return gender_;
}
QString User::getBirthday() const
{
	return birthday_;
}
QString User::getSignature() const
{
	return signature_;
}
QString User::getAddress() const
{
	return address_;
}
QString User::getPhoneNumber() const
{
	return phonenumber_;
}
QString User::getMail() const
{
	return mail_;
}
QString User::getOwnerId() const
{
	return ownerid_;
}
Status User::getStatus() const
{
	return status_;
}
QString User::getStatusString() const
{
	return statusToString(status_);
}

QString User::getRegisterTime() const
{
	return registertime_;
}

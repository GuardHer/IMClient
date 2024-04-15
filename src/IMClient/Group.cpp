/*****************************************************************//**
 * @file   Group.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "Group.h"

#include "manager/User.h"

#include <QString>

Group::Group()
{
}

Group::Group(const QString groupId, const QString& name)
	: groupId_(groupId)
	, groupName_(name)
	, groupOwnerId_(QString())
	, groupOwnerName_(QString())
	, groupNotice_(QString())
	, groupMemberList_(QVector<QString>())
	, hasNewMessages_(false)
	, userWasMentioned_(false)
	, groupCreateTime_(QString())
{
}

QString Group::getGroupId() const
{
	return groupId_;
}

void Group::setGroupId(const QString& groupId)
{
	groupId_ = groupId;
}

QString Group::getGroupName() const
{
	return groupName_;
}

void Group::setGroupName(const QString& groupName)
{
	groupName_ = groupName;
}

QString Group::getGroupOwnerId() const
{
	return groupOwnerId_;
}

void Group::updateGroupOwnerId(const QString& newOwnerId)
{
	groupOwnerId_ = newOwnerId;
}

QString Group::getGroupOwnerName() const
{
	return groupOwnerName_;
}

void Group::updateGroupOwnerName(const QString& newOwnerName)
{
	groupOwnerName_ = newOwnerName;
}

QString Group::getGroupNotice() const
{
	return groupNotice_;
}

void Group::setGroupNotice(const QString& notice)
{
	groupNotice_ = notice;
}

int Group::getGroupMemberCount() const
{
	return groupMemberList_.size();
}

void Group::setGroupMemberCount(int count)
{
	grioupMemberCount_ = count;
}

QVector<QString> Group::getGroupMemberList() const
{
	return groupMemberList_;
}

QString Group::getGroupMemberListString() const
{
	QString memberList;
	for (const auto& member : groupMemberList_)
	{
		memberList += member + ",";
	}
	return memberList;
}

void Group::setGroupMemberList(const QList<QString>& memberList)
{
	if (memberList.isEmpty()){
		groupMemberList_.clear();
		for (const auto& member : memberList)
		{
			groupMemberList_.push_back(member);
		}
	}
	else {
		return;
	}
	
}

void Group::setGroupMemberListForString(const QString memberList)
{
	if (memberList.isEmpty()) {
		groupMemberList_.clear();
		QStringList list = memberList.split(",");
		for (const auto& member : list)
		{
			groupMemberList_.push_back(member);
		}
	}
	else {
		return;
	}

}

void Group::addGroupMember(const QString& userId)
{
	groupMemberList_.push_back(userId);
}

QVector<QString> Group::getDisplayNameList() const
{
	return displayNameList_;
}

QString Group::getDisplayNameString() const
{
	QString nameList;
	for (const auto& name : displayNameList_)
	{
		nameList += name + ",";
	}
	return nameList;
}

void Group::setDisplayNameList(const QList<QString>& nameList)
{
	displayNameList_.clear();
	for (const auto& name : nameList)
	{
		displayNameList_.push_back(name);
	}
}

void Group::setDisplayNameListForString(const QString nameList)
{
	displayNameList_.clear();
	QStringList list = nameList.split(",");
	for (const auto& name : list)
	{
		displayNameList_.push_back(name);
	}
}

void Group::setEventFlag(bool f)
{
	hasNewMessages_ = f;
}

bool Group::getEventFlag() const
{
	return hasNewMessages_;
}

void Group::setMentionedFlag(bool f)
{
	userWasMentioned_ = f;
}

bool Group::getMentionedFlag() const
{
	return userWasMentioned_;
}

QString Group::getGroupCreateTime() const
{
	return groupCreateTime_;
}

void Group::setGroupCreateTime(const QString& createTime)
{
	groupCreateTime_ = createTime;
}

QString Group::getCustomFace() const
{
	return customFace_;
}

void Group::setCustomFace(const QString& customFace)
{
	customFace_ = customFace;
}


QString Group::getLastMsg() const
{
	return lastMsg_;
}

void Group::setLastMsg(const QString& lastMsg)
{
	lastMsg_ = lastMsg;
}

QString Group::getLastMsgTime() const
{
	return lastMsgTime_;
}

void Group::setLastMsgTime(const QString& lastMsgTime)
{
	lastMsgTime_ = lastMsgTime;
}

QString Group::getSelfDisplayName() const
{
	return selfDisplayName_;
}

void Group::setSelfDisplayName(const QString& selfDisplayName)
{
	selfDisplayName_ = selfDisplayName;
}

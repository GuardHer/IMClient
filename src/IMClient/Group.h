/*****************************************************************//**
 * @file   Group.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024
 *********************************************************************/

#pragma once

#include <QVector>
#include <QObject>

class User;
class QString;

class Group
{
public:
    Group();
    Group(const QString groupId, const QString& name);

    QString getGroupId() const;                                   // 获取群组ID
    void setGroupId(const QString& groupId);  				      // 设置群组ID

    QString getGroupName() const;								  // 获取群组名称
    void setGroupName(const QString& groupName);				  // 设置群组名称

    QString getGroupOwnerId() const;							  // 获取群主ID
    void updateGroupOwnerId(const QString& newOwnerId);			  // 更新群主ID
    QString getGroupOwnerName() const;							  // 获取群主名称
    void updateGroupOwnerName(const QString& newOwnerName);		  // 更新群主名称

    QString getGroupNotice() const;								  // 获取群公告
    void setGroupNotice(const QString& notice);					  // 设置群公告

    int getGroupMemberCount() const;							  // 获取群成员数量
    void setGroupMemberCount(int count);						  // 设置群成员数量

    QVector<QString> getGroupMemberList() const;			      // 获取群成员列表
    QString getGroupMemberListString() const;					  // 获取群成员列表字符串
    void setGroupMemberList(const QList<QString>& memberList);	  // 设置群成员列表
    void setGroupMemberListForString(const QString memberList);   // 设置群成员列表
    void addGroupMember(const QString& userId);					  // 添加群成员

    QVector<QString> getDisplayNameList() const;			      // 获取群成员名列表
    QString getDisplayNameString() const;					      // 获取群成员名列表字符串
    void setDisplayNameList(const QList<QString>& nameList);	  // 设置群成员名列表
    void setDisplayNameListForString(const QString nameList);  // 设置群成员名列表

    void setEventFlag(bool f);                                    // 设置事件标志
    bool getEventFlag() const; 								      // 获取事件标志

    void setMentionedFlag(bool f); 							      // 设置提及标志
    bool getMentionedFlag() const; 								  // 获取提及标志

    QString getGroupCreateTime() const; 						  // 获取群组创建时间
    void setGroupCreateTime(const QString& time); 				  // 设置群组创建时间

    QString getCustomFace() const;								  // 获取自定义头像
    void setCustomFace(const QString& customFace);				  // 设置自定义头像

    QString getLastMsg() const;									  // 获取最后一条消息
    void setLastMsg(const QString& lastMsg);					  // 设置最后一条消息

    QString getLastMsgTime() const;								  // 获取最后一条消息时间
    void setLastMsgTime(const QString& lastMsgTime);			  // 设置最后一条消息时间

    QString getSelfDisplayName() const;							  // 获取自己的显示名称
    void setSelfDisplayName(const QString& selfDisplayName);		  // 设置自己的显示名称

private:

    // 群组的标题
    QString groupName_{};                 // 群组名称
    QString groupId_{};                   // 群组ID
    QString groupOwnerId_{};              // 群主ID
    QString groupOwnerName_{};            // 群主名称
    QString groupNotice_{};               // 群公告
    QString grioupMemberCount_{};         // 群成员数量
    QVector<QString> groupMemberList_{};  // 群成员列表
    QVector<QString> displayNameList_{};  // 群成员显示名称列表
    QString selfDisplayName_{};           // 自己的显示名称
    QString groupCreateTime_{};           // 群组创建时间

    QString customFace_{};				// 自定义头像
    QString lastMsg_{};                   // 最后一条消息
    QString lastMsgTime_{};               // 最后一条消息时间

    
    bool hasNewMessages_{ false };               // 是否有新消息
    bool userWasMentioned_{ false };             // 用户被提及
};

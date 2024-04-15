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

    QString getGroupId() const;                                   // ��ȡȺ��ID
    void setGroupId(const QString& groupId);  				      // ����Ⱥ��ID

    QString getGroupName() const;								  // ��ȡȺ������
    void setGroupName(const QString& groupName);				  // ����Ⱥ������

    QString getGroupOwnerId() const;							  // ��ȡȺ��ID
    void updateGroupOwnerId(const QString& newOwnerId);			  // ����Ⱥ��ID
    QString getGroupOwnerName() const;							  // ��ȡȺ������
    void updateGroupOwnerName(const QString& newOwnerName);		  // ����Ⱥ������

    QString getGroupNotice() const;								  // ��ȡȺ����
    void setGroupNotice(const QString& notice);					  // ����Ⱥ����

    int getGroupMemberCount() const;							  // ��ȡȺ��Ա����
    void setGroupMemberCount(int count);						  // ����Ⱥ��Ա����

    QVector<QString> getGroupMemberList() const;			      // ��ȡȺ��Ա�б�
    QString getGroupMemberListString() const;					  // ��ȡȺ��Ա�б��ַ���
    void setGroupMemberList(const QList<QString>& memberList);	  // ����Ⱥ��Ա�б�
    void setGroupMemberListForString(const QString memberList);   // ����Ⱥ��Ա�б�
    void addGroupMember(const QString& userId);					  // ���Ⱥ��Ա

    QVector<QString> getDisplayNameList() const;			      // ��ȡȺ��Ա���б�
    QString getDisplayNameString() const;					      // ��ȡȺ��Ա���б��ַ���
    void setDisplayNameList(const QList<QString>& nameList);	  // ����Ⱥ��Ա���б�
    void setDisplayNameListForString(const QString nameList);  // ����Ⱥ��Ա���б�

    void setEventFlag(bool f);                                    // �����¼���־
    bool getEventFlag() const; 								      // ��ȡ�¼���־

    void setMentionedFlag(bool f); 							      // �����ἰ��־
    bool getMentionedFlag() const; 								  // ��ȡ�ἰ��־

    QString getGroupCreateTime() const; 						  // ��ȡȺ�鴴��ʱ��
    void setGroupCreateTime(const QString& time); 				  // ����Ⱥ�鴴��ʱ��

    QString getCustomFace() const;								  // ��ȡ�Զ���ͷ��
    void setCustomFace(const QString& customFace);				  // �����Զ���ͷ��

    QString getLastMsg() const;									  // ��ȡ���һ����Ϣ
    void setLastMsg(const QString& lastMsg);					  // �������һ����Ϣ

    QString getLastMsgTime() const;								  // ��ȡ���һ����Ϣʱ��
    void setLastMsgTime(const QString& lastMsgTime);			  // �������һ����Ϣʱ��

    QString getSelfDisplayName() const;							  // ��ȡ�Լ�����ʾ����
    void setSelfDisplayName(const QString& selfDisplayName);		  // �����Լ�����ʾ����

private:

    // Ⱥ��ı���
    QString groupName_{};                 // Ⱥ������
    QString groupId_{};                   // Ⱥ��ID
    QString groupOwnerId_{};              // Ⱥ��ID
    QString groupOwnerName_{};            // Ⱥ������
    QString groupNotice_{};               // Ⱥ����
    QString grioupMemberCount_{};         // Ⱥ��Ա����
    QVector<QString> groupMemberList_{};  // Ⱥ��Ա�б�
    QVector<QString> displayNameList_{};  // Ⱥ��Ա��ʾ�����б�
    QString selfDisplayName_{};           // �Լ�����ʾ����
    QString groupCreateTime_{};           // Ⱥ�鴴��ʱ��

    QString customFace_{};				// �Զ���ͷ��
    QString lastMsg_{};                   // ���һ����Ϣ
    QString lastMsgTime_{};               // ���һ����Ϣʱ��

    
    bool hasNewMessages_{ false };               // �Ƿ�������Ϣ
    bool userWasMentioned_{ false };             // �û����ἰ
};

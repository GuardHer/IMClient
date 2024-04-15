/*****************************************************************//**
 * @file   FriendList.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "FriendList.h"

#include "Friend.h"
#include "Contactid.h"
#include <QDebug>
#include <QHash>
#include <QMenu>

// ��̬��Ա��������
QHash<QString, Friend*> FriendList::friendList;  // �����б�
QHash<QString, QString> FriendList::id2name;     // ID�����Ƶ�ӳ��

// ��Ӻ���
Friend* FriendList::addFriend(QString friendId, const QString& userName)
{
    // ����Ƿ��Ѵ��ڸú���
    auto friendChecker = friendList.find(friendId);
    if (friendChecker != friendList.end()) {
        qWarning() << "addFriend: friendId already taken";
    }

    Friend* newfriend = new Friend(friendId, userName);

    // ��ӵ������б�
    friendList[friendId] = newfriend;
    id2name[friendId] = userName;

    return newfriend;
}

// ���Һ���
Friend* FriendList::findFriend(const QString& friendId)
{
    // �ڹ�ϣ���в��Һ���
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        return *f_it;
    }

    return nullptr;
}

// ����ID��ȡName
const QString& FriendList::id2Key(QString friendId)
{
    return id2name[friendId];
}

/**
 * @function removeFriend
 * @brief ɾ������
 * @param friendId: ����ID
 * @param fake: �Ƿ��ɾ��
 */
void FriendList::removeFriend(const QString& friendId, bool fake)
{
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        // ������Ǽ�ɾ����������������
        if (!fake);
            //Settings::getInstance().removeFriendSettings(f_it.value()->getPublicKey());
        friendList.erase(f_it); // erase: ɾ��������ָ���Ԫ��
    }
}

// ��պ����б�
void FriendList::clear()
{
    // ������ɾ������Friend����
    for (auto friendptr : friendList)
        delete friendptr;
    friendList.clear();
}

// ��ȡ���к���
QList<Friend*> FriendList::getAllFriends()
{
    return friendList.values();
}

// ���������ǳ�
QString FriendList::decideNickname(const QString& friendId, const QString& origName)
{
    // ���Һ��ѣ�������ʾ���ƻ�ԭʼ����
    Friend* f = FriendList::findFriend(friendId);
    if (f != nullptr) {
        return f->getDisplayedName();
    }
    else if (!origName.isEmpty()) {
        return origName;
    }
    else {
        return friendId;
    }
}


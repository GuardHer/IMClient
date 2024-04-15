/*****************************************************************//**
 * @file   UserList.cpp
 * @brief
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "UserList.h"

#include "manager/User.h"
#include "Contactid.h"
#include <QDebug>
#include <QHash>
#include <QMenu>

 // ��̬��Ա��������
QHash<QString, User*> UserList::friendList;  // �����б�
QHash<QString, QString> UserList::id2name;     // ID�����Ƶ�ӳ��
QList<QString> UserList::friendIdList;      // ����ID�б�

// ��Ӻ���
User* UserList::addFriend(QString friendId, const QString& userName)
{
    // ����Ƿ��Ѵ��ڸú���
    auto friendChecker = friendList.find(friendId);
    if (friendChecker != friendList.end()) {
        qWarning() << "addFriend: friendId already taken";
    }

    User* newfriend = new User(friendId, userName);

    // ��ӵ������б�
    friendList[friendId] = newfriend;
    id2name[friendId] = userName;

    // ��ӵ�����ID�б�
    if (!friendIdList.contains(friendId)) // contains: �ж��б����Ƿ����ĳ��Ԫ��
		friendIdList.append(friendId);

    return newfriend;
}

User* UserList::addFriend(User* friendUser)
{
    // ����Ƿ��Ѵ��ڸú���
	auto friendChecker = friendList.find(friendUser->getUserId());
    if (friendChecker != friendList.end()) {
		qWarning() << "addFriend: friendId already taken";
	}

	// ��ӵ������б�
	friendList[friendUser->getUserId()] = friendUser;
	id2name[friendUser->getUserId()] = friendUser->getUserName();

	// ��ӵ�����ID�б�
	if (!friendIdList.contains(friendUser->getUserId()))
		friendIdList.append(friendUser->getUserId());

	return friendUser;
}

void UserList::setFriendList(const QHash<QString, User*>& newFriendList)
{
    friendList = newFriendList;

    // ���� friendIdList �� id2name
    friendIdList = friendList.keys();
    id2name.clear();
    //for (const auto& pair : friendList)
    //{
    //    id2name[pair.first] = pair.second->getUserName();

    //}
}

// ���Һ���
User* UserList::findFriend(const QString& friendId)
{
    // �ڹ�ϣ���в��Һ���
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        return *f_it;
    }

    return nullptr;
}

// ����ID��ȡName
const QString& UserList::id2Key(QString friendId)
{
    return id2name[friendId];
}

/**
 * @function removeFriend
 * @brief ɾ������
 * @param friendId: ����ID
 * @param fake: �Ƿ��ɾ��
 */
void UserList::removeFriend(const QString& friendId, bool fake)
{
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        // ������Ǽ�ɾ����������������
        if (!fake);
        //Settings::getInstance().removeFriendSettings(f_it.value()->getPublicKey());
        friendList.erase(f_it); // erase: ɾ��������ָ���Ԫ��
    }

    // �Ӻ���ID�б���ɾ��
    friendIdList.removeAll(friendId);  // removeAll: ɾ���б������е�Ԫ�� friendId: Ҫɾ����Ԫ��
}

// ��պ����б�
void UserList::clear()
{
    // ������ɾ������Friend����
    for (auto friendptr : friendList)
        delete friendptr;
    friendList.clear();

    // ���ID�б�
    friendIdList.clear();
}

// ��ȡ���к���
QList<User*> UserList::getAllFriends()
{
    return friendList.values();
}

// ���������ǳ�
QString UserList::decideNickname(const QString& friendId, const QString& origName)
{
    // ���Һ��ѣ�������ʾ���ƻ�ԭʼ����
    User* f = UserList::findFriend(friendId);
    if (f != nullptr) {
        return f->getUserName();
    }
    else if (!origName.isEmpty()) {
        return origName;
    }
    else {
        return friendId;
    }
}

bool UserList::isEmpty()
{
    return friendList.isEmpty();
}



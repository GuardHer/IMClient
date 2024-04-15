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

 // 静态成员变量定义
QHash<QString, User*> UserList::friendList;  // 好友列表
QHash<QString, QString> UserList::id2name;     // ID到名称的映射
QList<QString> UserList::friendIdList;      // 好友ID列表

// 添加好友
User* UserList::addFriend(QString friendId, const QString& userName)
{
    // 检查是否已存在该好友
    auto friendChecker = friendList.find(friendId);
    if (friendChecker != friendList.end()) {
        qWarning() << "addFriend: friendId already taken";
    }

    User* newfriend = new User(friendId, userName);

    // 添加到好友列表
    friendList[friendId] = newfriend;
    id2name[friendId] = userName;

    // 添加到好友ID列表
    if (!friendIdList.contains(friendId)) // contains: 判断列表中是否包含某个元素
		friendIdList.append(friendId);

    return newfriend;
}

User* UserList::addFriend(User* friendUser)
{
    // 检查是否已存在该好友
	auto friendChecker = friendList.find(friendUser->getUserId());
    if (friendChecker != friendList.end()) {
		qWarning() << "addFriend: friendId already taken";
	}

	// 添加到好友列表
	friendList[friendUser->getUserId()] = friendUser;
	id2name[friendUser->getUserId()] = friendUser->getUserName();

	// 添加到好友ID列表
	if (!friendIdList.contains(friendUser->getUserId()))
		friendIdList.append(friendUser->getUserId());

	return friendUser;
}

void UserList::setFriendList(const QHash<QString, User*>& newFriendList)
{
    friendList = newFriendList;

    // 更新 friendIdList 和 id2name
    friendIdList = friendList.keys();
    id2name.clear();
    //for (const auto& pair : friendList)
    //{
    //    id2name[pair.first] = pair.second->getUserName();

    //}
}

// 查找好友
User* UserList::findFriend(const QString& friendId)
{
    // 在哈希表中查找好友
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        return *f_it;
    }

    return nullptr;
}

// 根据ID获取Name
const QString& UserList::id2Key(QString friendId)
{
    return id2name[friendId];
}

/**
 * @function removeFriend
 * @brief 删除好友
 * @param friendId: 好友ID
 * @param fake: 是否假删除
 */
void UserList::removeFriend(const QString& friendId, bool fake)
{
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        // 如果不是假删除，则清除相关设置
        if (!fake);
        //Settings::getInstance().removeFriendSettings(f_it.value()->getPublicKey());
        friendList.erase(f_it); // erase: 删除迭代器指向的元素
    }

    // 从好友ID列表中删除
    friendIdList.removeAll(friendId);  // removeAll: 删除列表中所有的元素 friendId: 要删除的元素
}

// 清空好友列表
void UserList::clear()
{
    // 遍历并删除所有Friend对象
    for (auto friendptr : friendList)
        delete friendptr;
    friendList.clear();

    // 清空ID列表
    friendIdList.clear();
}

// 获取所有好友
QList<User*> UserList::getAllFriends()
{
    return friendList.values();
}

// 决定好友昵称
QString UserList::decideNickname(const QString& friendId, const QString& origName)
{
    // 查找好友，返回显示名称或原始名称
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



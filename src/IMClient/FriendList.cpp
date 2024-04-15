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

// 静态成员变量定义
QHash<QString, Friend*> FriendList::friendList;  // 好友列表
QHash<QString, QString> FriendList::id2name;     // ID到名称的映射

// 添加好友
Friend* FriendList::addFriend(QString friendId, const QString& userName)
{
    // 检查是否已存在该好友
    auto friendChecker = friendList.find(friendId);
    if (friendChecker != friendList.end()) {
        qWarning() << "addFriend: friendId already taken";
    }

    Friend* newfriend = new Friend(friendId, userName);

    // 添加到好友列表
    friendList[friendId] = newfriend;
    id2name[friendId] = userName;

    return newfriend;
}

// 查找好友
Friend* FriendList::findFriend(const QString& friendId)
{
    // 在哈希表中查找好友
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        return *f_it;
    }

    return nullptr;
}

// 根据ID获取Name
const QString& FriendList::id2Key(QString friendId)
{
    return id2name[friendId];
}

/**
 * @function removeFriend
 * @brief 删除好友
 * @param friendId: 好友ID
 * @param fake: 是否假删除
 */
void FriendList::removeFriend(const QString& friendId, bool fake)
{
    auto f_it = friendList.find(friendId);
    if (f_it != friendList.end()) {
        // 如果不是假删除，则清除相关设置
        if (!fake);
            //Settings::getInstance().removeFriendSettings(f_it.value()->getPublicKey());
        friendList.erase(f_it); // erase: 删除迭代器指向的元素
    }
}

// 清空好友列表
void FriendList::clear()
{
    // 遍历并删除所有Friend对象
    for (auto friendptr : friendList)
        delete friendptr;
    friendList.clear();
}

// 获取所有好友
QList<Friend*> FriendList::getAllFriends()
{
    return friendList.values();
}

// 决定好友昵称
QString FriendList::decideNickname(const QString& friendId, const QString& origName)
{
    // 查找好友，返回显示名称或原始名称
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


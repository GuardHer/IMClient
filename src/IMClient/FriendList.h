/*****************************************************************//**
 * @file   FriendList.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024
 *********************************************************************/

#pragma once
#include <cstdint>

template <class T>
class QList;
template <class A, class B>
class QHash;
class Friend;
class QByteArray;
class QString;

class FriendList
{
public:
    /**
     * @brief 添加好友，返回Friend对象的指针
     * @param friendId
     * @param friendPk
     * @return 
     */
    static Friend* addFriend(QString friendId, const QString& userName);

    /**
     * @brief 根据Tox公钥查找好友
     * @param friendPk
     * @return 
     */
    static Friend* findFriend(const QString& friendId);

    /**
     * @brief 将好友ID转换为Tox公钥
     * @param friendId
     * @return 
     */
    static const QString& id2Key(QString friendId);

    /**
     * @brief 获取所有好友列表
     * @return 
     */
    static QList<Friend*> getAllFriends();

    /**
     * @brief 移除好友
     * @param friendPk
     * @param fake
     */
    static void removeFriend(const QString& friendId, bool fake = false);

    /**
     * @brief 清空.
     */
    static void clear();

    /**
     * @brief 决定好友的昵称
     * @param friendPk: 好友的Tox公钥
     * @param origName: 原始昵称
     * @return 
     */
    static QString decideNickname(const QString& friendId, const QString& origName);

private:
    // 存储好友列表的哈希表
    static QHash<QString, Friend*> friendList;
    // 存储好友ID和Name的哈希表
    static QHash<QString, QString> id2name;
};


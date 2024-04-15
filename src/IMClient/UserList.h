/*****************************************************************//**
 * @file   UserList.h
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
class User;
class QByteArray;
class QString;

class UserList
{
public:
   

public:
    /**
     * @brief 添加好友，返回Friend对象的指针
     * @param friendId
     * @param friendPk
     * @return
     */
    static User* addFriend(QString friendId, const QString& userName);

    // 通过User对象添加好友
    static User* addFriend(User* friendUser);

    // 
    static void setFriendList(const QHash<QString, User*>& newFriendList);

    /**
     * @brief 根据Tox公钥查找好友
     * @param friendPk
     * @return
     */
    static User* findFriend(const QString& friendId);

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
    static QList<User*> getAllFriends();

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
     * @param friendId: 好友的Id
     * @param origName: 原始昵称
     * @return
     */
    static QString decideNickname(const QString& friendId, const QString& origName);

    // 判断是否为空
    static bool isEmpty();



private:
    // 存储好友列表的哈希表
    static QHash<QString, User*> friendList;
    // 存储好友ID和Name的哈希表
    static QHash<QString, QString> id2name;

public:
    // 好友列表( 只含id )
    static QList<QString> friendIdList;
};




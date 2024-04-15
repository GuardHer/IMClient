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
     * @brief ��Ӻ��ѣ�����Friend�����ָ��
     * @param friendId
     * @param friendPk
     * @return
     */
    static User* addFriend(QString friendId, const QString& userName);

    // ͨ��User������Ӻ���
    static User* addFriend(User* friendUser);

    // 
    static void setFriendList(const QHash<QString, User*>& newFriendList);

    /**
     * @brief ����Tox��Կ���Һ���
     * @param friendPk
     * @return
     */
    static User* findFriend(const QString& friendId);

    /**
     * @brief ������IDת��ΪTox��Կ
     * @param friendId
     * @return
     */
    static const QString& id2Key(QString friendId);

    /**
     * @brief ��ȡ���к����б�
     * @return
     */
    static QList<User*> getAllFriends();

    /**
     * @brief �Ƴ�����
     * @param friendPk
     * @param fake
     */
    static void removeFriend(const QString& friendId, bool fake = false);

    /**
     * @brief ���.
     */
    static void clear();

    /**
     * @brief �������ѵ��ǳ�
     * @param friendId: ���ѵ�Id
     * @param origName: ԭʼ�ǳ�
     * @return
     */
    static QString decideNickname(const QString& friendId, const QString& origName);

    // �ж��Ƿ�Ϊ��
    static bool isEmpty();



private:
    // �洢�����б�Ĺ�ϣ��
    static QHash<QString, User*> friendList;
    // �洢����ID��Name�Ĺ�ϣ��
    static QHash<QString, QString> id2name;

public:
    // �����б�( ֻ��id )
    static QList<QString> friendIdList;
};




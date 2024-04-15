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
     * @brief ��Ӻ��ѣ�����Friend�����ָ��
     * @param friendId
     * @param friendPk
     * @return 
     */
    static Friend* addFriend(QString friendId, const QString& userName);

    /**
     * @brief ����Tox��Կ���Һ���
     * @param friendPk
     * @return 
     */
    static Friend* findFriend(const QString& friendId);

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
    static QList<Friend*> getAllFriends();

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
     * @param friendPk: ���ѵ�Tox��Կ
     * @param origName: ԭʼ�ǳ�
     * @return 
     */
    static QString decideNickname(const QString& friendId, const QString& origName);

private:
    // �洢�����б�Ĺ�ϣ��
    static QHash<QString, Friend*> friendList;
    // �洢����ID��Name�Ĺ�ϣ��
    static QHash<QString, QString> id2name;
};


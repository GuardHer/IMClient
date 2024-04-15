/*****************************************************************//**
 * @file   GroupList.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#pragma once

#include "Group.h"

template <class A, class B>
class QHash;
template <class T>
class QList;
class QString;

class GroupList
{
public:
    static Group* addGroup(QString groupId, const QString& name);

    static Group* addGroup(Group* group);

    static void setGroupList(const QHash< QString, Group*>& newGroupList);
    
    static Group* findGroup(const QString& groupId);

    static const QString& id2Name(QString groupNum);

    static void removeGroup(const QString& groupId, bool fake = false);

    static QList<Group*> getAllGroups();

    static void clear();

    static bool isEmpty(); // �ж�Ⱥ���б��Ƿ�Ϊ��

private:
    // Ⱥ���б�
    static QHash<QString, Group*> groupList;

    // Ⱥ��idת��ΪȺ������
    static QHash<QString, QString> id2name;
};


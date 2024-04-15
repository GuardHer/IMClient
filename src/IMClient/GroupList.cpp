/*****************************************************************//**
 * @file   GroupList.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "GroupList.h"

#include "Group.h"
#include <QHash>
#include <QString>
#include "base/Logging.h"

QHash<QString, Group*> GroupList::groupList;
QHash<QString, QString> GroupList::id2name;

Group* GroupList::addGroup(QString groupId, const QString& groupName)
{
    auto checker = groupList.find(groupId);
    if (checker != groupList.end())                            // 如果群组ID已经存在
    {
        LOG_DEBUG << ("addGroup: groupId already taken");
        return nullptr;
    }
        
    Group* newGroup = new Group(groupId, groupName);
    groupList[groupId] = newGroup;
    id2name[groupId] = groupName;
    return newGroup;
}

Group* GroupList::addGroup(Group* group)
{
    auto checker = groupList.find(group->getGroupId());
    if (checker != groupList.end())                            // 如果群组ID已经存在
    {
        LOG_DEBUG << ("addGroup: groupId already taken");
		return nullptr;
	}

	groupList[group->getGroupId()] = group;
	id2name[group->getGroupId()] = group->getGroupName();
	return group;
}

void GroupList::setGroupList(const QHash<QString, Group*>& newGroupList)
{
    groupList = newGroupList;

    for (auto it : groupList)
		id2name[it->getGroupId()] = it->getGroupName();


}

Group* GroupList::findGroup(const QString& groupId)
{
    auto g_it = groupList.find(groupId);
    if (g_it != groupList.end())
        return *g_it;

    return nullptr;
}

const QString& GroupList::id2Name(QString groupNum)
{
    return id2name[groupNum];
}

void GroupList::removeGroup(const QString& groupId, bool /*fake*/)
{
    auto g_it = groupList.find(groupId);
    if (g_it != groupList.end()) {
        groupList.erase(g_it);
    }
}

QList<Group*> GroupList::getAllGroups()
{
    return groupList.values();
}

// 
void GroupList::clear()
{
    for (auto groupptr : groupList)
        delete groupptr;
    groupList.clear();
}

bool GroupList::isEmpty()
{
    return groupList.isEmpty();
}

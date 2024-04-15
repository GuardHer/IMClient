/*****************************************************************//**
 * @file   Friend.cpp
 * @brief  
 * @author 12910
 * @date   January 2024 10
 *********************************************************************/

#include "Friend.h"
#include "Status.h"


Friend::Friend(QString friendId, const QString& userName)
    : userName{ userName }
    , friendId{ friendId }
    , hasNewEvents{ false }
    , friendStatus{ Status::Status::Offline }
{
    if (userName.isEmpty()) {
        this->userName = friendId;
    }
}

/**
 * @brief Friend::set Name 为好友设置新用户名
 * @param 命名新用户名，如果名称为空则设置用户名为好友ID
 */
void Friend::setName(const QString& _name)
{
    QString name = _name;
    if (name.isEmpty()) {
        name = friendId;
    }

    // 保存旧的显示名称以便能够比较更改
    const auto oldDisplayed = getDisplayedName();

    if (userName != name) {
        userName = name;
        emit nameChanged(friendId, name);
    }

    const auto newDisplayed = getDisplayedName();
    if (oldDisplayed != newDisplayed) {
        emit displayedNameChanged(newDisplayed);
    }
}

/**
 * @function setStatusMessage
 * @brief 设置当前状态消息
 * @param message: 新的状态消息
 */
void Friend::setStatusMessage(const QString& message)
{
    if (statusMessage != message) {
        statusMessage = message;
        emit statusMessageChanged(friendId, message);
    }
}

/**
 * @function getStatusMessage
 * @brief 获取当前状态消息
 * @return: 当前状态消息
 */
QString Friend::getStatusMessage() const
{
    return statusMessage;
}

/**
 * @brief Friend::get Displayed Name 获取应为用户显示的名称
 * @return 包含别名、用户名或Id的 QString
 * @note 这个函数和相应的信号应该优先于获取直接名称或别名。
 */
QString Friend::getDisplayedName() const
{
    if (userName.isEmpty()) {
        return friendId;
    }

    return userName;
}


QString Friend::getUserName() const
{
    return userName;
}

QString Friend::getId() const
{
    return friendId;
}


void Friend::setEventFlag(bool flag)
{
    hasNewEvents = flag;
}

bool Friend::getEventFlag() const
{
    return hasNewEvents;
}

void Friend::setStatus(Status::Status s)
{
    if (friendStatus != s) {
        auto oldStatus = friendStatus;
        friendStatus = s;
        emit statusChanged(friendId, friendStatus);
        
        if (!Status::isOnline(oldStatus) && Status::isOnline(friendStatus)) {
            // 如果状态从离线变为在线，则发出信号
            emit onlineOfflineChanged(friendId, true);
        }
        else if (Status::isOnline(oldStatus) && !Status::isOnline(friendStatus)) {
            // 如果状态从在线变为离线，则发出信号
            emit onlineOfflineChanged(friendId, false);
        }

    }
}

Status::Status Friend::getStatus() const
{
    return friendStatus;
}

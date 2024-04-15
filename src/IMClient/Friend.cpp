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
 * @brief Friend::set Name Ϊ�����������û���
 * @param �������û������������Ϊ���������û���Ϊ����ID
 */
void Friend::setName(const QString& _name)
{
    QString name = _name;
    if (name.isEmpty()) {
        name = friendId;
    }

    // ����ɵ���ʾ�����Ա��ܹ��Ƚϸ���
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
 * @brief ���õ�ǰ״̬��Ϣ
 * @param message: �µ�״̬��Ϣ
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
 * @brief ��ȡ��ǰ״̬��Ϣ
 * @return: ��ǰ״̬��Ϣ
 */
QString Friend::getStatusMessage() const
{
    return statusMessage;
}

/**
 * @brief Friend::get Displayed Name ��ȡӦΪ�û���ʾ������
 * @return �����������û�����Id�� QString
 * @note �����������Ӧ���ź�Ӧ�������ڻ�ȡֱ�����ƻ������
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
            // ���״̬�����߱�Ϊ���ߣ��򷢳��ź�
            emit onlineOfflineChanged(friendId, true);
        }
        else if (Status::isOnline(oldStatus) && !Status::isOnline(friendStatus)) {
            // ���״̬�����߱�Ϊ���ߣ��򷢳��ź�
            emit onlineOfflineChanged(friendId, false);
        }

    }
}

Status::Status Friend::getStatus() const
{
    return friendStatus;
}

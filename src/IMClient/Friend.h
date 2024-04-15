/*****************************************************************//**
 * @file   Friend.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024
 *********************************************************************/

#ifndef FRIEND_H
#define FRIEND_H

#include "Contact.h"
#include "Status.h"
#include <QObject>
#include <QString>

class Friend : public Contact
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数，创建一个Friend对象
     * @param friendId: 用户ID
     * @param userName: 用户名
     */
    Friend(QString friendId, const QString& userName = {});

    /**
     * @brief 禁止拷贝构造函数
     * @param other: 
     */
    Friend(const Friend& other) = delete;

    /**
     * @brief 禁止赋值操作符
     * @param other: 
     * @return : 
     */
    Friend& operator=(const Friend& other) = delete;

    /**
     * @brief 设置用户昵称
     * @param name: 用户昵称
     */
    void setName(const QString& name) override;

    /**
     * @brief 获取显示的用户名称
     * @return : 
     * @note: override: 表示重写基类的虚函数
     */
    QString getDisplayedName() const override;

    /**
     * @brief 获取用户用户名
     * @return: 用户用户名
     */
    QString getUserName() const;

    /**
     * @brief 获取用户用户名
     * @param message: 用户状态消息
     */
    void setStatusMessage(const QString& message);

    /**
     * @brief 获取用户用户名
     * @return: 用户用户名
     */
    QString getStatusMessage() const;

    /**
     * @brief 设置事件标志，表示是否有新事件
     * @param f: 事件标志
     */
    void setEventFlag(bool f) override;

    /**
     * @brief 获取事件标志，表示是否有新事件
     * @return: 事件标志
     */
    bool getEventFlag() const override;

    /**
     * @brief 获取用户ID
     * @return: 用户ID
     */
    QString getId() const override;

    /**
	 * @brief 设置用户ID
	 * @param id: 用户ID
	 */
    void setStatus(Status::Status s);

    /**
	 * @brief 获取用户状态
	 * @return: 用户状态
	 */
    Status::Status getStatus() const;

signals:
    // 用户名发生变化的信号
    void nameChanged(const QString& friendId, const QString& name);

    // 用户状态发生变化的信号
    void statusChanged(const QString& friendId, Status::Status status);

    // 用户上下线状态发生变化的信号
    void onlineOfflineChanged(const QString& friendId, bool isOnline);

    // 用户状态消息发生变化的信号
    void statusMessageChanged(const QString& friendId, const QString& message);

    // 加载聊天历史记录的信号
    void loadChatHistory();

public slots:

private:
    // 用户ID
    QString friendId;

    // 用户用户名
    QString userName;

    // 用户状态消息
    QString statusMessage;

    // 是否有新事件标志
    bool hasNewEvents;

    // 用户状态
    Status::Status friendStatus;
};

#endif // FRIEND_H

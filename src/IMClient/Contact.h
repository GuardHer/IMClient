/*****************************************************************//**
 * @file   Contact.h
 * @brief  
 * 
 * @author 12910
 * @date   January 2024
 *********************************************************************/

#pragma once

#include <QObject>
#include <QString>

/**
 * @brief 联系人接口
 */
class Contact : public QObject
{
    Q_OBJECT
public:
    virtual ~Contact() = 0;

    /**
     * @brief 设置联系人名称
     * @param name 联系人名称
     */
    virtual void setName(const QString& name) = 0;

    /**
     * @brief 获取用于显示的联系人名称
     * @return 用于显示的联系人名称
     */
    virtual QString getDisplayedName() const = 0;

    /**
     * @brief 获取联系人ID
     * @return 联系人ID
     */
    virtual QString getId() const = 0;

    /**
     * @brief 设置事件标志
     * @param flag 事件标志
     */
    virtual void setEventFlag(bool flag) = 0;

    /**
     * @brief 获取事件标志
     * @return 事件标志
     */
    virtual bool getEventFlag() const = 0;

signals:
    /**
     * @brief 联系人显示名称改变时发射的信号
     * @param newName 新的显示名称
     */
    void displayedNameChanged(const QString& newName);
};

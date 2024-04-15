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
 * @brief ��ϵ�˽ӿ�
 */
class Contact : public QObject
{
    Q_OBJECT
public:
    virtual ~Contact() = 0;

    /**
     * @brief ������ϵ������
     * @param name ��ϵ������
     */
    virtual void setName(const QString& name) = 0;

    /**
     * @brief ��ȡ������ʾ����ϵ������
     * @return ������ʾ����ϵ������
     */
    virtual QString getDisplayedName() const = 0;

    /**
     * @brief ��ȡ��ϵ��ID
     * @return ��ϵ��ID
     */
    virtual QString getId() const = 0;

    /**
     * @brief �����¼���־
     * @param flag �¼���־
     */
    virtual void setEventFlag(bool flag) = 0;

    /**
     * @brief ��ȡ�¼���־
     * @return �¼���־
     */
    virtual bool getEventFlag() const = 0;

signals:
    /**
     * @brief ��ϵ����ʾ���Ƹı�ʱ������ź�
     * @param newName �µ���ʾ����
     */
    void displayedNameChanged(const QString& newName);
};

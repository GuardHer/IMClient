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
     * @brief ���캯��������һ��Friend����
     * @param friendId: �û�ID
     * @param userName: �û���
     */
    Friend(QString friendId, const QString& userName = {});

    /**
     * @brief ��ֹ�������캯��
     * @param other: 
     */
    Friend(const Friend& other) = delete;

    /**
     * @brief ��ֹ��ֵ������
     * @param other: 
     * @return : 
     */
    Friend& operator=(const Friend& other) = delete;

    /**
     * @brief �����û��ǳ�
     * @param name: �û��ǳ�
     */
    void setName(const QString& name) override;

    /**
     * @brief ��ȡ��ʾ���û�����
     * @return : 
     * @note: override: ��ʾ��д������麯��
     */
    QString getDisplayedName() const override;

    /**
     * @brief ��ȡ�û��û���
     * @return: �û��û���
     */
    QString getUserName() const;

    /**
     * @brief ��ȡ�û��û���
     * @param message: �û�״̬��Ϣ
     */
    void setStatusMessage(const QString& message);

    /**
     * @brief ��ȡ�û��û���
     * @return: �û��û���
     */
    QString getStatusMessage() const;

    /**
     * @brief �����¼���־����ʾ�Ƿ������¼�
     * @param f: �¼���־
     */
    void setEventFlag(bool f) override;

    /**
     * @brief ��ȡ�¼���־����ʾ�Ƿ������¼�
     * @return: �¼���־
     */
    bool getEventFlag() const override;

    /**
     * @brief ��ȡ�û�ID
     * @return: �û�ID
     */
    QString getId() const override;

    /**
	 * @brief �����û�ID
	 * @param id: �û�ID
	 */
    void setStatus(Status::Status s);

    /**
	 * @brief ��ȡ�û�״̬
	 * @return: �û�״̬
	 */
    Status::Status getStatus() const;

signals:
    // �û��������仯���ź�
    void nameChanged(const QString& friendId, const QString& name);

    // �û�״̬�����仯���ź�
    void statusChanged(const QString& friendId, Status::Status status);

    // �û�������״̬�����仯���ź�
    void onlineOfflineChanged(const QString& friendId, bool isOnline);

    // �û�״̬��Ϣ�����仯���ź�
    void statusMessageChanged(const QString& friendId, const QString& message);

    // ����������ʷ��¼���ź�
    void loadChatHistory();

public slots:

private:
    // �û�ID
    QString friendId;

    // �û��û���
    QString userName;

    // �û�״̬��Ϣ
    QString statusMessage;

    // �Ƿ������¼���־
    bool hasNewEvents;

    // �û�״̬
    Status::Status friendStatus;
};

#endif // FRIEND_H

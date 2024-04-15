#pragma once
#include <QWidget> 
#include <QLabel>
#include <QHBoxLayout>
#include "widget/RoundLabel.h"

class FriendItem: public QWidget
{
	Q_OBJECT
public:
	FriendItem(QWidget *parent = 0);
	~FriendItem();


public:
	void setFriendInfo(const QString& headImage, const QString& name, const QString& userId, const QString& msg, const QString& time);  // ���ú�����Ϣ
	void setLabelStyle();   // ���ñ�ǩ��ʽ

	void updateHeadImage(const QString& headImage);   // ����ͷ��
	void updateName(const QString& name);             // ��������
	void updateUserId(const QString& userId);         // ����ID
	void updateMsg(const QString& msg);               // ������Ϣ
	void updateTime(const QString& time);             // ����ʱ��

public:
	QString getHeadImage() const;   // ��ȡͷ��
	QString getName() const;        // ��ȡ����
	QString getUserId() const;      // ��ȡID
	QString getMsg() const;         // ��ȡ��Ϣ
	QString getTime() const;        // ��ȡʱ��

public:
	void initLayout();   // ��ʼ������

	void setHeadImage(const QString& headImage);   // ����ͷ��
	void setName(const QString& name);        // ��������
	void setUserId(const QString& userId);      // ����ID
	void setMsg(const QString& msg);         // ������Ϣ
	void setTime(const QString& time);        // ����ʱ��

private:
	QHBoxLayout *m_mainLayout;   // ������

	RoundLabel *m_headImage;   // ͷ��
	QLabel *m_name;        // ����
	QLabel *m_userId;      // ID
	QLabel *m_msg;         // ��Ϣ
	QLabel *m_time;        // ʱ��

	QString m_headImageStr;   // ͷ��
	QString m_nameStr;        // ����
	QString m_userIdStr;      // ID
	QString m_msgStr;         // ��Ϣ
	QString m_timeStr;        // ʱ��

};


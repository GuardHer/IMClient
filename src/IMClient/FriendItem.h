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
	void setFriendInfo(const QString& headImage, const QString& name, const QString& userId, const QString& msg, const QString& time);  // 设置好友信息
	void setLabelStyle();   // 设置标签样式

	void updateHeadImage(const QString& headImage);   // 更新头像
	void updateName(const QString& name);             // 更新名字
	void updateUserId(const QString& userId);         // 更新ID
	void updateMsg(const QString& msg);               // 更新消息
	void updateTime(const QString& time);             // 更新时间

public:
	QString getHeadImage() const;   // 获取头像
	QString getName() const;        // 获取名字
	QString getUserId() const;      // 获取ID
	QString getMsg() const;         // 获取消息
	QString getTime() const;        // 获取时间

public:
	void initLayout();   // 初始化布局

	void setHeadImage(const QString& headImage);   // 设置头像
	void setName(const QString& name);        // 设置名字
	void setUserId(const QString& userId);      // 设置ID
	void setMsg(const QString& msg);         // 设置消息
	void setTime(const QString& time);        // 设置时间

private:
	QHBoxLayout *m_mainLayout;   // 主布局

	RoundLabel *m_headImage;   // 头像
	QLabel *m_name;        // 名字
	QLabel *m_userId;      // ID
	QLabel *m_msg;         // 消息
	QLabel *m_time;        // 时间

	QString m_headImageStr;   // 头像
	QString m_nameStr;        // 名字
	QString m_userIdStr;      // ID
	QString m_msgStr;         // 消息
	QString m_timeStr;        // 时间

};


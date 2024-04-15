#pragma execution_character_set("utf-8")

#include "FriendItem.h"
#include <QPixmap>


FriendItem::FriendItem(QWidget* parent)
{
	//this->setFixedSize(370, 90);
	initLayout();
	setLabelStyle();
}
// 请注意，这里没有进行析构函数的工作，因为我们假设 RoundLabel 和 QLabel 的析构函数能够正确释放资源。
FriendItem::~FriendItem()
{
	// 释放内存
	if (m_headImage != nullptr)
	{
		delete m_headImage;
		m_headImage = nullptr;
	}

	if (m_name != nullptr)
	{
		delete m_name;
		m_name = nullptr;
	}

	if (m_userId != nullptr)
	{
		delete m_userId;
		m_userId = nullptr;
	}

	if (m_msg != nullptr)
	{
		delete m_msg;
		m_msg = nullptr;
	}

	if (m_time != nullptr)
	{
		delete m_time;
		m_time = nullptr;
	}

	if (m_mainLayout != nullptr)
	{
		delete m_mainLayout;
		m_mainLayout = nullptr;
	}
}



void FriendItem::initLayout()
{
	// 
	QLabel * label = new QLabel(this);
	m_headImage = new RoundLabel(this);
	m_name = new QLabel(this);
	m_userId = new QLabel(this);
	m_msg = new QLabel(this);
	m_time = new QLabel(this);

	m_headImage->setFixedSize(50, 50);
	m_name->setFixedSize(150, 30);
	label->setFixedSize(100, 30);
	m_msg->setFixedSize(150, 30);
	m_time->setFixedSize(100, 30);

	m_userId->hide(); // 隐藏用户id

	// m_name 和 m_msg 垂直布局
	QVBoxLayout* nameMsgLayout = new QVBoxLayout();
	m_name->setContentsMargins(0, 0, 0, 0);
	m_msg->setContentsMargins(0, 0, 0, 0);
	nameMsgLayout->addWidget(m_name);
	nameMsgLayout->addWidget(m_msg);
	//nameMsgLayout->setContentsMargins(0, 0, 0, 0);
	//nameMsgLayout->setSpacing(0);

	// m_time 和 m_userId 垂直布局
	QVBoxLayout* timeUserIdLayout = new QVBoxLayout();
	timeUserIdLayout->addWidget(m_time);
	timeUserIdLayout->addWidget(label);
	timeUserIdLayout->setContentsMargins(0, 0, 0, 0);  // 设置边距
	timeUserIdLayout->setSpacing(0);  // 设置间距

	// m_headImage 和 nameMsgLayout 水平布局
	m_mainLayout = new QHBoxLayout();
	m_mainLayout->addWidget(m_headImage);
	m_mainLayout->addLayout(nameMsgLayout);
	m_mainLayout->addLayout(timeUserIdLayout);
	m_mainLayout->setContentsMargins(0, 0, 0, 0);
	m_mainLayout->setSpacing(0);

	setLayout(m_mainLayout);

}

void FriendItem::setFriendInfo(const QString& headImage, const QString& name, const QString& userId, const QString& msg, const QString& time)
{
	m_headImageStr = headImage;
	m_nameStr = name;
	m_userIdStr = userId;
	m_msgStr = msg;
	m_timeStr = time;

	m_headImage->setPixmap(QPixmap(headImage));
	m_name->setText(name);
	m_userId->setText(userId);
	m_msg->setText(msg);
	m_time->setText(time);
}

void FriendItem::setLabelStyle()
{
	this->setStyleSheet("background: none; border: none; color: black;");

	m_headImage->setStyleSheet("border-radius: 25px;");
	m_name->setStyleSheet("font-size: 22px; color: rgb(0, 0, 77);");
	m_msg->setStyleSheet("font-size: 18px; color: rgb(153, 153, 153);");
	m_time->setStyleSheet("font-size: 14px; color: rgb(153, 153, 153);");

}

void FriendItem::updateHeadImage(const QString& headImage)
{
	m_headImage->clear();
	m_headImage->setPixmap(QPixmap(headImage));

	m_headImageStr = headImage;
}

void FriendItem::updateName(const QString& name)
{
	m_name->setText(name);
	m_nameStr = name;
}

void FriendItem::updateUserId(const QString& userId)
{
	m_userId->setText(userId);
	m_userIdStr	= userId;
}

void FriendItem::updateMsg(const QString& msg)
{
	m_msg->setText(msg);
	m_msgStr = msg;
}

void FriendItem::updateTime(const QString& time)
{
	m_time->setText(time);
	m_timeStr = time;
}

QString FriendItem::getHeadImage() const
{
	if (m_headImageStr.isEmpty())
	{
		return QString();
	}

	return m_headImageStr;
}

QString FriendItem::getName() const
{
	if (m_nameStr.isEmpty())
		{
			return QString();
		}
	
	return m_nameStr;
}

QString FriendItem::getUserId() const
{
	if (m_userIdStr.isEmpty())
	{
		return QString();
	}

	return m_userIdStr;
}

QString FriendItem::getMsg() const
{
	if (m_msgStr.isEmpty())
	{
		return QString();
	}

	return m_msgStr;
}

QString FriendItem::getTime() const
{
	if (m_timeStr.isEmpty())
	{
		return QString();
	}

	return m_timeStr;
}

void FriendItem::setHeadImage(const QString& headImage)
{
	m_headImage->setPixmap(QPixmap(headImage));
	m_headImageStr = headImage;
}

void FriendItem::setName(const QString& name)
{
	m_name->setText(name);
	m_nameStr = name;
}

void FriendItem::setUserId(const QString& userId)
{
	m_userId->setText(userId);
	m_userIdStr = userId;
}

void FriendItem::setMsg(const QString& msg)
{
	m_msg->setText(msg);
	m_msgStr = msg;
}

void FriendItem::setTime(const QString& time)
{
	m_time->setText(time);
	m_timeStr = time;
}




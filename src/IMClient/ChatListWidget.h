#pragma once

#include <QWidget>
#include <QListWidget>

class SmoothScrollBar;

class ChatListWidget: public QListWidget
{
	Q_OBJECT

public:
	ChatListWidget(QWidget *parent = Q_NULLPTR);
	~ChatListWidget();

private:
	SmoothScrollBar *m_scrollBar;

};


#pragma once

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QEvent>
#include <QEnterEvent>
class MyIconButton : public QPushButton
{
	Q_OBJECT;
public:
	explicit MyIconButton(QWidget* parent);
	~MyIconButton();

public:
	void setIconEnter(const QIcon& icon);  // 设置鼠标进入图标

	void setIconLeave(const QIcon& icon);  // 设置鼠标离开图标
protected:

	bool event(QEvent* event) override;  // 事件过滤器
private:
	QIcon iconEnter_;  // 鼠标进入图标
	QIcon iconLeave_;  // 鼠标离开图标
};

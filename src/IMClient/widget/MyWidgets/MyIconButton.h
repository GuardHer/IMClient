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
	void setIconEnter(const QIcon& icon);  // ����������ͼ��

	void setIconLeave(const QIcon& icon);  // ��������뿪ͼ��
protected:

	bool event(QEvent* event) override;  // �¼�������
private:
	QIcon iconEnter_;  // ������ͼ��
	QIcon iconLeave_;  // ����뿪ͼ��
};

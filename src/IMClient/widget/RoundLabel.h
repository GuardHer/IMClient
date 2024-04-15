#pragma once

#include <QObject>
#include <QPixmap>
#include <QtWidgets/QLabel>

class RoundLabel :public QLabel
{
	Q_OBJECT
public:
	RoundLabel(QWidget* parent = 0);

	void setImage(const QPixmap& pixmap);

signals:
	void sigClick();
protected:
	void paintEvent(QPaintEvent* e);
	void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);
	void mousePressEvent(QMouseEvent* ev);
private:
	bool bTransLayer_; //Í¸Ã÷²ã

	QPixmap pixmap_{};

};

#include "Widget.h"

#include <QDebug>

#include "SQLiteManager.h"

Widget::Widget(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(800, 800);
	init();
}

Widget::~Widget()
{

}

void Widget::init()
{
	SQLiteManager* sqliteManager = new SQLiteManager();
	sqliteManager->Init("1291035260");

}

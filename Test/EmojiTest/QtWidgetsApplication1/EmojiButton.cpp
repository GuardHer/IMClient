#include "EmojiButton.h"

EmojiButton::EmojiButton(const EmojiCat& EmojiCat, const int flag, const bool isEmoji)
	: _cat{ &EmojiCat }, _name{ QString::fromStdString(EmojiCat.name()) }, _isEmoji{ isEmoji }, _flag{ flag }
{
	// ÉèÖÃÍ¼±ê
	initStyle();
	this->setIcon(QIcon(QString::fromStdString(EmojiCat.icon())));
	this->setIconSize(QSize(30, 30));

	this->setObjectName(QString::fromStdString(EmojiCat.name()));
	this->setToolTip(QString::fromStdString(EmojiCat.name()));

	connect(this, &QPushButton::clicked, this, &EmojiButton::handleButtonClicked);

}

EmojiButton::EmojiButton(const QString& name, const int flag, const bool isEmoji):
	_cat{ nullptr }, _name{ name }, _isEmoji{ isEmoji }, _flag{ flag }
{
	initStyle();
	if (_name == "right")
	{
		this->setIcon(QIcon(":/EmojiTest/res/right.png"));
	}
	else
	{
		this->setIcon(QIcon(":/EmojiTest/res/left.png"));
	}
	this->setIconSize(QSize(30, 30));
	this->setObjectName(_name);
	this->setToolTip(_name);

	connect(this, &QPushButton::clicked, this, &EmojiButton::handleButtonClicked);
}

EmojiButton::~EmojiButton()
{
	if (_cat)
	{
		delete _cat;
	}
}

void EmojiButton::initStyle()
{
	this->setFixedSize(48, 48);
	this->setStyleSheet("QPushButton {"
		"    background-color: rgb(255, 255, 255);"
		"    border-radius: 10px;"
		"}");
}

void EmojiButton::enterEvent(QEvent* event)
{
	this->setStyleSheet("QPushButton {"
		"    background-color: rgb(40, 193, 253);"
		"    border-radius: 10px;"
		"}");
	event->accept();
}

void EmojiButton::leaveEvent(QEvent* event)
{
	this->setStyleSheet("QPushButton {"
		"    background-color: rgb(255, 255, 255);"
		"    border-radius: 10px;"
		"}");
	event->accept();
}

void EmojiButton::handleButtonClicked()
{
	emit emojiClicked(*_cat);
}

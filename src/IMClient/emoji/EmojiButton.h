#pragma once

#include <QPushButton>
#include <QEvent>
#include "EmojiDB.hpp"

class EmojiButton : public QPushButton
{
	Q_OBJECT
public:
	explicit EmojiButton(const EmojiCat& EmojiCat, const int _flag, const bool isEmoji = true);  // explicit: ��ֹ��ʽת��
	EmojiButton(const QString& name, const int flag, const bool isEmoji = false);  // ���� 0���� 1����

	~EmojiButton();

	const EmojiCat& cat() const noexcept  // noexcept: ��֤���׳��쳣
	{
		return *_cat;
	}

public:
	const QString& name() const noexcept
	{
		return _name;
	}

	const bool isEmoji() const noexcept
	{
		return _isEmoji;
	}

	const int flag() const noexcept
	{
		return _flag;
	}

private:
	void initStyle();

signals:
	void emojiClicked(const EmojiCat& cat);

private slots:
	void handleButtonClicked();

protected:
	// �������¼�
	void enterEvent(QEvent* event) override;
	// ����뿪�¼�
	void leaveEvent(QEvent* event) override;

private:
	const EmojiCat *_cat;
	const QString _name;
	const bool _isEmoji;
	const int _flag;

};


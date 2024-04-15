#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EmojiTest.h"
#include "EmojiDB.hpp"
#include "EmojiImages.hpp"
#include "EmojiWindow.hpp"
#include "Settings.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class EmojiTestClass; };
QT_END_NAMESPACE

enum class Format
{
    UTF8,
    CODEPOINTS_HEX,
};

class EmojiTest : public QMainWindow
{
    Q_OBJECT

public:
    EmojiTest(QWidget *parent = nullptr);
    ~EmojiTest();

    void init();
    std::string formatEmoji(const Emoji& emoji,
        const std::optional<Emoji::SkinTone>& skinTone, const Format fmt,
        const std::string& cpPrefix, const bool noNl);

    char32_t * formatEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, 
        const Format fmt, const std::string& cpPrefix);

private slots:
    void onCanceled();  // 取消
    void onEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone);  // 表情符号选择

private:
    EmojiDb *_emojiDb;
    //QEmojisWidget *_emojisWidget;
    EmojiWindow* _emojiWindow;

private:
    Ui::EmojiTestClass *ui;
};

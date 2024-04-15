#include "EmojiTest.h"

// 布局
#include <QVBoxLayout>
#include <cstdio>
#include <cstdlib>
#include <array>

EmojiTest::EmojiTest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmojiTestClass())
{
    ui->setupUi(this);
    init();
}

EmojiTest::~EmojiTest()
{
    delete ui;

    if (_emojiDb != nullptr)
    {
		delete _emojiDb;
		_emojiDb = nullptr;
	}

    //if (_emojisWidget != nullptr)
	//{
	//    delete _emojisWidget;
	//    _emojisWidget = nullptr;
	//}

	if (_emojiWindow != nullptr)
	{
		delete _emojiWindow;
        _emojiWindow = nullptr;
	}
}

void EmojiTest::init()
{
    _emojiDb = new EmojiDb("E:\\StudyVideo\\IMClient\\Test\\EmojiTest\\QtWidgetsApplication1\\assets", EmojiDb::EmojiSize::SIZE_32);
	//_emojisWidget = new QEmojisWidget(this, *_emojiDb, false);
    _emojiWindow = new EmojiWindow(*_emojiDb, false);
	QObject::connect(_emojiWindow, &EmojiWindow::canceled, this, &EmojiTest::onCanceled);
	QObject::connect(_emojiWindow, &EmojiWindow::emojiChosen, this, &EmojiTest::onEmojiChosen);
    _emojiWindow->show();
}

/**
 * @function formatEmoji
 * @brief 格式化表情符号
 * @param emoji: 表情符号
 * @param skinTone: 皮肤色调
 * @param fmt: 格式
 * @param cpPrefix: 代码点前缀
 * @param noNl: 没有换行
 * @return : 返回格式化后的表情符号
 */
std::string EmojiTest::formatEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, 
    const Format fmt, const std::string& cpPrefix, const bool noNl)
{
    std::string output;
    int cnt = 0;

    switch (fmt) {
    case Format::UTF8:
    {
        if (skinTone && emoji.hasSkinToneSupport()) {
            output = emoji.strWithSkinTone(*skinTone);
        }
        else {
            output = emoji.str();
        }

        break;
    }

    case Format::CODEPOINTS_HEX:
    {
        Emoji::Codepoints codepoints;
        

        if (skinTone && emoji.hasSkinToneSupport()) {
            codepoints = emoji.codepointsWithSkinTone(*skinTone);
            std::cout << "ok" << std::endl;
        }
        else {
            codepoints = emoji.codepoints();
        }
        cnt = codepoints.size();
        
        for (const auto codepoint : codepoints) {
            std::array<char, 32> buf;

            std::sprintf(buf.data(), "%s%x ", cpPrefix.c_str(), codepoint);
            output += buf.data();
        }

        // remove trailing space
        output.resize(output.size() - 1);
        break;
    }
    }

    if (!noNl) {
        output += '\n';
    }

    if (cnt > 1)
    {
		// 将空格替换为-
        //std::replace(output.begin(), output.end(), ' ', '-');
	}
    std::cout << cnt << std::endl;

    return output;
}

char32_t* EmojiTest::formatEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, const Format fmt, const std::string& cpPrefix)
{
    char32_t* output = nullptr;

    Emoji::Codepoints codepoints;

    if (skinTone && emoji.hasSkinToneSupport()) {
        codepoints = emoji.codepointsWithSkinTone(*skinTone);
    }
    else {
        codepoints = emoji.codepoints();
    }

    for (const auto codepoint : codepoints) {
        std::array<char, 32> buf;

        std::sprintf(buf.data(), "%s%x ", cpPrefix.c_str(), codepoint);
        output += codepoint;
    }

    return output;
}



void EmojiTest::onEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone)
{
    const auto fmt = Format::CODEPOINTS_HEX;
	const auto cpPrefix = "";
	const auto output = formatEmoji(emoji, skinTone, fmt, cpPrefix, true);

    size_t pos;
    char32_t codepoint = std::stoul(output, &pos, 16);
    // 创建 char32_t 数组
    char32_t arr[] = { codepoint, 0x0 };

    std::u32string u32str;
    u32str.push_back(codepoint);

    //ui->plainTextEdit->textCursor().insertText(QString::fromUcs4(arr));
    ui->plainTextEdit->textCursor().insertText(QString::fromStdU32String(u32str));

    //std::wcout << arr << std::endl;
	printf("%s\n", output.c_str());

}

void EmojiTest::onCanceled()
{
    _emojiWindow->close();
	if (_emojiWindow != nullptr)
	{
		delete _emojiWindow;
        _emojiWindow = nullptr;
	}
}

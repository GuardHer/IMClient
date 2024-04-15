/*
 * Copyright (C) 2019 Philippe Proulx <eepp.ca>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QScrollBar>
#include <QListWidget>
#include <QLabel>
#include <sstream>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QPushButton>
#include <QBitmap>
//#include <boost/algorithm/string.hpp>

#include "EmojiWindow.hpp"
#include "EmojiPedia.hpp"



QSearchBoxEventFilter::QSearchBoxEventFilter(QObject * const parent) :
    QObject {parent}
{
}

/**
 * @function eventFilter
 * @brief 事件过滤器
 * @param obj: 
 * @param event: 
 * @return :
 */
bool QSearchBoxEventFilter::eventFilter(QObject * const obj, QEvent * const event)
{
    if (event->type() != QEvent::KeyPress) {  // 如果不是按键事件
        return QObject::eventFilter(obj, event);
    }

    const auto keyEvent = static_cast<const QKeyEvent *>(event);          // 转换为按键事件
    const bool withCtrl = keyEvent->modifiers() & Qt::ControlModifier; 

    switch (keyEvent->key()) { 
    case Qt::Key_Up:
        emit this->upKeyPressed();
        break;

    case Qt::Key_Right:
        emit this->rightKeyPressed(withCtrl);
        break;

    case Qt::Key_Down:
        emit this->downKeyPressed();
        break;

    case Qt::Key_Left:
        emit this->leftKeyPressed(withCtrl);
        break;

    case Qt::Key_F1:
        emit this->f1KeyPressed();
        break;

    case Qt::Key_F2:
        emit this->f2KeyPressed();
        break;

    case Qt::Key_F3:
        emit this->f3KeyPressed();
        break;

    case Qt::Key_F4:
        emit this->f4KeyPressed();
        break;

    case Qt::Key_F5:
        emit this->f5KeyPressed();
        break;

    case Qt::Key_F12:
        emit this->f12KeyPressed();
        break;

    case Qt::Key_PageUp:
        emit this->pgUpKeyPressed();
        break;

    case Qt::Key_PageDown:
        emit this->pgDownKeyPressed();
        break;

    case Qt::Key_Home:
        emit this->homeKeyPressed();
        break;

    case Qt::Key_End:
        emit this->endKeyPressed();
        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit this->enterKeyPressed();
        break;

    case Qt::Key_Escape:
        emit this->escapeKeyPressed();
        break;

    default:
        return QObject::eventFilter(obj, event);
    }

    return true;
}

//EmojiWindow::EmojiWindow(QWidget* parent, const EmojiDb& emojiDb, const bool darkBg) :
//    QWidget {parent}, _emojiDb {&emojiDb}
//{
//    this->setWindowIcon(QIcon {QString {R"(E:\StudyVideo\IMClient\src\IMClient\res\emoji\assets)"} + "/icon.png"});
//    this->setWindowTitle("Emoji");
//    this->resize(560, 420);
//    this->setMaximumSize(560, 420);
//    this->_setMainStyleSheet();
//    this->_buildUi(darkBg);
//
//    this->_initialized = true;
//}

EmojiWindow::EmojiWindow() :
    _emojiDb{ nullptr }
{
}

void EmojiWindow::init(const EmojiDb& emojiDb, bool darkBg)
{
    if (!_initialized) {
		_emojiDb = &emojiDb;
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); 
        this->setAttribute(Qt::WA_TranslucentBackground); // 设置透明背景
        this->setAttribute(Qt::WA_DeleteOnClose);  // 关闭时删除
		this->setWindowIcon(QIcon {QString {R"(E:\StudyVideo\IMClient\src\IMClient\res\emoji\assets)"} + "/icon.png"});
		this->setWindowTitle("Emoji");
		this->resize(560, 420);
		this->setMaximumSize(560, 420);
		this->_setMainStyleSheet();
		this->_buildUi(darkBg);
		this->_initialized = true;
	}
}

EmojiWindow::~EmojiWindow()
{
    // 删除表情符号按钮
    for (auto& btns : _emojiBtns) {
        for (auto btn : btns) {
			delete btn;
		}
	}

    // 删除表情符号窗口部件
	delete _wEmojis;

    // 删除搜索框
    delete _wSearchBox;

    // 删除信息标签
    delete _wInfoLabel;

    // 删除版本标签
    delete _wVersionLabel;

    // 删除按钮窗口部件
    delete _wBtns;

    // 删除选择的表情符号
    delete _selectedEmoji;

    // 删除表情符号数据库
    delete _emojiDb;

}

void EmojiWindow::_setMainStyleSheet()
{
    static const char * const styleSheet =
        "* {"
        "  font-family: 'Hack', 'DejaVu Sans Mono', monospace;"
        "  font-size: 12px;"
        "  border: none;"
        "}"
        "QWidget {"
        "  background-color: #f8f8f8;"
        "}"
        "QLineEdit {"
        "  background-color: rgba(255, 255, 255, 0.2);"
        "  color: #f7fbfe;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "  border-bottom: 1px solid #d5d6d7;"
        "  padding: 4px;"
        "}"
        "QScrollBar:vertical {"        // 垂直滚动条
        "  border: none;"
        "  background-color: #f8f8f8;"
        "  width: 10px;"
        "  margin: 0;"
        "}"
        "QScrollBar::handle:vertical {"  // 垂直滚动条手柄
        "  border: none;"
        "  background-color: #d5d6d7;"
        "  min-height: 72px;"
        "  border-radius: 5px;"
        "}"
        "QScrollBar::add-line:vertical,"
        "QScrollBar::sub-line:vertical {"    
        "  height: 0;"
        "}"
        "QLabel {"
        "  color: #ff3366;"
        "}";

    this->setStyleSheet(styleSheet);
}


QWidget* EmojiWindow::_createBtnWidget()
{
    auto btnWidget = new QWidget;
    // 设置固定大小 560 * 60
    btnWidget->setFixedSize(560, 60);
    // 设置背景色
    btnWidget->setStyleSheet("background-color: rgb(255, 255, 255);");

    auto btnLayout = new QHBoxLayout; // 水平布局
    // 设置左右间距为19.5
    btnLayout->setSpacing(19.5);
    btnLayout->setMargin(0);  // 设置边距
    // 设置居中
    btnLayout->setAlignment(Qt::AlignCenter);

    int flag = -1;
    for (int i = 1; i < _emojiDb->cats().size(); i += 6) {
        flag += 1;
        std::vector<EmojiButton*> currentBtns;
        auto leftBtn = new EmojiButton("left", flag);
        auto rightBtn = new EmojiButton("right", flag);
        QObject::connect(leftBtn, &EmojiButton::clicked, this, &EmojiWindow::_leftOrRightBtnClicked);
        QObject::connect(rightBtn, &EmojiButton::clicked, this, &EmojiWindow::_leftOrRightBtnClicked);
        currentBtns.push_back(leftBtn);
        for (int j = i; j < i + 6 && j < _emojiDb->cats().size(); ++j) {
            auto btn = new EmojiButton(*_emojiDb->cats()[j], flag);
            QObject::connect(btn, &EmojiButton::emojiClicked, this, &EmojiWindow::_emojiButtonClicked);
            currentBtns.push_back(btn);
        }
        currentBtns.push_back(rightBtn);
        _emojiBtns.push_back(currentBtns);
    }

    for (int i = 0; i < _emojiBtns.size(); ++i) {
        for (int j = 0; j < _emojiBtns[i].size(); ++j) {
            if (i != 0) _emojiBtns[i][j]->hide();
			btnLayout->addWidget(_emojiBtns[i][j]);
		}
	}

	btnWidget->setLayout(btnLayout);
	return btnWidget;
}

void EmojiWindow::_buildUi(const bool darkBg)
{
    // 取消窗口边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    _wSearchBox = new QLineEdit;
    QObject::connect(_wSearchBox, &QLineEdit::textChanged,
                     this, &EmojiWindow::_searchTextChanged);

    auto eventFilter = new QSearchBoxEventFilter {this};  // 事件过滤器

    _wSearchBox->installEventFilter(eventFilter);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::upKeyPressed, this,
                     &EmojiWindow::_searchBoxUpKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::rightKeyPressed, this,
                     &EmojiWindow::_searchBoxRightKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::downKeyPressed, this,
                     &EmojiWindow::_searchBoxDownKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::leftKeyPressed, this,
                     &EmojiWindow::_searchBoxLeftKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::enterKeyPressed, this,
                     &EmojiWindow::_searchBoxEnterKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::f1KeyPressed, this,
                     &EmojiWindow::_searchBoxF1KeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::f2KeyPressed, this,
                     &EmojiWindow::_searchBoxF2KeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::f3KeyPressed, this,
                     &EmojiWindow::_searchBoxF3KeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::f4KeyPressed, this,
                     &EmojiWindow::_searchBoxF4KeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::f5KeyPressed, this,
                     &EmojiWindow::_searchBoxF5KeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::f12KeyPressed, this,
                     &EmojiWindow::_searchBoxF12KeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::pgUpKeyPressed, this,
                     &EmojiWindow::_searchBoxPgUpKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::pgDownKeyPressed, this,
                     &EmojiWindow::_searchBoxPgDownKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::homeKeyPressed, this,
                     &EmojiWindow::_searchBoxHomeKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::endKeyPressed, this,
                     &EmojiWindow::_searchBoxEndKeyPressed);
    QObject::connect(eventFilter, &QSearchBoxEventFilter::escapeKeyPressed, this,
                     &EmojiWindow::_searchBoxEscapeKeyPressed);

    auto mainVbox = new QVBoxLayout;

    mainVbox->setMargin(0);  // 设置边距
    mainVbox->setSpacing(0); // 设置间距
    mainVbox->addWidget(_wSearchBox);

    _wEmojis = new QEmojisWidget {nullptr, *_emojiDb, darkBg};

    QObject::connect(_wEmojis, &QEmojisWidget::selectionChanged, this, &EmojiWindow::_emojiSelectionChanged); // 信号: 选择改变
    QObject::connect(_wEmojis, &QEmojisWidget::emojiClicked, this, &EmojiWindow::_emojiClicked);              // 信号: 表情符号点击
    QObject::connect(_wEmojis, &QEmojisWidget::emojiHoverEntered, this, &EmojiWindow::_emojiHoverEntered);    // 信号: 表情符号悬停进入
    QObject::connect(_wEmojis, &QEmojisWidget::emojiHoverLeaved, this,  &EmojiWindow::_emojiHoverLeaved);     // 信号: 表情符号悬停离开


    mainVbox->addWidget(_wEmojis);  // 创建标签

    _wBtns = this->_createBtnWidget();
    mainVbox->addWidget(_wBtns);

    const auto centralWidget = new QWidget;

    //centralWidget->setLayout(mainVbox);
    //this->setCentralWidget(centralWidget);  // 设置中心部件
    this->setLayout(mainVbox);

    const auto infoHbox = new QHBoxLayout;  // 水平布局
    _wInfoLabel = new QLabel {""};     // 创建标签
    infoHbox->addWidget(_wInfoLabel);
    _wVersionLabel = new QLabel {""};  // 创建标签 
    _wVersionLabel->setFixedWidth(80);
    _wVersionLabel->setAlignment(Qt::AlignRight);
    infoHbox->addWidget(_wVersionLabel);
    mainVbox->addLayout(infoHbox);

}

/**
 * @function showEvent
 * @brief 显示事件
 * @param event:
 */
void EmojiWindow::showEvent(QShowEvent * const event)
{
    QWidget::showEvent(event);  // 调用父类的方法

    if (!_emojisWidgetBuilt) {  // 如果表情符号窗口部件未构建
        _wEmojis->rebuild();    // 重建
        _emojisWidgetBuilt = true;
    }

    _wEmojis->showAllEmojis();   // 显示所有表情符号
    _wSearchBox->clear();        // 清空搜索框
    _wSearchBox->setFocus();     // 设置焦点
}

void EmojiWindow::closeEvent(QCloseEvent * const event)
{
    event->ignore();  // 忽略
    this->close(); // 隐藏
    emit this->canceled();  // 发送信号: 取消
}

void EmojiWindow::_findEmojis(const std::string& cat, const std::string& needles)
{
    std::vector<const Emoji *> results;

    _emojiDb->findEmojis(cat, needles, results);
    _wEmojis->showFindResults(results);
}

void split(const std::string& input, std::vector<std::string>& parts, char delimiter) {
    std::istringstream stream(input);
    std::string part;

    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }
}

/***************************************  slot  *********************************************/
/**
 * @function _searchTextChanged
 * @brief 查找文本改变
 * @param text:
 */
void EmojiWindow::_searchTextChanged(const QString& text)
{
    if (text.isEmpty()) {
        _wEmojis->showAllEmojis();
        return;
    }

    std::vector<std::string> parts;
    const std::string textStr {text.toUtf8().constData()};

    //boost::split(parts, textStr, boost::is_any_of("/"));
    split(textStr, parts, '/');

    if (parts.size() != 2) {
        this->_findEmojis("", textStr);
        return;
    }

    this->_findEmojis(parts[0], parts[1]);
}

/**
 * @function _searchBoxUpKeyPressed
 * @brief 查找框上键按下
 */
void EmojiWindow::_searchBoxUpKeyPressed()
{
    _wEmojis->selectPreviousRow();
}

/**
 * @function _searchBoxRightKeyPressed
 * @brief 查找框右键按下
 * @param withCtrl:
 */
void EmojiWindow::_searchBoxRightKeyPressed(const bool withCtrl)
{
    _wEmojis->selectNext(withCtrl ? 5 : 1);
}

/**
 * @function _searchBoxDownKeyPressed
 * @brief 查找框下键按下
 */
void EmojiWindow::_searchBoxDownKeyPressed()
{
    _wEmojis->selectNextRow();
}

/**
 * @function _searchBoxLeftKeyPressed
 * @brief 查找框左键按下
 * @param withCtrl: 
 */ 
void EmojiWindow::_searchBoxLeftKeyPressed(const bool withCtrl)
{
    _wEmojis->selectPrevious(withCtrl ? 5 : 1);
}

void EmojiWindow::_searchBoxPgUpKeyPressed()
{
    _wEmojis->selectPreviousRow(10);
}

void EmojiWindow::_searchBoxPgDownKeyPressed()
{
    _wEmojis->selectNextRow(10);
}

void EmojiWindow::_searchBoxHomeKeyPressed()
{
    _wEmojis->selectFirst();
}

void EmojiWindow::_searchBoxEndKeyPressed()
{
    _wEmojis->selectLast();
}

/**
 * @function _searchBoxEscapeKeyPressed
 * @brief 查找框 Esc 按下
 */
void EmojiWindow::_searchBoxEscapeKeyPressed()
{
    this->hide();
    emit this->canceled();
}

void EmojiWindow::_searchBoxEnterKeyPressed()
{
    this->_acceptSelectedEmoji(std::nullopt);
}

void EmojiWindow::_searchBoxF1KeyPressed()
{
    this->_acceptSelectedEmoji(Emoji::SkinTone::LIGHT);
}

void EmojiWindow::_searchBoxF2KeyPressed()
{
    this->_acceptSelectedEmoji(Emoji::SkinTone::MEDIUM_LIGHT);
}

void EmojiWindow::_searchBoxF3KeyPressed()
{
    this->_acceptSelectedEmoji(Emoji::SkinTone::MEDIUM);
}

void EmojiWindow::_searchBoxF4KeyPressed()
{
    this->_acceptSelectedEmoji(Emoji::SkinTone::MEDIUM_DARK);
}

void EmojiWindow::_searchBoxF5KeyPressed()
{
    this->_acceptSelectedEmoji(Emoji::SkinTone::DARK);
}

void EmojiWindow::_searchBoxF12KeyPressed()
{
    this->_requestSelectedEmojiInfo();
}

/**
 * @function _emojiSelectionChanged
 * @brief 表情符号选择改变
 * @param emoji:
 */
void EmojiWindow::_emojiSelectionChanged(const Emoji * const emoji)
{
    _selectedEmoji = emoji;
    this->_updateBottomLabels(emoji);  // 更新底部标签
}

/**
 * @function _emojiClicked
 * @brief 表情符号点击
 * @param emoji:
 */
void EmojiWindow::_emojiClicked(const Emoji& emoji)
{
    this->_acceptEmoji(emoji, std::nullopt);
}

/**
 * @function _emojiHoverEntered
 * @brief 表情符号悬停进入
 * @param emoji:
 */
void EmojiWindow::_emojiHoverEntered(const Emoji& emoji)
{
    this->_updateBottomLabels(&emoji);
}

/**
 * @function _emojiHoverLeaved
 * @brief 表情符号悬停离开
 * @param :
 */
void EmojiWindow::_emojiHoverLeaved(const Emoji&)
{
    this->_updateBottomLabels(_selectedEmoji);
}

/**
 * @function _emojiButtonClicked
 * @brief 表情符号按钮点击, 跳转到指定的表情符号
 * @param cat: 
 */
void EmojiWindow::_emojiButtonClicked(const EmojiCat& cat)
{
	_wEmojis->scrollToCat(cat);
}

/**
 * @function _leftOrRightBtnClicked
 * @brief 左右按钮点击
 */
void EmojiWindow::_leftOrRightBtnClicked()
{
    EmojiButton* senderButton = qobject_cast<EmojiButton*>(QObject::sender());
    if (senderButton) {
        QString buttonObjectName = senderButton->objectName();
        if (buttonObjectName == "left") {
			if (!senderButton->isEmoji() && senderButton->flag() == 0) return;
            this->_setCurrentButtons(senderButton->flag() - 1);
        }
        else if (buttonObjectName == "right") {
            if (!senderButton->isEmoji() && senderButton->flag() == this->_emojiBtns.size() - 1) return;
            this->_setCurrentButtons(senderButton->flag() + 1);
        }
    }

}

/**
 * @function _acceptSelectedEmoji
 * @brief 接受选择的表情符号
 * @param skinTone:
 */
void EmojiWindow::_acceptSelectedEmoji(const std::optional<Emoji::SkinTone>& skinTone)
{
    if (_selectedEmoji) {
        this->_acceptEmoji(*_selectedEmoji, skinTone);
    }
}

/**
 * @function _acceptEmoji
 * @brief 接受表情符号
 * @param emoji: 
 * @param skinTone:
 */
void EmojiWindow::_acceptEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone)
{
    // 检查是否指定了肤色，并且 Emoji 支持肤色变体
    if (skinTone && !emoji.hasSkinToneSupport()) {
        // 如果指定了肤色但 Emoji 不支持肤色变体，直接返回
        return;
    }

    // 发送信号通知 Emoji 被选择
    emit this->emojiChosen(emoji, skinTone);
}

/**
 * @function _requestSelectedEmojiInfo
 * @brief 请求选择的表情符号信息
 */
void EmojiWindow::_requestSelectedEmojiInfo()
{
    if (_selectedEmoji) {
        this->_requestEmojiInfo(*_selectedEmoji);
    }
}

/**
 * @function _requestEmojiInfo
 * @brief 请求表情符号信息
 * @param emoji:
 */
void EmojiWindow::_requestEmojiInfo(const Emoji& emoji)
{
    gotoEmojipediaPage(emoji);
}

/**
 * @function _setCurrentButtons
 * @brief 设置当前按钮
 * @param index: 
 */
void EmojiWindow::_setCurrentButtons(const int index)
{
    for (int i = 0; i < _emojiBtns.size(); ++i) {
        for (int j = 0; j < _emojiBtns[i].size(); ++j) {
            if (i == index) {
				_emojiBtns[i][j]->show();
			}
            else {
				_emojiBtns[i][j]->hide();
			}
		}
	}
}


/**
 * @function paintEvent
 * @brief 绘制事件
 * @param event:
 */
void EmojiWindow::paintEvent(QPaintEvent* event)
{
    //绘制样式
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//绘制样式

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter painter(&bmp);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    //设置边框为圆角12px
    painter.drawRoundedRect(bmp.rect(), 12, 12);
    setMask(bmp);
    QWidget::paintEvent(event);
}

/**
 * @function event
 * @brief 事件
 * @param event:
 * @return :
 */
bool EmojiWindow::event(QEvent* event)
{
    /// 判断是否是活动窗口，如果不活动窗口，隐藏窗口
    if (event->type() == QEvent::ActivationChange)
    {
        if (!this->isActiveWindow()) // 如果窗口不是活动窗口
        {
			this->hide();
			// emit this->canceled();
		}
    }

    return QWidget::event(event);
}


/**
 * @function _updateBottomLabels
 * @brief 更新底部标签
 * @param emoji:
 */
void EmojiWindow::_updateBottomLabels(const Emoji * const emoji)
{
    this->_updateInfoLabel(emoji);
    this->_updateVersionLabel(emoji);
}

/**
 * @function _updateInfoLabel
 * @brief 更新信息标签
 * @param emoji:
 */
void EmojiWindow::_updateInfoLabel(const Emoji * const emoji)
{
    QString text;

    if (emoji) {
        text += "<b>";
        text += emoji->name().c_str();
        text += "</b> <span style=\"color: #999\">(";

        for (const auto codepoint : emoji->codepoints()) {
            text += QString::number(codepoint, 16) + ", ";
        }

        text.truncate(text.size() - 2);
        text += ")</span>";
    }

    _wInfoLabel->setText(text);
}

/**
 * @function emojiVersionStr
 * @brief 表情符号版本字符串
 * @param version:
 * @return :
 */
QString emojiVersionStr(const EmojiVersion version)
{
    switch (version) {
    case EmojiVersion::V_0_6:
        return "0.6";

    case EmojiVersion::V_0_7:
        return "0.7";

    case EmojiVersion::V_1_0:
        return "1.0";

    case EmojiVersion::V_2_0:
        return "2.0";

    case EmojiVersion::V_3_0:
        return "3.0";

    case EmojiVersion::V_4_0:
        return "4.0";

    case EmojiVersion::V_5_0:
        return "5.0";

    case EmojiVersion::V_11_0:
        return "11.0";

    case EmojiVersion::V_12_0:
        return "12.0";

    case EmojiVersion::V_12_1:
        return "12.1";

    case EmojiVersion::V_13_0:
        return "13.0";

    case EmojiVersion::V_13_1:
        return "13.1";

    case EmojiVersion::V_14_0:
        return "14.0";

    default:
        std::abort();
    }
}

/**
 * @function _updateVersionLabel
 * @brief 更新版本标签
 * @param emoji:
 */
void EmojiWindow::_updateVersionLabel(const Emoji * const emoji)
{
    QString text;

    if (emoji) {
        text += "<span style=\"color: #2ecc71\">Emoji <b>";
        text += emojiVersionStr(emoji->version());
        text += "</b></span>";
    }

    _wVersionLabel->setText(text);
}

/**
 * @function emojiDbChanged
 * @brief 表情符号数据库改变
 */
void EmojiWindow::emojiDbChanged()
{
    _wEmojis->rebuild();
    _wEmojis->showAllEmojis();
}


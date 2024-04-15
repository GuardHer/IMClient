#pragma once

#include <QObject>
#include <QEvent>
#include <QLabel>
#include <QListWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <boost/optional.hpp>
#include <functional>
#include <optional>
#include <vector>

#include "EmojiDB.hpp"
#include "EmojiImages.hpp"
#include "EmojisGraphicsWidget.hpp"
#include "EmojiButton.h"

class QSearchBoxEventFilter :
    public QObject
{
    Q_OBJECT

public:
    explicit QSearchBoxEventFilter(QObject *parent);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:  // 信号 和QJomeWindow类中的槽函数相连
    void upKeyPressed();                   // 上键按下
    void rightKeyPressed(bool withCtrl);   // 右键按下
    void downKeyPressed();                 // 下键按下
    void leftKeyPressed(bool withCtrl);    // 左键按下
    void enterKeyPressed();                // 回车键按下
    void f1KeyPressed();                   // F1键按下
    void f2KeyPressed();                   // F2键按下
    void f3KeyPressed();                   // F3键按下
    void f4KeyPressed();                   // F4键按下
    void f5KeyPressed();                   // F5键按下
    void f12KeyPressed();                  // F12键按下
    void pgUpKeyPressed();                 // PgUp键按下
    void pgDownKeyPressed();               // PgDown键按下
    void homeKeyPressed();                 // Home键按下
    void endKeyPressed();                  // End键按下
    void escapeKeyPressed();               // Esc键按下
};

class EmojiWindow : public QWidget
{
    Q_OBJECT

public:

    //explicit EmojiWindow(QWidget* parent, const EmojiDb& emojiDb, bool darkBg);
    explicit EmojiWindow();
    void init(const EmojiDb& emojiDb, bool darkBg);

    ~EmojiWindow();

signals:
    void emojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone); // 表情符号选择
    void canceled();         // 取消
     
public slots:
    void emojiDbChanged();     // 表情符号数据库改变

private:
    void closeEvent(QCloseEvent *event) override;  // 关闭事件
    void showEvent(QShowEvent *event) override;    // 显示事件
    void _setMainStyleSheet();                     // 设置主样式表
    void _buildUi(bool darkBg);                    // 构建用户界面
    QWidget *_createBtnWidget();                   // 创建按钮窗口部件
    void _updateBottomLabels(const Emoji *emoji);  // 更新底部标签
    void _updateInfoLabel(const Emoji *emoji);     // 更新信息标签
    void _updateVersionLabel(const Emoji *emoji);  // 更新版本标签
    void _findEmojis(const std::string& cat, const std::string& needles);                   // 查找表情符号
    void _acceptSelectedEmoji(const std::optional<Emoji::SkinTone>& skinTone);              // 接受选择的表情符号
    void _acceptEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone);  // 接受表情符号
    void _requestSelectedEmojiInfo();                                                       // 请求选择的表情符号信息
    void _requestEmojiInfo(const Emoji& emoji);                                             // 请求表情符号信息

    void _setCurrentButtons(const int index);                                           // 设置当前按钮

protected:
    // 绘制
    void paintEvent(QPaintEvent *event) override;

    bool event(QEvent* event) override;  // 事件

private slots:
    void _searchTextChanged(const QString& text);       // 查找文本改变
    void _searchBoxUpKeyPressed(); 					    // 查找框上键按下
    void _searchBoxRightKeyPressed(bool withCtrl);      // 查找框右键按下
    void _searchBoxDownKeyPressed();
    void _searchBoxLeftKeyPressed(bool withCtrl);
    void _searchBoxEnterKeyPressed();
    void _searchBoxF1KeyPressed();
    void _searchBoxF2KeyPressed();
    void _searchBoxF3KeyPressed();
    void _searchBoxF4KeyPressed();
    void _searchBoxF5KeyPressed();
    void _searchBoxF12KeyPressed();
    void _searchBoxPgUpKeyPressed();
    void _searchBoxPgDownKeyPressed();
    void _searchBoxHomeKeyPressed();
    void _searchBoxEndKeyPressed();
    void _searchBoxEscapeKeyPressed();
    void _emojiSelectionChanged(const Emoji *emoji);       // 表情符号选择改变
    void _emojiClicked(const Emoji& emoji);                // 表情符号点击
    void _emojiHoverEntered(const Emoji& emoji);           // 表情符号悬停进入
    void _emojiHoverLeaved(const Emoji& emoji);            // 表情符号悬停离开

    void _emojiButtonClicked(const EmojiCat& cat);         // 表情符号按钮点击
    void _leftOrRightBtnClicked();                         // 左右按钮点击

private:
    const EmojiDb * _emojiDb;           // 表情符号数据库
    QEmojisWidget *_wEmojis = nullptr;        // 表情符号窗口部件
    QLabel *_wInfoLabel = nullptr;            // 信息标签
    QLabel *_wVersionLabel = nullptr;         // 版本标签
    QLineEdit *_wSearchBox = nullptr;         // 查找框
    bool _emojisWidgetBuilt = false;          // 表情符号窗口部件是否已构建
    const Emoji *_selectedEmoji = nullptr;    // 选择的表情符号

    QWidget *_wBtns = nullptr;                // 按钮窗口部件
    std::vector<std::vector<EmojiButton*>> _emojiBtns;    // 按钮

    bool _initialized = false;                // 初始化

};


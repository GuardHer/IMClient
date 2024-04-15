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

signals:  // �ź� ��QJomeWindow���еĲۺ�������
    void upKeyPressed();                   // �ϼ�����
    void rightKeyPressed(bool withCtrl);   // �Ҽ�����
    void downKeyPressed();                 // �¼�����
    void leftKeyPressed(bool withCtrl);    // �������
    void enterKeyPressed();                // �س�������
    void f1KeyPressed();                   // F1������
    void f2KeyPressed();                   // F2������
    void f3KeyPressed();                   // F3������
    void f4KeyPressed();                   // F4������
    void f5KeyPressed();                   // F5������
    void f12KeyPressed();                  // F12������
    void pgUpKeyPressed();                 // PgUp������
    void pgDownKeyPressed();               // PgDown������
    void homeKeyPressed();                 // Home������
    void endKeyPressed();                  // End������
    void escapeKeyPressed();               // Esc������
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
    void emojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone); // �������ѡ��
    void canceled();         // ȡ��
     
public slots:
    void emojiDbChanged();     // ����������ݿ�ı�

private:
    void closeEvent(QCloseEvent *event) override;  // �ر��¼�
    void showEvent(QShowEvent *event) override;    // ��ʾ�¼�
    void _setMainStyleSheet();                     // ��������ʽ��
    void _buildUi(bool darkBg);                    // �����û�����
    QWidget *_createBtnWidget();                   // ������ť���ڲ���
    void _updateBottomLabels(const Emoji *emoji);  // ���µײ���ǩ
    void _updateInfoLabel(const Emoji *emoji);     // ������Ϣ��ǩ
    void _updateVersionLabel(const Emoji *emoji);  // ���°汾��ǩ
    void _findEmojis(const std::string& cat, const std::string& needles);                   // ���ұ������
    void _acceptSelectedEmoji(const std::optional<Emoji::SkinTone>& skinTone);              // ����ѡ��ı������
    void _acceptEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone);  // ���ܱ������
    void _requestSelectedEmojiInfo();                                                       // ����ѡ��ı��������Ϣ
    void _requestEmojiInfo(const Emoji& emoji);                                             // ������������Ϣ

    void _setCurrentButtons(const int index);                                           // ���õ�ǰ��ť

protected:
    // ����
    void paintEvent(QPaintEvent *event) override;

    bool event(QEvent* event) override;  // �¼�

private slots:
    void _searchTextChanged(const QString& text);       // �����ı��ı�
    void _searchBoxUpKeyPressed(); 					    // ���ҿ��ϼ�����
    void _searchBoxRightKeyPressed(bool withCtrl);      // ���ҿ��Ҽ�����
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
    void _emojiSelectionChanged(const Emoji *emoji);       // �������ѡ��ı�
    void _emojiClicked(const Emoji& emoji);                // ������ŵ��
    void _emojiHoverEntered(const Emoji& emoji);           // ���������ͣ����
    void _emojiHoverLeaved(const Emoji& emoji);            // ���������ͣ�뿪

    void _emojiButtonClicked(const EmojiCat& cat);         // ������Ű�ť���
    void _leftOrRightBtnClicked();                         // ���Ұ�ť���

private:
    const EmojiDb * _emojiDb;           // ����������ݿ�
    QEmojisWidget *_wEmojis = nullptr;        // ������Ŵ��ڲ���
    QLabel *_wInfoLabel = nullptr;            // ��Ϣ��ǩ
    QLabel *_wVersionLabel = nullptr;         // �汾��ǩ
    QLineEdit *_wSearchBox = nullptr;         // ���ҿ�
    bool _emojisWidgetBuilt = false;          // ������Ŵ��ڲ����Ƿ��ѹ���
    const Emoji *_selectedEmoji = nullptr;    // ѡ��ı������

    QWidget *_wBtns = nullptr;                // ��ť���ڲ���
    std::vector<std::vector<EmojiButton*>> _emojiBtns;    // ��ť

    bool _initialized = false;                // ��ʼ��

};


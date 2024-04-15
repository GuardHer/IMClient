#pragma once
#include <QPlainTextEdit>

enum class CustomKey {
    CK_EnterSend,      // ���Իس���������Ϣ
    CK_CRLF            // carriage return/line feed �س�����
};

class ChatInputEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit ChatInputEdit(QWidget* parent = nullptr);

protected:
    void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void sigCustomKeyBtnReleased(const CustomKey& customKey);
};


#include "ChatInput.h"


ChatInputEdit::ChatInputEdit(QWidget* parent)
    : QPlainTextEdit(parent)
{
}

void ChatInputEdit::keyReleaseEvent(QKeyEvent* event)
{

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        event->ignore(); // ���Ϊδ�������������¼���������¼�������

        if (event->modifiers() == Qt::ShiftModifier || event->modifiers() == Qt::ControlModifier) { // ���µ��� Shift/Ctrl + Enter
            emit sigCustomKeyBtnReleased(CustomKey::CK_CRLF);

            if (event->modifiers() == Qt::ShiftModifier) {
                                // shift + enter ������������
            }
            else if (event->modifiers() == Qt::ControlModifier) {
                insertPlainText("\n");  // ����֣�appendPlainText �򲻿�����������
            }
        }
        else { // �� Enter
            emit sigCustomKeyBtnReleased(CustomKey::CK_EnterSend);
            return;
        }
    }

    QPlainTextEdit::keyPressEvent(event);
}

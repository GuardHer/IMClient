#include "MyIconButton.h"

MyIconButton::MyIconButton(QWidget *parent)
	: QPushButton(parent)
{}

MyIconButton::~MyIconButton()
{}

void MyIconButton::setIconEnter(const QIcon& icon)  // ����������ͼ��
{
	iconEnter_ = icon;
}

void MyIconButton::setIconLeave(const QIcon& icon)  // ��������뿪ͼ�� 
{
	iconLeave_ = icon;
}


bool MyIconButton::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::Enter:
        setIcon(iconEnter_);
        break;
    case QEvent::Leave:
        setIcon(iconLeave_);
        break;
    case QEvent::MouseButtonPress:   // ��갴��
        setIcon(iconEnter_);
		break;
    case QEvent::MouseButtonRelease:  // ����ͷ�
        setIcon(iconLeave_);
        break;
  //  case QEvent::MouseButtonDblClick:  // ���˫��
		//break;
  //  case QEvent::MouseMove:  // ����ƶ�
  //      break;
  //  case QEvent::KeyPress:  // ���̰���
		//break;
    default:
        break;
    }
    return QPushButton::event(event);
}

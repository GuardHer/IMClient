#include "MyIconButton.h"

MyIconButton::MyIconButton(QWidget *parent)
	: QPushButton(parent)
{}

MyIconButton::~MyIconButton()
{}

void MyIconButton::setIconEnter(const QIcon& icon)  // 设置鼠标进入图标
{
	iconEnter_ = icon;
}

void MyIconButton::setIconLeave(const QIcon& icon)  // 设置鼠标离开图标 
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
    case QEvent::MouseButtonPress:   // 鼠标按下
        setIcon(iconEnter_);
		break;
    case QEvent::MouseButtonRelease:  // 鼠标释放
        setIcon(iconLeave_);
        break;
  //  case QEvent::MouseButtonDblClick:  // 鼠标双击
		//break;
  //  case QEvent::MouseMove:  // 鼠标移动
  //      break;
  //  case QEvent::KeyPress:  // 键盘按下
		//break;
    default:
        break;
    }
    return QPushButton::event(event);
}

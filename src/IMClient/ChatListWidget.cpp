#include "ChatListWidget.h"

#include "SmoothScrollBar.h"

ChatListWidget::ChatListWidget(QWidget* parent)
{
	m_scrollBar = new SmoothScrollBar();
	this->setVerticalScrollBar(m_scrollBar);
	setVerticalScrollMode(QListWidget::ScrollPerPixel);

	// 禁用水平滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// 设置listWidgetitem无法被选中
	setSelectionMode(QAbstractItemView::NoSelection);
	setFocusPolicy(Qt::NoFocus); // 设置焦点策略为无焦点


}

ChatListWidget::~ChatListWidget()
{
	delete m_scrollBar;
}

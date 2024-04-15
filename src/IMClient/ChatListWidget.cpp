#include "ChatListWidget.h"

#include "SmoothScrollBar.h"

ChatListWidget::ChatListWidget(QWidget* parent)
{
	m_scrollBar = new SmoothScrollBar();
	this->setVerticalScrollBar(m_scrollBar);
	setVerticalScrollMode(QListWidget::ScrollPerPixel);

	// ����ˮƽ������
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// ����listWidgetitem�޷���ѡ��
	setSelectionMode(QAbstractItemView::NoSelection);
	setFocusPolicy(Qt::NoFocus); // ���ý������Ϊ�޽���


}

ChatListWidget::~ChatListWidget()
{
	delete m_scrollBar;
}

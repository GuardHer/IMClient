#include "RoundLabel.h"

#include <QPainter>
#include <QPainterPath>

RoundLabel::RoundLabel(QWidget* parent) :QLabel(parent)
{
	bTransLayer_ = false;  // Ĭ�ϲ�͸��
}
void RoundLabel::setImage(const QPixmap& pixmap)
{
	pixmap_ = pixmap;
}

void RoundLabel::paintEvent(QPaintEvent* e)
{
	if (NULL != pixmap())
	{
		QPainter painter(this);  // ����QPainterһ������
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);  // �����
		QPainterPath path;  // ����һ��·������
		int round = qMin(width(), height());  // ȡ��Сֵ
		path.addEllipse(0, 0, round, round);  // ����Բ��
		painter.setClipPath(path);           // ���û���·��
		painter.drawPixmap(-1, -1, width() + 2, height() + 2, *pixmap());   // ����ͼƬ
		if (bTransLayer_)   
		{
			//�����label�ϣ�����͸��ͼƬ
			if (pixmap_.isNull())
			{
				QPixmap pixMap(";/res/head/head1.jpg");
				painter.drawPixmap(-1, -1, width() + 2, height() + 2, pixMap);
			}
			else
			{
				painter.drawPixmap(-1, -1, width() + 2, height() + 2, pixmap_);
			}
			
		}
	}
	else
	{
		QLabel::paintEvent(e);
	}
}
void RoundLabel::enterEvent(QEvent* event)
{
	bTransLayer_ = true;
	update();//ˢ��label
}
void RoundLabel::leaveEvent(QEvent* event)
{
	bTransLayer_ = false;
	update();//ˢ��label
}
void RoundLabel::mousePressEvent(QMouseEvent* ev)
{ //��굥����Ϣ
	emit sigClick();
}

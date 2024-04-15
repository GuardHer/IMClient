#include "RoundLabel.h"

#include <QPainter>
#include <QPainterPath>

RoundLabel::RoundLabel(QWidget* parent) :QLabel(parent)
{
	bTransLayer_ = false;
}
void RoundLabel::paintEvent(QPaintEvent* e)
{
	if (NULL != pixmap())  // �ж��Ƿ���ͼƬ, ��ͼƬ�����ͼƬ
	{
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);  // �����
		QPainterPath path;
		int round = qMin(width(), height());  // ȡ��Сֵ
		path.addEllipse(0, 0, round, round);  // ����Բ��
		painter.setClipPath(path);           // ���û���·��
		painter.drawPixmap(-1, -1, width() + 2, height() + 2, *pixmap());   // ����ͼƬ
		if (bTransLayer_)   
		{
			//�����label�ϣ�����͸��ͼƬ
			QPixmap pixMap(";/res/head/head1.jpg");
			painter.drawPixmap(-1, -1, width() + 2, height() + 2, pixMap);
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

#include "RoundLabel.h"

#include <QPainter>
#include <QPainterPath>

RoundLabel::RoundLabel(QWidget* parent) :QLabel(parent)
{
	bTransLayer_ = false;  // 默认不透明
}
void RoundLabel::setImage(const QPixmap& pixmap)
{
	pixmap_ = pixmap;
}

void RoundLabel::paintEvent(QPaintEvent* e)
{
	if (NULL != pixmap())
	{
		QPainter painter(this);  // 创建QPainter一个对象
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);  // 抗锯齿
		QPainterPath path;  // 创建一个路径对象
		int round = qMin(width(), height());  // 取最小值
		path.addEllipse(0, 0, round, round);  // 绘制圆形
		painter.setClipPath(path);           // 设置绘制路径
		painter.drawPixmap(-1, -1, width() + 2, height() + 2, *pixmap());   // 绘制图片
		if (bTransLayer_)   
		{
			//鼠标在label上，加载透明图片
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
	update();//刷新label
}
void RoundLabel::leaveEvent(QEvent* event)
{
	bTransLayer_ = false;
	update();//刷新label
}
void RoundLabel::mousePressEvent(QMouseEvent* ev)
{ //鼠标单击消息
	emit sigClick();
}

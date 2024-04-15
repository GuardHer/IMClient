#include "SmoothScrollBar.h"

SmoothScrollBar::SmoothScrollBar(int type, QWidget* parent)
	: QScrollBar(parent)
{
	this->setStyleSheet(
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}"
		"QScrollBar{ background-color:white; width: 10px; }"
		"QScrollBar:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,0%);"
		"margin:0px,0px,0px,0px;"
		"padding-top:0px;"
		"padding-bottom:0px;"
		"}"
		"QScrollBar::handle:vertical"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,25%);"
		" border-radius:4px;"
		"min-height:20px;"
		"max-height:20px"
		"}"
		"QScrollBar::handle:vertical:hover"
		"{"
		"width:8px;"
		"background:rgba(0,0,0,50%);"
		" border-radius:4px;"
		"min-height:20px;"
		"max-height:20px"
		"}"
		"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
		"{"
		"background:rgba(0,0,0,10%);"
		"border-radius:4px;"
		"}"
	);
}

void SmoothScrollBar::wheelEvent(QWheelEvent* event)
{
	/*int numberDegrees = event->angleDelta().y() / 8;
	int numberSteps = (numberDegrees / 15);
	if (event->orientation() == Qt::Vertical)
	{
		this->setValue(this->value() - numberSteps);
	}
	event->accept();
	QWidget::wheelEvent(event);*/
	int para = event->angleDelta().y();//获得鼠标滚轮的滚动距离para，para<0向下滚动，>0向上滚动  
	int tempStep = this->value();
	if (para < 0)
	{
		//向下滚动，设定鼠标滚轮每滚动一个单位，滑块就移动20个单位  
		this->setSliderPosition(tempStep + 20);
	}
	else
	{
		//向上滚动  
		this->setSliderPosition(tempStep - 20);
	}
	event->accept();      //接收该事件
}


SmoothScrollBar::~SmoothScrollBar()
{

}


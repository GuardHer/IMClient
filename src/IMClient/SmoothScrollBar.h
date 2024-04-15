#pragma once

#include <QScrollBar>
#include <QWheelEvent>

class SmoothScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    SmoothScrollBar(int type = 0, QWidget* parent = nullptr);
    ~SmoothScrollBar();

protected:
    void wheelEvent(QWheelEvent* event);

};
#include "Bubblehistory.h"
#include <QListWidgetItem>
#include <QTimer>
#include <QDebug>
#include <QSize>
#include <QTextBrowser>
#include "manager/User.h"
#include "BubbleToMe.h"

BubbleHistory::BubbleHistory(QWidget* parent)
    : QListWidget(parent)
{
    initUI();
    m_user = new User();

}

// 创建一个文本为空的 气泡控件，先显示出来，然后再使用追加文字的方式计算宽和高度
void BubbleHistory::addBubble(const BubbleParas& paras)
{
    switch (paras.role) 
    {
        case BUBBLE_ROLE::BR_ME:
            addBubbleToMe(paras);
            break;
        case BUBBLE_ROLE::BR_OTHER:
            addBubbleToOther(paras);
    }
}
void BubbleHistory::addBubbleToMe(const BubbleParas& paras)
{
    BubbleToMe* bubble = new BubbleToMe("", paras.type, paras.role, paras.time, this);
    QListWidgetItem* item = new QListWidgetItem(this);

    // 取消两个控件之间的间距
    QBoxLayout* layout = qobject_cast<QBoxLayout*>(this->layout());
    if (layout) layout->setSpacing(0);


    this->addItem(item);

    // bubble->height()，bubble->size().height()：前者是实时的，后者是固定的
    const int& bubbleHeight = paras.role == BUBBLE_ROLE::BR_ME ? bubble->height() : bubble->size().height();
    item->setSizeHint(QSize(bubble->size().width(), bubble->size().height()));  // 初次添加进来得时候，需要给定一个大小，此处不能参考下面赋值 item->sizeHint()，一样会有问题
    this->setItemWidget(item, bubble);

    // sigChangedHeight：当气泡控件的高度发生变化时候，会发射此信号，然后在此处接收到信号，然后重新计算 item 的高度
    connect(bubble, &BubbleToMe::sigChangedHeight, [=](int height) {
        bubble->textBrowser()->setFixedHeight(height);
        item->setSizeHint(QSize(item->sizeHint().width(), height + 60));
        });

    if (!paras.text.isEmpty()) // fix: Me 气泡创建时候，先创建一个空白气泡，此时立即 追加文字 时刻，此刻此 item 还没有显示，故计算是错误的
    {
        QTimer::singleShot(10, [bubble, paras]() { bubble->appendText(paras.text); });
        switch (paras.type)
        {
            case MessageType::Text:
				bubble->appendText(paras.text);
				break;
            case MessageType::Image:
                bubble->appendImage(paras.text);
				break;
			case MessageType::File:
				bubble->appendFile(paras.text);
				break;
			case MessageType::Video:
				bubble->appendVideo(paras.text);
				break;
			case MessageType::Voice:
				bubble->appendVoice(paras.text);
				break;
			case MessageType::Undefine:
				bubble->appendUnknown(paras.text);
				break;
			default:
				break;
        }
    }
        
}
void BubbleHistory::addBubbleToOther(const BubbleParas& paras)
{
    Bubble* bubble = new Bubble("", paras.role, paras.time, this);
    QListWidgetItem* item = new QListWidgetItem(this);

    this->addItem(item);

    // bubble->height()，bubble->size().height()：前者是实时的，后者是固定的
    const int& bubbleHeight = paras.role == BUBBLE_ROLE::BR_ME ? bubble->height() : bubble->size().height();
    //    const QSize size(bubble->size().width(),  bubbleHeight);
    item->setSizeHint(QSize(bubble->size().width(), bubble->size().height()));  // 初次添加进来得时候，需要给定一个大小，此处不能参考下面赋值 item->sizeHint()，一样会有问题
    this->setItemWidget(item, bubble);
    if (!paras.text.isEmpty()) // fix: Me 气泡创建时候，先创建一个空白气泡，此时立即 追加文字 时刻，此刻此 item 还没有显示，故计算是错误的
        QTimer::singleShot(10, [bubble, paras]() { bubble->appendText(paras.text); });
}

void BubbleHistory::setUser(const User& user)
{
    m_user = const_cast<User*>(&user); // 
}


Bubble* BubbleHistory::lastBubble(BUBBLE_ROLE role)
{
    // 最后一个 role 类型的气泡
    QListWidgetItem* item = lastListItem(role);
    return item ? qobject_cast<Bubble*>(itemWidget(item)) : nullptr;
}

const int BubbleHistory::lastBubbleHeight(BUBBLE_ROLE role, int& itemHeight, int& browserHeight)
{
    
    const auto& bubble = lastBubble(role);
    if (!bubble) return 0;

    itemHeight = bubble->size().height();          // item 的高度
    browserHeight = bubble->textBrowserheight();   // textBrowser 的高度
    //qDebug() << "--@1->role:" << int(role) << bubble->size() << browserHeight << itemHeight - browserHeight << bubble->isVisible();
    return itemHeight - browserHeight; 
}


QListWidgetItem* BubbleHistory::lastListItem(BUBBLE_ROLE role)
{
    for (int i = this->count() - 1; i >= 0; --i) {
        QListWidgetItem* lastItem = item(i);
        Bubble* bubble = qobject_cast<Bubble*>(this->itemWidget(lastItem));
        if (!bubble) return nullptr;

        if (bubble->role() == role) {
            //qDebug() << "lastListItem:" << bubble << "i:" << i << "text:" << "  count():" << count() << bubble->text();
            return lastItem;
        }
        else {
            continue;
        }
    }

    qDebug() << "lastListItem is nullptr";
    return nullptr;
}

void BubbleHistory::initUI()
{
    // 设置样式表禁用悬浮高亮效果
    //setStyleSheet("QListWidget::item:hover { background-color: transparent; }");
    //resize(1200, 760);

    // 设置滚动条样式，取消
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 取消两个item之间的间距
    this->setSpacing(0);
    //this->layout()->setSpacing(0);

}

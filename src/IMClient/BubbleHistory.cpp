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

// ����һ���ı�Ϊ�յ� ���ݿؼ�������ʾ������Ȼ����ʹ��׷�����ֵķ�ʽ�����͸߶�
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

    // ȡ�������ؼ�֮��ļ��
    QBoxLayout* layout = qobject_cast<QBoxLayout*>(this->layout());
    if (layout) layout->setSpacing(0);


    this->addItem(item);

    // bubble->height()��bubble->size().height()��ǰ����ʵʱ�ģ������ǹ̶���
    const int& bubbleHeight = paras.role == BUBBLE_ROLE::BR_ME ? bubble->height() : bubble->size().height();
    item->setSizeHint(QSize(bubble->size().width(), bubble->size().height()));  // ������ӽ�����ʱ����Ҫ����һ����С���˴����ܲο����渳ֵ item->sizeHint()��һ����������
    this->setItemWidget(item, bubble);

    // sigChangedHeight�������ݿؼ��ĸ߶ȷ����仯ʱ�򣬻ᷢ����źţ�Ȼ���ڴ˴����յ��źţ�Ȼ�����¼��� item �ĸ߶�
    connect(bubble, &BubbleToMe::sigChangedHeight, [=](int height) {
        bubble->textBrowser()->setFixedHeight(height);
        item->setSizeHint(QSize(item->sizeHint().width(), height + 60));
        });

    if (!paras.text.isEmpty()) // fix: Me ���ݴ���ʱ���ȴ���һ���հ����ݣ���ʱ���� ׷������ ʱ�̣��˿̴� item ��û����ʾ���ʼ����Ǵ����
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

    // bubble->height()��bubble->size().height()��ǰ����ʵʱ�ģ������ǹ̶���
    const int& bubbleHeight = paras.role == BUBBLE_ROLE::BR_ME ? bubble->height() : bubble->size().height();
    //    const QSize size(bubble->size().width(),  bubbleHeight);
    item->setSizeHint(QSize(bubble->size().width(), bubble->size().height()));  // ������ӽ�����ʱ����Ҫ����һ����С���˴����ܲο����渳ֵ item->sizeHint()��һ����������
    this->setItemWidget(item, bubble);
    if (!paras.text.isEmpty()) // fix: Me ���ݴ���ʱ���ȴ���һ���հ����ݣ���ʱ���� ׷������ ʱ�̣��˿̴� item ��û����ʾ���ʼ����Ǵ����
        QTimer::singleShot(10, [bubble, paras]() { bubble->appendText(paras.text); });
}

void BubbleHistory::setUser(const User& user)
{
    m_user = const_cast<User*>(&user); // 
}


Bubble* BubbleHistory::lastBubble(BUBBLE_ROLE role)
{
    // ���һ�� role ���͵�����
    QListWidgetItem* item = lastListItem(role);
    return item ? qobject_cast<Bubble*>(itemWidget(item)) : nullptr;
}

const int BubbleHistory::lastBubbleHeight(BUBBLE_ROLE role, int& itemHeight, int& browserHeight)
{
    
    const auto& bubble = lastBubble(role);
    if (!bubble) return 0;

    itemHeight = bubble->size().height();          // item �ĸ߶�
    browserHeight = bubble->textBrowserheight();   // textBrowser �ĸ߶�
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
    // ������ʽ�������������Ч��
    //setStyleSheet("QListWidget::item:hover { background-color: transparent; }");
    //resize(1200, 760);

    // ���ù�������ʽ��ȡ��
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ȡ������item֮��ļ��
    this->setSpacing(0);
    //this->layout()->setSpacing(0);

}

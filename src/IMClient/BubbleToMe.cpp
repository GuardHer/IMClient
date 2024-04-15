#include "BubbleToMe.h"
#include "ui_bubble.h"
#include <QDebug>
#include <QString>
#include <QPalette>
#include <QTextLayout>
#include <QTextOption>
#include <QTextDocument>
#include <QScrollBar>
#include <QFontMetricsF>

BubbleToMe::BubbleToMe(const QString& text, MessageType type, BUBBLE_ROLE role, const QDateTime& time, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::BubbleToMe)
    , m_role(role)
    , m_type(type)
{
    CurrTextWidth = 0;
    ui->setupUi(this);

    connect(this, &BubbleToMe::sigChangedWidth, [=](int textWidth) {
        CurrTextWidth = textWidth;
        if (textWidth < (parent->size().width() - 145 - 200))      //����ı�ֻ��һ�оͿ�����ʾ���ȵ��������ʵĴ�С
        {
            ui->textBrowser->setFixedWidth(textWidth + 24);        //�ı��Ĵ�С+(24)�߾�
        }
        else    //����ı��Ĵ�С�Ǵ����޷�һ����ʾ���������ù̶�����øı�߶�����Ӧ�ı��Ĵ�С
        {
            ui->textBrowser->setFixedWidth(parent->size().width() - 110 - 200);
        }
    });
    //setText(text);
    initUI();
    setName();
    setPhoto();
    setTime(time);
}

BubbleToMe::~BubbleToMe()
{
    delete ui;
}

void BubbleToMe::setText(const QString& text)
{
    ui->textBrowser->setText(text);
}

/***********************************************************************************************************
ui->textBrowser->insertPlainText(text) ׷���ַ���������ӻ��з�; ��߶Ȼᴥ�� ui->textBrowser->document()
�� QTextDocument::contentsChanged �źţ�Ȼ���Զ���ʵ�� textBrowser �ĸ߶�ͨ���źŷ����ȥ��������ͨ�����������
�� ��QListWidgetItem �߶� = textBrowser �߶� + �����ؼ����ֵĸ߶ȡ���

��������Ȼ��Ȼ�ĵõ�Ԥ�ڵ�Ч�� --->  textBrowser ���� text �����Ӷ��߶ȱ�ߣ��Ӷ�ʹ������ Bubble Ҳ���
************************************************************************************************************/
void BubbleToMe::appendText(const QString& text)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ
    ui->textBrowser->insertPlainText(text);        // ׷���ַ���������ӻ��з�
}

void BubbleToMe::appendImage(const QByteArray& data)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ
	ui->textBrowser->insertHtml(QString("<img src=\"data:image/png;base64,%1\"/>").arg(QString(data.toBase64())));
}

void BubbleToMe::appendImage(const QString& path)
{
    // ����ͼƬ·������ͼƬ��������textBrowser��С
    QPixmap pixmap(path);
	int width = pixmap.width();
    int height = pixmap.height();
    qDebug() << "width:" << width;
    qDebug() << "height:" << height;

    const auto h1 = ui->textBrowser->contentsMargins().top() * 2;         // ���¼��
    const auto h2 = ui->textBrowser->document()->documentMargin() * 2;    // ���¼��

    ui->textBrowser->setFixedWidth(width + 20);
    ui->textBrowser->setFixedHeight(height + 20);
    emit sigChangedHeight(height + h1 + h2);

    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ
    //ui->textBrowser->insertHtml(QString("<img src=\"%1\"/>").arg(path));
    //ui->textBrowser->insertHtml(QString("<img src=\"%1\"/>").arg(path));
    QString htmlCode = QString("<img src=\"%1\"/>&nbsp;<br>").arg(path);
    ui->textBrowser->insertHtml(htmlCode);

}

void BubbleToMe::appendImage(const QImage& image)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ

    const uchar* ucharData = image.bits();
    int dataSize = image.byteCount();
    QString myString = QString::fromUtf8(reinterpret_cast<const char*>(ucharData), dataSize);

    ui->textBrowser->insertHtml(QString("<img src=\"data:image/png;base64,%1\"/>").arg(myString));
}

void BubbleToMe::appendImage(const QPixmap& pixmap)
{
    QImage image = pixmap.toImage();
    appendImage(image);
}

void BubbleToMe::appendFile(const QByteArray& data)
{
}

void BubbleToMe::appendFile(const QString& path)
{
}

void BubbleToMe::appendVoice(const QString& path)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ
	ui->textBrowser->insertHtml(QString("<audio src=\"%1\"/>").arg(path));
}

void BubbleToMe::appendVoice(const QByteArray& data)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ
	ui->textBrowser->insertHtml(QString("<audio src=\"data:audio/mp3;base64,%1\"/>").arg(QString(data.toBase64())));
}

void BubbleToMe::appendVideo(const QString& path)
{
}

void BubbleToMe::appendVideo(const QByteArray& data)
{
}

void BubbleToMe::appendLink(const QString& link, const QString& title, const QString& desc, const QString& icon)
{
}

void BubbleToMe::appendLink(const QString& url)
{
    ui->textBrowser->moveCursor(QTextCursor::End); // ������ƶ����ı�ĩβ
	ui->textBrowser->insertHtml(QString("<a href=\"%1\">%1</a>").arg(url));
}

void BubbleToMe::appendUserCard(const QString& name, const QString& desc, const QString& icon)
{
}

void BubbleToMe::appendUnknown(const QString& text)
{
}

void BubbleToMe::setName(const QString& name)
{
    QString t = "";
    if (m_role == BUBBLE_ROLE::BR_OTHER) {
        t = "OTher";
    }
    else {
        t = name.isEmpty() ? "Me" : name;
    }

    ui->labName->setText(t);
}

void BubbleToMe::setPhoto()
{
    auto& labPhoto = ui->labPhoto;
    const QString& path = m_role == BUBBLE_ROLE::BR_OTHER ? ":/res/head/head1.jpg" : ":/res/head/head1.jpg";
    const QPixmap* pixmap = new QPixmap(path);
    labPhoto->setPixmap(*pixmap);
}

void BubbleToMe::setTime(const QDateTime& time)
{
    if (m_role != BUBBLE_ROLE::BR_STATUS)
        ui->labStatus->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}

const QString BubbleToMe::text()
{
    return ui->textBrowser->toPlainText();
}

// ���ݵ�ǰ���ݵĽ�ɫ���Զ���ʾ�����Ҳ�ĵĶ����ť������״̬
void BubbleToMe::autoRoleBtn()
{
    ui->labStatus->setVisible(false);
    ui->textBrowser->setVisible(true);

    if (m_role == BUBBLE_ROLE::BR_ME) {
        //ui->tbEditor->setVisible(true);
        //ui->tbBot->setVisible(false);
        //ui->tbLove->setVisible(false);
//        setWidgetBackgroundColor(this, QColor("#e9e9e8"));
    }
    else if (m_role == BUBBLE_ROLE::BR_OTHER) {
        //ui->tbEditor->setVisible(false);
        //ui->tbBot->setVisible(true);
        //ui->tbLove->setVisible(true);
    }
    else if (m_role == BUBBLE_ROLE::BR_STATUS) {
        //ui->tbEditor->setVisible(false);
        //ui->tbBot->setVisible(false);
        //ui->tbLove->setVisible(false);

        ui->labStatus->setVisible(true);
        ui->textBrowser->setVisible(false);
    }
}

void BubbleToMe::initUI()
{
    autoRoleBtn();
    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // ֻ�ǲ���ʾ����ʵ��Ҳ�Ǵ��ڵģ��߶ȵ����м���
    ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("QListWidget::item { margin: 0px; }");
    this->setStyleSheet("border:none;");

    // ���ı��Զ���������ӦQTextBrowser�Ŀ�ȡ����ı��Ŀ�ȳ����ؼ��Ŀ��ʱ�����Զ����ı����л�����ʾ����ȷ���ı������ڿؼ��ڲ���ȫ�ɼ���
    ui->textBrowser->setLineWrapMode(QTextEdit::WidgetWidth);

    qDebug() << m_type;

    switch (m_type) {
    case MessageType::Text:
        connectContentsChanged();
        break;
    case MessageType::Image:
        
        break;
    case MessageType::File:
        break;
    case MessageType::Voice:
        break;
    case MessageType::Video:
        break;
    }
}

void BubbleToMe::setWidgetBackgroundColor(QWidget* widget, const QColor& color)
{
    if (!widget)
        return;

#if 1
    widget->setStyleSheet(QString("background-color: %1;").arg(color.name()));
#else
    //    QPalette palette = widget->palette();
    //    widget->setAutoFillBackground(true);
    //    palette.setColor(QPalette::Background, color);
    //    widget->setPalette(palette);
#endif

    // �ݹ������ӿؼ��ı���ɫ
    for (auto& childWidget : widget->findChildren<QWidget*>())
        setWidgetBackgroundColor(childWidget, color);
}

BUBBLE_ROLE BubbleToMe::role() const
{
    return m_role;
}

// ���δ��� Bubble����ʱ��һ���Ը������е��ı�����߶ȸ� items �᲻�ԣ��������¼���һ��
const int BubbleToMe::height() const
{
    const auto& margin = ui->gridLayout->contentsMargins();
    const int height = ui->textBrowser->size().height() + ui->labPhoto->size().height()
        + margin.top() + margin.bottom() + ui->gridLayout->verticalSpacing();

    //    qDebug() << "textBrowser:" << ui->textBrowser->size().height() <<  "labPhoto:" << ui->labPhoto->size().height();
    //    qDebug() << "margin:" << margin <<  "verticalSpacing:" << ui->gridLayout->verticalSpacing();
    return height;
}

const int BubbleToMe::textBrowserheight() const
{
    return ui->textBrowser->size().height();
}

void BubbleToMe::printfInfo() const
{
    qDebug() << "textBrowser-> size():" << ui->textBrowser->size() << "size():" << ui->textBrowser->sizeHint();
    qDebug() << "this Bubble-> size():" << this->size() << "sizeHint():" << this->sizeHint();
    qDebug() << "labPhoto -> size():" << ui->labPhoto->size() << "sizeHint():" << ui->labPhoto->sizeHint();
    qDebug() << "labName -> size():" << ui->labName->size() << "sizeHint():" << ui->labName->sizeHint();
    qDebug() << "labStatus -> size():" << ui->labStatus->size() << "sizeHint():" << ui->labStatus->sizeHint();
    //qDebug() << "tbEditor -> size():" << ui->tbEditor->size() <<  "sizeHint():" << ui->tbEditor->sizeHint();

}

/*------------------------------- connect -------------------------------*/

void BubbleToMe::connectContentsChanged()
{
    connect(ui->textBrowser->document(), &QTextDocument::contentsChanged, [=]() {
        // [���ս������] ��ʱ AI �� ME Bubble �Ѿ���ʾ�����ˣ�ͨ����֪���� ����ͨ�� ���ֵ��к͸߶������¼���ʵ�ʸ߶�
        const auto& textBrowser = ui->textBrowser;
        const QString& text = textBrowser->toPlainText();
        int allRowCount = 0;
        int i = 1;

        QFontMetrics fm(textBrowser->font());
        int maxWidth = textBrowser->size().width();

        for (const QString& line : text.split('\n')) {
            int lineWidth = fm.horizontalAdvance(line);
            int realRowCount = static_cast<int>(lineWidth / maxWidth) + 1;
            allRowCount += realRowCount;
            //qDebug() << "i:" << i++ << "  lineWidth:" << lineWidth << "  realRowCount:" << realRowCount << "  allRowCount:" << allRowCount << "  line:" << line;
        }

        if (text.isEmpty()) allRowCount = 0;  // �ı�Ϊ���� 0 ��
        int height = allRowCount * fm.lineSpacing();

        const auto h1 = textBrowser->contentsMargins().top() * 2;         // ���¼��
        const auto h2 = textBrowser->document()->documentMargin() * 2;    // ���¼��

        QFontMetrics fontSize(ui->textBrowser->font());                    // ��ȡ�����С
        QString myText = ui->textBrowser->toPlainText();                   // ��ȡ�ı�
        int textWidth = fontSize.size(Qt::TextSingleLine, myText).width(); // ��ȡ�ı��Ŀ��

        emit sigChangedWidth(textWidth + 20);  // �ǵü��ϼ��
        emit sigChangedHeight(height + h1 + h2);  // �ǵü��ϼ��

        });
}

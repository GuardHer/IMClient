#include "chatMessage.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QBitmap>
#include <QLabel>
#include <QDebug>
#include <QPointF>
#include <QColor>
#include <iostream>

#include "Self.h"
#include "UserList.h"
#include "base/Singleton.h"

#include "base/Logging.h"

ChatMessage::ChatMessage(const QString& userId, QColor color, int type, uint32_t messageId, QWidget* parent)
    : QWidget(parent), m_type{ type }, m_userId{ userId }, m_messageId{ messageId }
{
    QFont te_font = this->font(); 
    te_font.setFamily("MicrosoftYaHei"); // 设置字体
    te_font.setPointSize(12); // 设置字体大小
    colme_Kuang = color;       // 设置框颜色
    te_font.setWordSpacing(0); // 设置单词间距
    te_font.setLetterSpacing(QFont::PercentageSpacing,0);
    te_font.setLetterSpacing(QFont::PercentageSpacing, 100);          //300%,100为默认  //设置字间距%
    te_font.setLetterSpacing(QFont::AbsoluteSpacing, 0);             //设置字间距为3像素 //设置字间距像素值
    this->setFont(te_font);

    User *user = UserList::findFriend(userId);
    Self &self = Singleton<Self>::instance();

    m_leftPixmap = QPixmap(user->getCustomFace());
    m_rightPixmap = QPixmap(self.getSelf()->getCustomFace());

    m_loadingMovie = new QMovie(this);
    m_loadingMovie->setFileName(":/res/animation/loading.gif");
    m_loading = new QLabel(this);
    m_loading->setMovie(m_loadingMovie);
    m_loading->resize(16, 16);
    m_loading->setAttribute(Qt::WA_TranslucentBackground, true);
    m_loading->setAutoFillBackground(false);

    m_imageLabel = new QLabel(this);
    m_imageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    //    m_imageLabel->setFrameShape (QFrame::Box);
    //    m_imageLabel->setStyleSheet("border-width: 2px;border-style: solid;border-color: black;");
    m_imageLabel->resize(200, 200);
    m_imageLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_imageLabel->setAutoFillBackground(false);
    m_imageLabel->hide();

}

void ChatMessage::setTextSuccess(bool isSending)
{
    m_loading->hide();
    m_loadingMovie->stop();
    m_isSending = isSending;
}

void ChatMessage::setTextSending(bool isSending)
{
    m_isSending = isSending;
    if (m_isSending) {
        m_loading->move(m_kuangRightRect.x() - m_loading->width() - 10, m_kuangRightRect.y() + m_kuangRightRect.height() / 2 - m_loading->height() / 2);
        m_loading->show();
        m_loadingMovie->start();
    }
    else {
        m_loading->hide();
        m_loadingMovie->stop();
    }
}

void ChatMessage::setText(bool isSending, QString text, QString time, QSize allSize, ChatMessage::User_Type userType)
{
    m_msg = text;
    m_userType = userType;
    m_time = time;
    m_curTime = QDateTime::fromTime_t(time.toInt()).toString("hh:mm");
    m_allSize = allSize;
    m_isSending = isSending;
    if (userType == User_Me) {
        if (m_isSending) {
            m_loading->move(m_kuangRightRect.x() - m_loading->width() - 10, m_kuangRightRect.y() + m_kuangRightRect.height() / 2 - m_loading->height() / 2);
            m_loading->show();
            m_loadingMovie->start();
        }
        else {
            m_loading->hide();
        }
    }
    
    this->update();
}

QSize ChatMessage::fontRect(QString str)
{
    m_msg = std::move(str);
    int minHei = 30;            // 最小高度
    int iconWH = 40;            // 头像宽高
    int iconSpaceW = 20;        // 头像间隔
    int iconRectW = 5;          // 头像框宽度
    int iconTMPH = 10;          // 头像上边距
    int sanJiaoW = 6;           // 三角宽度
    int kuangTMP = 20;          // 框与边框的距离
    int textSpaceRect = 12;     // 文字与框的距离
    m_kuangWidth = this->width() - kuangTMP - 2 * (iconWH + iconSpaceW + iconRectW);
    m_textWidth = m_kuangWidth - 2 * textSpaceRect;
    m_spaceWid = this->width() - m_textWidth;
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);

    QSize size = getRealString(m_msg); // 整个的size
    //qDebug() << m_msg << m_msg.size();

    int hei = size.height() < minHei ? minHei : size.height();

    m_sanjiaoLeftRect = QRect(iconWH + iconSpaceW + iconRectW, m_lineHeight / 2, sanJiaoW, hei - m_lineHeight);
    m_sanjiaoRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - sanJiaoW, m_lineHeight / 2, sanJiaoW, hei - m_lineHeight);

    if (size.width() < (m_textWidth + m_spaceWid)) {
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), m_lineHeight / 4 * 3, size.width() - m_spaceWid + 2 * textSpaceRect, hei - m_lineHeight);
        m_kuangRightRect.setRect(this->width() - size.width() + m_spaceWid - 2 * textSpaceRect - iconWH - iconSpaceW - iconRectW - sanJiaoW,
            m_lineHeight / 4 * 3, size.width() - m_spaceWid + 2 * textSpaceRect, hei - m_lineHeight);
    }
    else {
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), m_lineHeight / 4 * 3, m_kuangWidth, hei - m_lineHeight);
        m_kuangRightRect.setRect(iconWH + kuangTMP + iconSpaceW + iconRectW - sanJiaoW, m_lineHeight / 4 * 3, m_kuangWidth, hei - m_lineHeight);
    }
    m_textLeftRect.setRect(m_kuangLeftRect.x() + textSpaceRect, m_kuangLeftRect.y() + iconTMPH,
        m_kuangLeftRect.width() - 2 * textSpaceRect, m_kuangLeftRect.height() - 2 * iconTMPH);
    m_textRightRect.setRect(m_kuangRightRect.x() + textSpaceRect, m_kuangRightRect.y() + iconTMPH,
        m_kuangRightRect.width() - 2 * textSpaceRect, m_kuangRightRect.height() - 2 * iconTMPH);
    
    return QSize(size.width(), hei);
}

void ChatMessage::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
}

QPixmap ChatMessage::getPixmap()
{
    return m_pixmap;
}

void ChatMessage::setImageLabel(QPixmap pixmap, QString time, ChatMessage::User_Type userType)
{
    m_pixmap = pixmap;
    int iconWH = 40;
    int iconSpaceW = 20;
    int iconTMPH = 10;
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);
    m_imageLabel->show();
    m_imageLabel->setPixmap(m_pixmap);
    m_userType = userType;
    m_time = time;
    m_curTime = QDateTime::fromTime_t(time.toInt()).toString("hh:mm");
    m_imageY = 10;
    if (userType == User_She) {
        //if(!m_isSending) {
//            m_loading->move(m_kuangRightRect.x() - m_loading->width() - 10, m_kuangRightRect.y()+m_kuangRightRect.height()/2- m_loading->height()/2);
//            m_loading->show();
//            m_loadingMovie->start();
        m_imageX = 70;
        //}
    }
    else if (userType == User_Me)
    {
        m_imageX = this->width() - 270;

    }
    else {
        m_loading->hide();
    }
    m_imageLabel->move(m_imageX, m_imageY);
    QSize picSize(200, 200);
    QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_imageLabel->setPixmap(scaledPixmap);
    m_imageLabel->show();
    this->update();
}

void ChatMessage::slotThemeStyle(bool isChange)
{
    if (isChange)
    {
        colme_Kuang.setRgb(255, 85, 0);  // 橙色
    }
    else
    {
        colme_Kuang.setRgb(75, 164, 242);  // 蓝色
    }
    update();
}

QSize ChatMessage::getRealString(QString& src)
{
    // 获取字体度量工具
    QFontMetricsF fm(this->font());
    // 获取行间距
    m_lineHeight = fm.lineSpacing();
    // 统计文本中换行符的数量
    int nCount = src.count("\n");
    // 记录最大宽度
    int nMaxWidth = 0;

    // 如果文本中没有换行符
    if (nCount == 0) {
        // 计算文本宽度
        nMaxWidth = fm.width(src) + 3;  
        //std::cout << "nMaxWidth: " << nMaxWidth  << ", len: " << src.length() << std::endl;
        QString value = src;

        // 如果文本宽度超过指定的最大宽度（m_textWidth）
        if (nMaxWidth > m_textWidth) {
            nMaxWidth = m_textWidth;

            // 计算需要换行的次数
            int size = m_textWidth / fm.width(" ");  // size` 表示每行文本的字符数
            int num = fm.width(value) / m_textWidth;  // num` 表示文本需要换行的次数
            int ttmp = num * fm.width(" ");  	// ttmp` 表示每行文本的字符数
            num = (fm.width(value)) / m_textWidth;  // num` 表示文本需要换行的次数
            nCount += num;

            // 将文本分段并在每段末尾添加换行符
            QString temp = "";
            for (int i = 0; i < num; i++) {
                temp += value.mid(i * size, (i + 1) * size) + "\n";
            }
            // 替换原始文本
            src.replace(value, temp);
        }
    }
    // 如果文本中有换行符
    else {
        for (int i = 0; i < (nCount + 1); i++) {
            // 获取每一行的文本
            QString value = src.split("\n").at(i);
            // 更新最大宽度
            nMaxWidth = fm.width(value) > nMaxWidth ? fm.width(value) : nMaxWidth;
            nMaxWidth += 3;

            // 如果该行文本宽度超过指定的最大宽度（m_textWidth）
            if (fm.width(value) > m_textWidth) {
                nMaxWidth = m_textWidth;
                int size = m_textWidth / fm.width(" ");
                int num = fm.width(value) / m_textWidth;
                num = ((i + num) * fm.width(" ") + fm.width(value)) / m_textWidth;
                nCount += num;

                // 将文本分段并在每段末尾添加换行符
                QString temp = "";
                for (int i = 0; i < num; i++) {
                    temp += value.mid(i * size, (i + 1) * size) + "\n";
                }
                // 替换原始文本
                src.replace(value, temp);
            }
        }
    }
    // 返回计算后的文本尺寸
    return QSize(nMaxWidth + m_spaceWid, (nCount + 1) * m_lineHeight + 2 * m_lineHeight + 2);
}


void ChatMessage::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));
    

    if (m_userType == User_Type::User_She) { // 用户
        //头像
        //        painter.drawRoundedRect(m_iconLeftRect,m_iconLeftRect.width(),m_iconLeftRect.height());
        //painter.drawPixmap(m_iconLeftRect, m_leftPixmap);
        painter.drawPixmap(m_iconLeftRect, PixmapToRound(m_leftPixmap, m_leftPixmap.width() / 4));
        if (m_type == 0)
        {
            //框加边
            QColor col_KuangB(234, 234, 234);
            painter.setBrush(QBrush(col_KuangB));
            painter.drawRoundedRect(m_kuangLeftRect.x() - 1, m_kuangLeftRect.y() - 1, m_kuangLeftRect.width() + 2, m_kuangLeftRect.height() + 2, 4, 4);
            //框
            QColor col_Kuang(255, 255, 255);
            painter.setBrush(QBrush(col_Kuang));
            painter.drawRoundedRect(m_kuangLeftRect, 4, 4);

            //三角
            QPointF points[3] = {
                QPointF(m_sanjiaoLeftRect.x(), 30),
                QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 25),
                QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 35),
            };
            QPen pen;
            pen.setColor(col_Kuang);
            painter.setPen(pen);
            painter.drawPolygon(points, 3);

            //三角加边
            QPen penSanJiaoBian;
            penSanJiaoBian.setColor(col_KuangB);
            painter.setPen(penSanJiaoBian);
            painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 24));
            painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 36));

            //内容
            QPen penText;
            penText.setColor(QColor(51, 51, 51));
            painter.setPen(penText);
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);   // 文字居左，垂直居中
            option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
            painter.setFont(this->font());
            painter.drawText(m_textLeftRect, m_msg, option);
        }

        else if (m_type == 3) { // 文件
            //框
            painter.setBrush(QBrush(QColor(255, 248, 247)));
            painter.drawRoundedRect(m_kuangLeftRect, 8, 8);

            //三角
            QPointF points[3] = {
                QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 30),
                QPointF(m_sanjiaoLeftRect.x(), 25),
                QPointF(m_sanjiaoLeftRect.x(), 35),
            };
            QPen pen;
            pen.setColor(QColor(255, 248, 247));
            painter.setPen(pen);
            painter.drawPolygon(points, 3);  // 画三角

            //文件图标
            painter.drawPixmap(m_kuangLeftRect.x() + 10, m_kuangLeftRect.y() + 10, m_fileInfo.fileIcon);

            //文件名
            QPen penText;
            penText.setColor(Qt::black);
            painter.setPen(penText);
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);  // 文字居左，垂直居中
            option.setWrapMode(QTextOption::WordWrap);
            painter.setFont(this->font());
            painter.drawText(m_kuangLeftRect.x() + 120, m_kuangLeftRect.y() + 28, m_fileInfo.fileName);

            //文件大小
            QFont font;
            font.setFamily("MicrosoftYaHei");
            font.setPointSize(10);
            penText.setColor(QColor(153, 153, 153));
            painter.setPen(penText);
            painter.setFont(font);
            painter.drawText(m_kuangLeftRect.x() + 220, m_kuangLeftRect.y() + 80, QString::number(m_fileInfo.fileSize) + m_fileInfo.fileUnit);

        }
    }
    else if (m_userType == User_Type::User_Me) { // 自己
        //头像

        painter.drawPixmap(m_iconRightRect, PixmapToRound(m_rightPixmap, m_rightPixmap.width() / 4));

        //qDebug() << "m_type: " << m_type;
        if (m_type == 0)  // 文字
        {
            //框
            painter.setBrush(QBrush(colme_Kuang));                 
            painter.drawRoundedRect(m_kuangRightRect, 8, 8);  

            //三角
            QPointF points[3] = {
                QPointF(m_sanjiaoRightRect.x() + m_sanjiaoRightRect.width(), 30),
                QPointF(m_sanjiaoRightRect.x(), 25),
                QPointF(m_sanjiaoRightRect.x(), 35),
            };
            QPen pen;
            pen.setColor(colme_Kuang);
            painter.setPen(pen);
            painter.drawPolygon(points, 3);

            //内容
            QPen penText;
            penText.setColor(Qt::white);
            painter.setPen(penText);
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);  // 文字居左，垂直居中
            option.setWrapMode(QTextOption::WordWrap);  
            //option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
            painter.setFont(this->font());

            painter.drawText(m_textRightRect, m_msg, option);
        }
        else if (m_type == 3) { // 文件
			//框
			painter.setBrush(QBrush(QColor(255, 248, 247)));
			painter.drawRoundedRect(m_kuangRightRect, 8, 8);

			//三角
            QPointF points[3] = {
				QPointF(m_sanjiaoRightRect.x() + m_sanjiaoRightRect.width(), 30),
				QPointF(m_sanjiaoRightRect.x(), 25),
				QPointF(m_sanjiaoRightRect.x(), 35),
			};
			QPen pen;
			pen.setColor(QColor(255, 248, 247));
			painter.setPen(pen);
			painter.drawPolygon(points, 3);  // 画三角

			//文件图标
			painter.drawPixmap(m_kuangRightRect.x() + 10, m_kuangRightRect.y() + 10, m_fileInfo.fileIcon);

			//文件名
			QPen penText;
			penText.setColor(Qt::black);
			painter.setPen(penText);
			QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);  // 文字居左，垂直居中
			option.setWrapMode(QTextOption::WordWrap);
			painter.setFont(this->font());
			painter.drawText(m_kuangRightRect.x() + 120, m_kuangRightRect.y() + 28, m_fileInfo.fileName);

			//文件大小
            QFont font;
            font.setFamily("MicrosoftYaHei");
            font.setPointSize(10);
            penText.setColor(QColor(153, 153, 153));
            painter.setPen(penText);
            painter.setFont(font);
			painter.drawText(m_kuangRightRect.x() + 220, m_kuangRightRect.y() + 80, QString::number(m_fileInfo.fileSize) + m_fileInfo.fileUnit);

        }
    }
    else if (m_userType == User_Type::User_Time) { // 时间
        QPen penText;
        penText.setColor(QColor(153, 153, 153));
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont te_font = this->font();
        te_font.setFamily("Microsoft YaHei");
        te_font.setPointSize(10);
        painter.setFont(te_font);
        painter.drawText(this->rect(), m_curTime, option);
    }
}


void ChatMessage::setFile(const QPixmap& fileIcon, const QString& fineName, const qint64& fileSize)
{
    m_fileInfo.fileIcon = fileIcon;
    m_fileInfo.fileName = fineName;
    m_fileInfo.fileSize = fileSize;
    m_fileInfo.fileUnit = "B";

    // 转换文件大小单位
    if (m_fileInfo.fileSize > 1024 * 1024) {
        m_fileInfo.fileSize = m_fileInfo.fileSize / (1024 * 1024);
        m_fileInfo.fileUnit = "MB";
	}
	else if (m_fileInfo.fileSize > 1024) {
        m_fileInfo.fileSize = m_fileInfo.fileSize / 1024;
        m_fileInfo.fileUnit = "KB";
	}
	else {
        m_fileInfo.fileSize = m_fileInfo.fileSize;
        m_fileInfo.fileUnit = "B";
	}
}

FileInfo ChatMessage::getFileInfo()
{
    return m_fileInfo;
}

QPixmap ChatMessage::PixmapToRound(QPixmap& src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2 * radius, 2 * radius);
    //掩码图（黑白色）
    QBitmap mask(size);  // 位图
    QPainter painter(&mask);  // 画布
    //Antialiasing：反走样（抗锯齿）
    painter.setRenderHint(QPainter::Antialiasing);
    //SmoothPixmapTransform：用来在对图片进行缩放时启用线性插值算法而不是最邻近算法
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    //填充矩形
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    //画刷
    painter.setBrush(QColor(0, 0, 0));
    //绘制圆角矩形
    /*QPainter::drawRoundedRect
       (const QRectF &rect,
        qreal xRadius,
        qreal yRadius,
        Qt::SizeMode mode = Qt::AbsoluteSize)
    */
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 150, 150);
    //自适应图片
    QPixmap image = src.scaled(size);
    //setMask：创建不规则窗口使用
    image.setMask(mask);
    return image;
}

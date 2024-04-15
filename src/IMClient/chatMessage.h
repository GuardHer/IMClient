#pragma once

#include <QWidget>
#include <QPainterPath>

#include "widget/RoundLabel.h"

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

struct FileInfo 
{
	QString fileName;  // 文件名
	qint64  fileSize;  // 文件大小
	QPixmap fileIcon;  // 文件图标
    QString fileUnit;  // 文件单位

    bool isUpload   = false;  // 是否上传
    bool isDownload = false;  // 是否下载
};

class ChatMessage : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMessage(const QString& userId, QColor color, int type, uint32_t messageId, QWidget* parent = nullptr);

    enum User_Type {
        User_She = 0,   //用户
        User_Me = 1,    //自己
        User_System = 2,//系统
        User_Time,  //时间
    };

    /**
     * @function setTextSuccess
     * @brief 设置文本是否发送成功
     * @param isSending: true 发送中，false 发送成功
     */
    void setTextSuccess(bool isSending);

    void setTextSending(bool isSending);

    /**
     * @function setText
     * @brief 设置文本内容
     * @param isSending: true 发送中，false 发送成功
     * @param text: 文本内容
     * @param time: 时间
     * @param allSize: 控件大小
     * @param userType: 用户类型
     */
    void setText(bool isSending, QString text, QString time, QSize allSize, User_Type userType);

    /**
     * @function getRealString
     * @brief 获取文本内容的真实大小
     * @param src: 文本内容
     * @return : 文本内容的真实大小
     */
    QSize getRealString(QString& src);

    /**
     * @function fontRect
     * @brief   
     * @param str: 文本内容
     * @return : 
     */
    QSize fontRect(QString str);

    /**
     * @function setPixmap
     * @brief 设置图片
     * @param pixmap:
     */
    void  setPixmap(QPixmap pixmap);

    /**
     * @function getPixmap
     * @brief  
     * @return : 
     */
    QPixmap getPixmap();

    /**
     * @function setImageLabel
     * @brief   
     * @param pixmap:
     * @param time:
     * @param userType:
     */
    void  setImageLabel(QPixmap pixmap, QString time, User_Type userType);

    /**
     * @function text
     * @brief 获取文本内容
     * @return : 文本内容
     */
    inline QString text() { return m_msg; }  // inline 内联函数: 代码替换，提高效率

    /**
     * @function time
     * @brief  
     * @return : 
     */
    inline QString time() { return m_time; }

    /**
     * @function userType
     * @brief  
     * @return :
     */
    inline User_Type userType() { return m_userType; }

    /**
	 * @function allSize
	 * @brief  
	 * @return : 
	 */
    void setFile(const QPixmap& fileIcon, const QString& fineName, const qint64& fileSize);

    /**
	 * @function allSize
	 * @brief 
	 * @return : 
	 */
    FileInfo getFileInfo();

    /**
     * @function type
     * @brief 
     * @return: m_type
     */
    int type() { return m_type; }

    uint32_t getMessageId() { return m_messageId; }

private:
    /**
     * @function PixmapToRound
     * @brief 图片转圆角
     * @param src: 图片
     * @param radius: 圆角半径
     * @return : 圆角图片
     */
    QPixmap PixmapToRound(QPixmap& src, int radius);

public slots:
    void slotThemeStyle(bool isChange);
protected:
    void paintEvent(QPaintEvent* event);
private:
    uint32_t m_messageId;                           //消息ID

    QString m_userId;                              //用户ID

    QString m_msg;                                 //文本内容
    QString m_time;                                //时间
    QString m_curTime;                             //当前时间
    QColor colme_Kuang;                            //边框颜色

    QSize m_allSize;                               //控件大小
    User_Type m_userType = User_System;            //用户类型

    int m_kuangWidth;                              //边框宽度
    int m_textWidth;                               //文本宽度
    int m_spaceWid;                                //文本与边框间隔
    int m_lineHeight;                              //行高

    QRect m_iconLeftRect;                          //左边图标区域
    QRect m_iconRightRect;                         //右边图标区域
    QRect m_sanjiaoLeftRect;                       //左边三角区域
    QRect m_sanjiaoRightRect;                      //右边三角区域
    QRect m_kuangLeftRect;                         //左边边框区域
    QRect m_kuangRightRect;                        //右边边框区域
    QRect m_textLeftRect;                          //左边文本区域
    QRect m_textRightRect;                         //右边文本区域
    QPixmap m_leftPixmap;                          //左边图标
    QPixmap m_rightPixmap;                         //右边图标
    QLabel* m_loading = Q_NULLPTR;                 //加载中
    QMovie* m_loadingMovie = Q_NULLPTR;            //加载中动画
    bool m_isSending = false;                      //是否发送中

    QPixmap m_pixmap;                              //图片
    int     m_type;                                //0文本，1图片，2视频, 3文件
    QLabel* m_imageLabel = nullptr;                //图片控件
    size_t  m_imageX;                              //图片x坐标
    size_t  m_imageY;                              //图片y坐标

    FileInfo m_fileInfo;                           //文件信息

};


#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QList>
#include <QObject>
#include <QVector>
#include <map>
#include "ui_MainWindow.h"
#include "base/config.h"

#include "LoginWidget.h"
#include "../manager/User.h"
#include "../FriendItem.h"
#include "../SQLiteManager.h"
#include "../emoji/EmojiDB.hpp"
#include "../emoji/EmojiWindow.hpp"

#include "../FriendChatWidget.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT
		 
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	bool init();

private:
	void InitBtn();
	void InitConnect();
	void InitStyle();
	void InitEmoji();

	void connectBtn(); // 连接按钮

	void addFriendItem(QString userName, QString userId, QString head, QString lastMsg); //添加好友
	void addGroupItem(QString groupName, QString groupId, QString head, QString lastMsg); //添加群组

	void loadFriends(); // 加载好友
	void loadGroups(); // 加载群组

	void loadFriendList(); // 加载好友列表
	void loadGroupList(); // 加载群组列表

	void setCurrentFriendCard(const QString& userId); // 设置当前好友名片
	void setCurrentGroupCard(const QString& groupId); // 设置当前群组名片

	QPixmap PixmapToRound(QPixmap& src, int radius); // 图片圆角化

protected:
	// 窗口关闭事件
	void closeEvent(QCloseEvent* event) override;

public slots:
	void handleUser(const QString& userId); // 处理用户登录
	
	//好友列表点击
	void onFriendChatItemClicked(QListWidgetItem* item);           // 好友列表点击
	void onGroupChatItemClicked(QListWidgetItem* item);            // TODO: 群组列表点击
	void onFriendCardItemClicked(QListWidgetItem* item);           // TODO: 好友名片点击
	void onGroupCardItemClicked(QListWidgetItem* item);            // TODO: 群组名片点击


	// button
	void onChatBtnClicked(); // 聊天按钮
	void onFriendBtnClicked(); // 好友按钮
	void onSettingBtnClicked(); // 设置按钮
	void onFriendCardSendBtnClicked(); // 好友名片发送消息
	void onGroupCardSendBtnClicked(); // 群组名片发送消息

	// emoji
	void onEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone); // 表情符号选择
	void onCanceled(); // 取消

	// recv msg
	void onRecvMessage(const std::string& from, int type, const QByteArray& data); // 接收消息
	void onSendChatMsgResponse(bool success, const std::string& toUserId, uint32_t seq);  // 发送聊天消息响应

	// file slot
	void onUploadResponse(const std::string& from, const std::string& to, uint32_t seq, bool isSuccess);
	void onDownloadResponse(const std::string& from, const std::string& to, const std::string& fileName, uint32_t seq, bool isSuccess);

signals:
	void emitClose(); // 关闭信号
	void emitEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, const QString& userId); // 表情符号选择信号

private:
	Ui::MainWindow *ui;
	
	SQLiteManager &sqlManager_;

	QString userName_;
	QString userId_;

	QList<FriendItem*> friendItems_;  // 好友列表item 
	QList<FriendItem*> groupItems_;   // 群组列表item

	std::map<QString, FriendChatWidget*> chatPages_;  // 群组聊天列表
};

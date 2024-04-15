#pragma once

#include <QWidget>
#include <unordered_map>

class MyIconButton;
class QVBoxLayout;
class QLabel;
class QListWidget;
class QPushButton;
class QTextEdit;
class QPlainTextEdit;
class ChatListWidget;
class QByteArray;
class ChatMessage;

#include "ChatInput.h"
#include "SQLiteMessage.h"
#include "SmoothScrollBar.h"
#include "SQLiteManager.h"

#include "emoji/EmojiDB.hpp"
#include "emoji/EmojiWindow.hpp"
#include "emoji/Settings.hpp"

#include "Self.h"
#include "IMClient.h"
#include "ClientSession.h"

enum class Format
{
	UTF8,
	CODEPOINTS_HEX,
};

class FriendChatWidget:public QWidget
{
	Q_OBJECT
public:
	FriendChatWidget(const QString& userName, const QString& userId, QWidget* parent = nullptr);
	~FriendChatWidget();

public:
	// 返回userId，便于通过userId查找聊天窗口
	QString getUserId() const { return userId_; }

	// 返回userName
	QString getUserName() const { return userName_; }

	void initChatMessage();

	// receive message from server
	void onRecvMessage(const std::string& from, int type, const QByteArray& data);
	// receive send message response from server
	void onSendMessageResponse(uint32_t messageId);

	// upload file response
	void onUploadResponse(uint32_t messageId);
	// download file response
	void onDownloadResponse(uint32_t messageId);


private:
	void init();
	void initConnect();
	void testBubble();
	void initEmoji();

	bool SendMessageText(const QString& msg, int IsSender, bool isSending, bool fromDb);
	bool SendMessageText(const QByteArray& msg, int IsSender, bool isSending, bool fromDb);
	bool SendMessageImage(const QString& imagePath, int IsSender, bool isSending, bool fromDb );
	bool SendMessageImage(const QByteArray& image, int IsSender, bool isSending, bool fromDb );
	bool SendMessageFile(const QString& filePath, int IsSender, bool isSending, bool fromDb );

	std::string formatEmoji(const Emoji& emoji,
		const std::optional<Emoji::SkinTone>& skinTone, const Format fmt,
		const std::string& cpPrefix, const bool noNl);

	// save message to database
	void saveMessageToDB(const Message& message);
	void saveMessageToDB(const QString& talker, bool isSender, MessageType::Type type, const QString& content);
	// send message to server
	void sendMessageToServer(const QByteArray& content);
	// add message to cache
	void addMessageToCache(const QString& talker, bool isSender, MessageType::Type type, const QString& content);

private:
	//QWidget* FriendChatPage_;
	QWidget* layoutWidget3{ nullptr };
	QVBoxLayout* verticalLayout_3{ nullptr };
	QWidget* widget{ nullptr };
	QLabel* label{ nullptr };
	MyIconButton* VoiceCallBtn_{ nullptr };
	MyIconButton* VideoCallBtn_{ nullptr };
	//BubbleHistory* chatWidget{ nullptr };
	ChatListWidget* chatWidget{ nullptr };
	QWidget* widget_2{ nullptr };
	QWidget* ChatMsgToolWidget_{ nullptr };
	MyIconButton* FaceMsgBtn_{ nullptr };          // 表情
	MyIconButton* ScreenShotBtn_{ nullptr };       // 截图
	MyIconButton* FileMsgBtn_{ nullptr };          // 文件
	MyIconButton* ImageMsgBtn_{ nullptr };         // 图片
	MyIconButton* SpeechMsgBtn_{ nullptr };        // 语音
	ChatInputEdit* ChatMsgEdit_{ nullptr };        // 输入框
	QWidget* SendToolWidget_{ nullptr };    // 发送工具
	QPushButton* pushButton{ nullptr };           // 发送按钮
	SmoothScrollBar *scrollBar_{ nullptr };       // 滚动条

private:
	QString userName_;
	QString userId_;

	SQLiteManager& sqlManager_;
	EmojiWindow& emojiWindow_;

	// message window
	std::unordered_map<uint32_t, ChatMessage*> chatMessageMap_{};    // id -> messageWindow
	std::unordered_map<uint32_t, bool> messageIsSaveToDb_{};         // id -> isSaveToDb
	std::unordered_map<uint32_t, Message> messageCache_{};           // id -> message
	static uint32_t messageId_;											 // message id

private slots:
	void onSendBtnClicked();          // 发送按钮点击槽函数
	void onImageMsgBtnClicked();      // 图片按钮点击槽函数
	void onEmojiMsgBtnClicked();      // 表情按钮点击槽函数
	void onFileMsgBtnClicked();       // 文件按钮点击槽函数

	void onListWidgetItemClicked(QListWidgetItem* item);  // 聊天记录点击槽函数

public slots:
	// 表情
	void onEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, const QString& userId);  // 表情符号选择
};


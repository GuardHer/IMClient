#include "FriendChatWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QDebug>
#include "widget/MyWidgets/MyIconButton.h"
#include "chatMessage.h"
#include "ChatListWidget.h"

#include "Message.h"
#include "base/Singleton.h"
#include "base/Logging.h"
#include "IMClient.h"
#include "FileSession.h"

uint32_t FriendChatWidget::messageId_ = 1001;


FriendChatWidget::FriendChatWidget(const QString& userName, const QString& userId, QWidget* parent)
	: QWidget(parent), userName_(userName), userId_(userId), sqlManager_(Singleton<SQLiteManager>::instance())
	, emojiWindow_(Singleton<EmojiWindow>::instance())
{
	// init map
	chatMessageMap_.clear();
	messageIsSaveToDb_.clear();
	messageCache_.clear();
	if (userName_.isEmpty() && !userId_.isEmpty())
		userName_ = userId_;
	this->setMinimumWidth(755);
	//if (!userId_.isEmpty())
	//	friendInfo_ = new Friend(userId_, userName_);

	init();
	initConnect();
	initChatMessage();
	//testBubble();
}

FriendChatWidget::~FriendChatWidget()
{
	// delete
	//delete FriendChatPage_;
	delete layoutWidget3;
	delete verticalLayout_3;
	delete widget;
	delete label;
	delete VoiceCallBtn_;
	delete VideoCallBtn_;
	delete chatWidget;
	delete widget_2;
	delete ChatMsgToolWidget_;
	delete FaceMsgBtn_;
	delete ScreenShotBtn_;
	delete FileMsgBtn_;
	delete ImageMsgBtn_;
	delete SpeechMsgBtn_;
	delete ChatMsgEdit_;
	delete SendToolWidget_;
	delete pushButton;

	// delete Friend
	//if (friendInfo_ != nullptr)
	//{
	//	delete friendInfo_;
	//	friendInfo_ = nullptr;
	//}

}

void FriendChatWidget::initChatMessage()
{
	// 获取聊天记录
	// <<msg, type>, IsSender>
	//QVector<QPair<QPair<QString, int>, int>> msg = sqlMsg.getFriendMessagesText(userId_.toUtf8());

	auto resFuture = sqlManager_.messageExecute(&SQLiteMessage::getFriendMessagesText, Singleton<SQLiteMessage>::instance(), userId_.toUtf8());
	auto msg = resFuture.get();

	if (msg.empty()) return;
	// 解析消息并且显示
	for (auto& it : msg)
	{
		if (it.first.second == MessageType::Text)
		{
			SendMessageText(it.first.first, it.second, false, true);
		}
		else if (it.first.second == MessageType::Image)
		{
			SendMessageImage(it.first.first, it.second, false, true);
		}
		else if (it.first.second == MessageType::File)
		{
			SendMessageFile(it.first.first, it.second, false, true);
		}
	}
}

void FriendChatWidget::onRecvMessage(const std::string& from, int type, const QByteArray& data)
{
	LOG_DEBUG << "FriendChatWidget::onRecvMessage " << type << ", from: " << from;

	switch (type)
	{
		case MessageType::Text:
			SendMessageText(data, 0, false, true);
			saveMessageToDB(QString::fromStdString(from), false, MessageType::Text, QString::fromUtf8(data));
			break;
		case MessageType::Image:
			SendMessageImage(data, 0, false, true);
			saveMessageToDB(QString::fromStdString(from), false, MessageType::Image, QString::fromUtf8(data));
			break;
		default:
			break;
	}
}

void FriendChatWidget::init()
{
	//FriendChatPage_ = new QWidget();

	//FriendChatPage_->setObjectName(QString::fromUtf8("FriendChatPage_"));
	this->setStyleSheet(QString::fromUtf8("background-color: rgb(242, 242, 242);"));
	layoutWidget3 = new QWidget(this);
	//layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
	layoutWidget3->setGeometry(QRect(1, 1, 757, 816));
	verticalLayout_3 = new QVBoxLayout(layoutWidget3);
	verticalLayout_3->setSpacing(0);
	//verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
	verticalLayout_3->setContentsMargins(0, 0, 0, 0);
	widget = new QWidget(layoutWidget3);
	//widget->setObjectName(QString::fromUtf8("widget"));
	widget->setMinimumSize(QSize(755, 80));
	widget->setStyleSheet(QString::fromUtf8(""));
	label = new QLabel(widget);
	//label->setObjectName(QString::fromUtf8("label"));
	label->setGeometry(QRect(20, 20, 311, 41));
	QFont font;
	font.setPointSize(12);
	label->setFont(font);
	label->setAlignment(Qt::AlignCenter);
	label->setText(QString(userName_));
	label->repaint();
	//qDebug() << label->text();

	VoiceCallBtn_ = new MyIconButton(widget);
	//VoiceCallBtn_->setObjectName(QString::fromUtf8("VoiceCallBtn_"));
	VoiceCallBtn_->setGeometry(QRect(590, 30, 40, 40));
	QIcon icon4;
	icon4.addFile(QString::fromUtf8(":/res/QQIcon/voiceCallNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	VoiceCallBtn_->setIcon(icon4);
	VoiceCallBtn_->setIconSize(QSize(32, 32));
	VoiceCallBtn_->setFlat(true);
	VideoCallBtn_ = new MyIconButton(widget);
	//VideoCallBtn_->setObjectName(QString::fromUtf8("VideoCallBtn_"));
	VideoCallBtn_->setGeometry(QRect(660, 30, 40, 40));
	QIcon icon5;
	icon5.addFile(QString::fromUtf8(":/res/QQIcon/videoCallNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	VideoCallBtn_->setIcon(icon5);
	VideoCallBtn_->setIconSize(QSize(32, 32));
	VideoCallBtn_->setFlat(true);

	verticalLayout_3->addWidget(widget);

	//chatWidget = new BubbleHistory(layoutWidget3);
	chatWidget = new ChatListWidget(layoutWidget3);
	//chatWidget->setObjectName(QString::fromUtf8("chatWidget"));
	chatWidget->setMinimumSize(QSize(750, 500));
	chatWidget->setMaximumSize(QSize(16777215, 16777215));
	//chatWidget->setStyleSheet(QString::fromUtf8("border-top: 2px solid ;\n"
	//	"border-bottom: 1px solid #C0C0C0;\n"
	//	"border-right: 2px solid ;\n"
	//	"\n"
	//	"border-top-color: rgb(233, 233, 233);\n"
	//	"border-bottom-color: rgb(233, 233, 233);\n"
	//	"border-right-color: rgb(233, 233, 233);\n"
	//	"\n"
	//	"border-left: none;"));

	chatWidget->setStyleSheet("border:none;border-bottom:1px solid #C0C0C0;");
	verticalLayout_3->addWidget(chatWidget);

	widget_2 = new QWidget(layoutWidget3);
	//widget_2->setObjectName(QString::fromUtf8("widget_2"));
	widget_2->setMinimumSize(QSize(755, 220));
	ChatMsgToolWidget_ = new QWidget(widget_2);
	//ChatMsgToolWidget_->setObjectName(QString::fromUtf8("ChatMsgToolWidget_"));
	ChatMsgToolWidget_->setGeometry(QRect(0, 0, 755, 40));
	ChatMsgToolWidget_->setMinimumSize(QSize(755, 40));
	ChatMsgToolWidget_->setStyleSheet(QString::fromUtf8(""));
	FaceMsgBtn_ = new MyIconButton(ChatMsgToolWidget_);
	//FaceMsgBtn_->setObjectName(QString::fromUtf8("FaceMsgBtn_"));
	FaceMsgBtn_->setGeometry(QRect(30, 0, 40, 40));
	QIcon icon6;
	icon6.addFile(QString::fromUtf8(":/res/QQIcon/faceNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	FaceMsgBtn_->setIcon(icon6);
	FaceMsgBtn_->setIconSize(QSize(32, 32));
	FaceMsgBtn_->setFlat(true);
	ScreenShotBtn_ = new MyIconButton(ChatMsgToolWidget_);
	//ScreenShotBtn_->setObjectName(QString::fromUtf8("ScreenShotBtn_"));
	ScreenShotBtn_->setGeometry(QRect(90, 0, 40, 40));
	QIcon icon7;
	icon7.addFile(QString::fromUtf8(":/res/QQIcon/screenshotNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	ScreenShotBtn_->setIcon(icon7);
	ScreenShotBtn_->setIconSize(QSize(24, 24));
	ScreenShotBtn_->setFlat(true);
	FileMsgBtn_ = new MyIconButton(ChatMsgToolWidget_);
	//FileMsgBtn_->setObjectName(QString::fromUtf8("FileMsgBtn_"));
	FileMsgBtn_->setGeometry(QRect(150, 0, 40, 40));
	QIcon icon8;
	icon8.addFile(QString::fromUtf8(":/res/QQIcon/folderNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	FileMsgBtn_->setIcon(icon8);
	FileMsgBtn_->setIconSize(QSize(32, 32));
	FileMsgBtn_->setFlat(true);
	ImageMsgBtn_ = new MyIconButton(ChatMsgToolWidget_);
	//ImageMsgBtn_->setObjectName(QString::fromUtf8("ImageMsgBtn_"));
	ImageMsgBtn_->setGeometry(QRect(220, 0, 40, 40));
	QIcon icon9;
	icon9.addFile(QString::fromUtf8(":/res/QQIcon/imageNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	ImageMsgBtn_->setIcon(icon9);
	ImageMsgBtn_->setIconSize(QSize(32, 32));
	ImageMsgBtn_->setFlat(true);
	SpeechMsgBtn_ = new MyIconButton(ChatMsgToolWidget_);
	//SpeechMsgBtn_->setObjectName(QString::fromUtf8("SpeechMsgBtn_"));
	SpeechMsgBtn_->setGeometry(QRect(290, 0, 40, 40));
	QIcon icon10;
	icon10.addFile(QString::fromUtf8(":/res/QQIcon/voiceNormal.png"), QSize(), QIcon::Normal, QIcon::Off);
	SpeechMsgBtn_->setIcon(icon10);
	SpeechMsgBtn_->setIconSize(QSize(32, 32));
	SpeechMsgBtn_->setFlat(true);
	ChatMsgEdit_ = new ChatInputEdit(widget_2);
	//ChatMsgEdit_->setObjectName(QString::fromUtf8("ChatMsgEdit_"));
	ChatMsgEdit_->setGeometry(QRect(0, 40, 755, 130));
	ChatMsgEdit_->setMinimumSize(QSize(755, 130));
	ChatMsgEdit_->setStyleSheet(QString::fromUtf8("border: none ;\n"
		""));
	SendToolWidget_ = new QWidget(widget_2);
	//SendToolWidget_->setObjectName(QString::fromUtf8("SendToolWidget_"));
	SendToolWidget_->setGeometry(QRect(0, 170, 755, 45));
	SendToolWidget_->setMinimumSize(QSize(755, 40));
	SendToolWidget_->setStyleSheet(QString::fromUtf8(""));
	pushButton = new QPushButton(SendToolWidget_);
	//pushButton->setObjectName(QString::fromUtf8("pushButton"));
	pushButton->setGeometry(QRect(650, 0, 71, 28));

	verticalLayout_3->addWidget(widget_2);

	//ChatStackedWidget_->addWidget(FriendChatPage_);

	FaceMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/faceNormal.png"));
	FaceMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/faceSelected.png"));
	FaceMsgBtn_->setStyleSheet("border:none;");

	ScreenShotBtn_->setIconLeave(QIcon(":/res/QQIcon/screenshotNormal.png"));
	ScreenShotBtn_->setIconEnter(QIcon(":/res/QQIcon/screenshotSelected.png"));
	ScreenShotBtn_->setStyleSheet("border:none;");

	FileMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/folderNormal.png"));
	FileMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/folderSelected.png"));
	FileMsgBtn_->setStyleSheet("border:none;");

	ImageMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/imageNormal.png"));
	ImageMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/imageSelect.png"));
	ImageMsgBtn_->setStyleSheet("border:none;");

	SpeechMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/voiceNormal.png"));
	SpeechMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/voiceSelected.png"));
	SpeechMsgBtn_->setStyleSheet("border:none;");

	VoiceCallBtn_->setIconLeave(QIcon(":/res/QQIcon/voiceCallNormal.png"));
	VoiceCallBtn_->setIconEnter(QIcon(":/res/QQIcon/voiceCallSelected.png"));
	VoiceCallBtn_->setStyleSheet("border:none;");

	VideoCallBtn_->setIconLeave(QIcon(":/res/QQIcon/videoCallNormal.png"));
	VideoCallBtn_->setIconEnter(QIcon(":/res/QQIcon/videoCallSelected.png"));
	VideoCallBtn_->setStyleSheet("border:none;");
}

void FriendChatWidget::initConnect()
{
	// Send button
	connect(pushButton, &QPushButton::clicked, this, &FriendChatWidget::onSendBtnClicked);

	// image button
	connect(ImageMsgBtn_, &MyIconButton::clicked, this, &FriendChatWidget::onImageMsgBtnClicked);
	connect(FaceMsgBtn_, &MyIconButton::clicked, this, &FriendChatWidget::onEmojiMsgBtnClicked);
	// file
	connect(FileMsgBtn_, &MyIconButton::clicked, this, &FriendChatWidget::onFileMsgBtnClicked);

	// listWidgetItemClicked
	connect(chatWidget, &ChatListWidget::itemClicked, this, &FriendChatWidget::onListWidgetItemClicked);
}

void FriendChatWidget::testBubble()
{
}

void FriendChatWidget::initEmoji()
{

}

bool FriendChatWidget::SendMessageText(const QString& msg, int IsSender, bool isSending, bool fromDb)
{
	messageId_++;
	QColor color = QColor(0, 153, 255);
	ChatMessage * messageW = new ChatMessage(userId_, color, 0, messageId_, chatWidget);
	//std::shared_ptr<ChatMessage> messageW = std::make_shared<ChatMessage>(userId_, color, chatWidget, 0);
	messageW->setFixedWidth(755);

	QSize size = messageW->fontRect(msg);

	QListWidgetItem * item = new QListWidgetItem(chatWidget);
	item->setSizeHint(size);
	if (IsSender)
		messageW->setText(isSending, msg, "12:50", size, ChatMessage::User_Me);
	else
		messageW->setText(isSending, msg, "12:50", size, ChatMessage::User_She);
	chatWidget->setItemWidget(item, messageW);
	// 将焦点移动到最后一个item
	chatWidget->scrollToBottom();

	if (!fromDb)
	{
		chatMessageMap_.insert(std::make_pair(messageId_, messageW));
		messageIsSaveToDb_.insert(std::make_pair(messageId_, false));
	}

	return true;
}

bool FriendChatWidget::SendMessageText(const QByteArray& msg, int IsSender, bool isSending, bool fromDb)
{
	// msg是二进制数据 需要解析 二进制数据转换为QString
	QString str = QString::fromUtf8(msg);
	SendMessageText(str, IsSender, isSending, fromDb);
	
	return true;
}

bool FriendChatWidget::SendMessageImage(const QString& imagePath, int IsSender, bool isSending, bool fromDb)
{
	messageId_++;
	QColor color = QColor(0, 153, 255);
	ChatMessage* messageW = new ChatMessage(userId_, color, 1, messageId_, chatWidget);
	messageW->setFixedWidth(755);

	QListWidgetItem* item = new QListWidgetItem(chatWidget);
	QPixmap pixmap(imagePath);
	item->setSizeHint(QSize(631, 200));
	messageW->setImageLabel(pixmap, "12:50", ChatMessage::User_Me);
	chatWidget->setItemWidget(item, messageW);
	// 将焦点移动到最后一个item
	chatWidget->scrollToBottom();

	return true;
}

bool FriendChatWidget::SendMessageImage(const QByteArray& image, int IsSender, bool isSending, bool fromDb)
{
	messageId_++;
	QColor color = QColor(0, 153, 255);
	ChatMessage* messageW = new ChatMessage(userId_, color, 1, messageId_, chatWidget);
	messageW->setFixedWidth(755);

	QListWidgetItem* item = new QListWidgetItem(chatWidget);
	QPixmap pixmap;
	pixmap.loadFromData(image);
	// 判断图片是否为空
	if (pixmap.isNull())
	{
		LOG_ERROR << ("load image failed");
		return false;
	}

	item->setSizeHint(QSize(631, 200));
	messageW->setImageLabel(pixmap, "12:50", ChatMessage::User_Me);
	chatWidget->setItemWidget(item, messageW);

	// 将焦点移动到最后一个item
	chatWidget->scrollToBottom();
	return true;
}

bool FriendChatWidget::SendMessageFile(const QString& filePath, int IsSender, bool isSending, bool fromDb)
{
	messageId_++;
	// 获取文件名，文件类型，文件大小
	QString fileName = filePath.right(filePath.size() - filePath.lastIndexOf('/') - 1);
	QString fileType = fileName.right(fileName.size() - fileName.lastIndexOf('.') - 1);
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		LOG_ERROR << ("open file failed");
		return false;
	}
	QByteArray fileData = file.readAll();
	file.close();

	// 用来测量文件框的大小
	QString files =	"ChatMessage* messageW = ne\n"
					"ChatMessage* messageW = ne\n"
					"ChatMessage* messageW = ne";

	// 文件发送
	QColor color = QColor(0, 153, 255);
	ChatMessage* messageW = new ChatMessage(userId_, color, 3, messageId_, chatWidget);
	messageW->setFixedWidth(755);
	QSize size = messageW->fontRect(files);

	QListWidgetItem* item = new QListWidgetItem(chatWidget);
	item->setSizeHint(size);
	messageW->setFile(QPixmap(QString(":/res/fileIcon/%1.png").arg(fileType)), fileName, fileData.size());
	if (IsSender)
		messageW->setText(isSending, files, "12:50", size, ChatMessage::User_Me);
	else
		messageW->setText(isSending, files, "12:50", size, ChatMessage::User_She);
	chatWidget->setItemWidget(item, messageW);
	// 将焦点移动到最后一个item
	chatWidget->scrollToBottom();

	if (!fromDb)
	{
		chatMessageMap_.insert(std::make_pair(messageId_, messageW));
		messageIsSaveToDb_.insert(std::make_pair(messageId_, false));
		addMessageToCache(userId_, true, MessageType::File, filePath);
		// upload file
		QString from = Singleton<Self>::instance().getSelfId();
		FileSession::uploadFile(filePath.toStdString(), from.toStdString(), userId_.toStdString(), messageId_);
	}

	return true;
}

std::string FriendChatWidget::formatEmoji(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, const Format fmt, const std::string& cpPrefix, const bool noNl)
{
	std::string output;
	int cnt = 0;

	switch (fmt) {
	case Format::UTF8:
	{
		if (skinTone && emoji.hasSkinToneSupport()) {
			output = emoji.strWithSkinTone(*skinTone);
		}
		else {
			output = emoji.str();
		}

		break;
	}

	case Format::CODEPOINTS_HEX:
	{
		Emoji::Codepoints codepoints;


		if (skinTone && emoji.hasSkinToneSupport()) {
			codepoints = emoji.codepointsWithSkinTone(*skinTone);
		}
		else {
			codepoints = emoji.codepoints();
		}
		cnt = codepoints.size();

		for (const auto codepoint : codepoints) {
			std::array<char, 32> buf{};

			std::sprintf(buf.data(), "%s%x ", cpPrefix.c_str(), static_cast<unsigned int>(codepoint));
			output += buf.data();
		}

		// remove trailing space
		output.resize(output.size() - 1);
		break;
	}
	}

	if (!noNl) {
		output += '\n';
	}

	return output;
}

void FriendChatWidget::saveMessageToDB(const Message& message)
{
	// 本地数据库添加缓存
	sqlManager_.messageExecute(&SQLiteMessage::addMessageCache, Singleton<SQLiteMessage>::instance(), message);
}

void FriendChatWidget::saveMessageToDB(const QString& talker, bool isSender, MessageType::Type type, const QString& content)
{
	Message message(talker, isSender, type);
	message.setIsFriend(true);
	message.setContent(content);
	message.setByteContent(content.toUtf8());
	message.setTime(QDateTime::currentDateTime());

	sqlManager_.messageExecute(&SQLiteMessage::addMessageCache, Singleton<SQLiteMessage>::instance(), message);
}

void FriendChatWidget::addMessageToCache(const QString& talker, bool isSender, MessageType::Type type, const QString& content)
{

	Message message(talker, isSender, type);
	message.setIsFriend(true);
	message.setContent(content);
	message.setByteContent(content.toUtf8());
	message.setTime(QDateTime::currentDateTime());

	messageCache_.insert(std::make_pair(messageId_, message));
}

void FriendChatWidget::sendMessageToServer(const QByteArray& content)
{

	ByteArray byteArray;
	std::string from = Singleton<Self>::instance().getSelfId().toStdString();
	std::string to = userId_.toStdString();
	Singleton<ClientSession>::instance().packChat(&byteArray, from, to, MessageType::Text, content, messageId_);
	IMClient& cli = Singleton<IMClient>::instance();
	if (!cli.isConnect()) return;
	cli.sendMessage(&byteArray);
}


void FriendChatWidget::onSendMessageResponse(uint32_t messageId)
{

	auto it = chatMessageMap_.find(messageId);
	if (it != chatMessageMap_.end())
	{
		messageIsSaveToDb_[messageId] = true;
		// 保存到本地数据库, 删除缓存
		if (messageCache_.find(messageId) != messageCache_.end())
		{
			saveMessageToDB(messageCache_[messageId]);  // 保存到数据库
			messageCache_.erase(messageId);             // 删除缓存
		}
		// 设置消息发送成功
		it->second->setTextSuccess(true);

		// 从chatMessageMap_中移除, 但是不删除对象
		chatMessageMap_.erase(it);
	}

	

}

void FriendChatWidget::onUploadResponse(uint32_t messageId)
{
	auto it = chatMessageMap_.find(messageId);
	if (it != chatMessageMap_.end())
	{
		messageIsSaveToDb_[messageId] = true;
		// 保存到本地数据库, 删除缓存
		if (messageCache_.find(messageId) != messageCache_.end())
		{
			saveMessageToDB(messageCache_[messageId]);  // 保存到数据库
			messageCache_.erase(messageId);             // 删除缓存
		}
		// 设置消息发送成功
		it->second->setTextSuccess(true);

		// 从chatMessageMap_中移除, 但是不删除对象
		chatMessageMap_.erase(it);
	}
}

void FriendChatWidget::onDownloadResponse(uint32_t messageId)
{
	LOG_DEBUG << "FriendChatWidget::onDownloadResponse " << messageId;
	auto it = chatMessageMap_.find(messageId);
	if (it != chatMessageMap_.end())
	{
		// 设置消息发送成功
		it->second->setTextSuccess(true);

		// 从chatMessageMap_中移除, 但是不删除对象
		chatMessageMap_.erase(it);
	}
}



/*======================= slots ==============================*/
void FriendChatWidget::onSendBtnClicked()
{
	QString msg = ChatMsgEdit_->toPlainText();  // 获取文本框内容
	if (msg.isEmpty())  
	{
		return;
	}
	bool res = SendMessageText(msg, 1, true, false);
	if (res) {
		// 先将消息发送到服务器
		this->sendMessageToServer(msg.toUtf8());

		// 添加缓存
		this->addMessageToCache(userId_, true, MessageType::Text, msg);
	}

	// 清空文本框
	ChatMsgEdit_->clear();
	// 重新聚焦
	ChatMsgEdit_->setFocus();
}

void FriendChatWidget::onImageMsgBtnClicked()
{

	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), " ",  tr("Image Files (*.png *.jpg *.bmp)"));
	if (filePath.isEmpty())
	{
		return;
	}

	// file path； IsSender； isSending； fromDb
	SendMessageImage(filePath, 1, true, false);
}

void FriendChatWidget::onEmojiMsgBtnClicked()
{
	if (!emojiWindow_.isVisible())
	{
		// 显示表情窗口在EmojiMsgBtn_的上方
		emojiWindow_.show();
		emojiWindow_.move(FaceMsgBtn_->mapToGlobal(QPoint(0, 0)).x(), FaceMsgBtn_->mapToGlobal(QPoint(0, 0)).y() - emojiWindow_.height());
	}
	else
	{
		emojiWindow_.hide();
	}
}

void FriendChatWidget::onFileMsgBtnClicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), " ", tr("All Files (*)"));
	if (filePath.isEmpty())
	{
		return;
	}

	// file path； IsSender； isSending； fromDb
	SendMessageFile(filePath, 1, true, false);
}


void FriendChatWidget::onEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone, const QString& userId)
{
	if (userId != userId_)
	{
		return;
	}
	const auto fmt = Format::CODEPOINTS_HEX;
	const auto cpPrefix = "";
	const auto output = formatEmoji(emoji, skinTone, fmt, cpPrefix, true);

	size_t pos;
	char32_t codepoint = std::stoul(output, &pos, 16);
	// 创建 char32_t 数组
	char32_t arr[] = { codepoint, 0x0 };

	std::u32string u32str;
	u32str.push_back(codepoint);


	ChatMsgEdit_->insertPlainText(QString::fromStdU32String(u32str));
	ChatMsgEdit_->setFocus();
}

void FriendChatWidget::onListWidgetItemClicked(QListWidgetItem* item)
{
	// 获取点击的item
	ChatMessage* chatMessage = dynamic_cast<ChatMessage*>(chatWidget->itemWidget(item));
	if (chatMessage == nullptr)	return;

	// 获取点击的item的消息type
	int type = chatMessage->type();
	int userType = chatMessage->userType(); // 1: me; 0: she

	// 如果是文件消息, 则下载文件
	if (type == 3)
	{
		FileInfo fileInfo = chatMessage->getFileInfo();
		uint32_t messageId = chatMessage->getMessageId();
		std::string from;
		std::string to;
		if (userType == 1) {   // me
			from = Singleton<Self>::instance().getSelfId().toStdString();
			to = userId_.toStdString();
		}
		else if (userType == 0) {   // she
			from = userId_.toStdString();
			to = Singleton<Self>::instance().getSelfId().toStdString();	
		}

		FileSession::downloadFile(fileInfo.fileName.toStdString(), from, to, messageId);
		chatMessage->setTextSending(true);
		chatMessageMap_.insert(std::make_pair(messageId, chatMessage));
	}
}


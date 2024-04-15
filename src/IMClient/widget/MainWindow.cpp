#include "MainWindow.h"
#include <QGraphicsOpacityEffect> // 透明度效果
#include <QPalette>
#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QPainterPath>
#include <QPainter>
#include <QListWidgetItem>
#include <QListWidget>
#include <QScrollBar>
#include <QStackedWidget>

#include "../base/Logging.h"
 
#include "../ClientSession.h"
#include "../SmoothScrollBar.h"
#include "MyWidgets/MyIconButton.h"
#include "../UserList.h"
#include "../Group.h"
#include "../GroupList.h"
#include "../Self.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, sqlManager_(Singleton<SQLiteManager>::instance())
{
	ui->setupUi(this);	

	// 设置窗口大小
	this->setFixedSize(1200, 800);

	InitStyle();
}

MainWindow::~MainWindow()
{
	// 释放内存
	delete ui;

	if (friendItems_.size() > 0)
	{
		for (auto& friendItem : friendItems_)
		{
			delete friendItem;
		}
		friendItems_.clear();
	}

	if (chatPages_.size() > 0)
	{
		for (auto& chatPage : chatPages_)
		{
			delete chatPage.second;
		}
		chatPages_.clear();
	}

	if (groupItems_.size() > 0)
	{
		for (auto& groupItem : groupItems_)
		{
			delete groupItem;
		}
		groupItems_.clear();
	}

	//onCanceled();

	UserList::clear();
	GroupList::clear();
}

bool MainWindow::init()
{
	InitBtn();
	InitConnect();
	InitEmoji();
	loadFriends();
	loadGroups();

	// 添加聊天窗口
	for (auto& chatPage : chatPages_)
	{
		ui->ChatStackedWidget_->addWidget(chatPage.second);
	}


	return true;
}

void MainWindow::InitBtn()
{ 
	// 设置按钮图标
	//ui->ChatBtn_->setIconLeave(QIcon(":/res/QQIcon/chatNormal.png"));
	ui->ChatBtn_->setIcon(QIcon(":/res/QQIcon/chatSelected.png"));
	ui->ChatBtn_->setStyleSheet("border:none;");
	ui->ChatBtn_->setChecked(true);

	//ui->FriendBtn_->setIconLeave(QIcon(":/res/QQIcon/friendNormal.png"));
	//ui->FriendBtn_->setIconEnter(QIcon(":/res/QQIcon/friendSelected.png"));
	ui->FriendBtn_->setStyleSheet("border:none;");

	ui->EmojiMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/faceNormal.png"));
	ui->EmojiMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/faceSelected.png"));
	ui->EmojiMsgBtn_->setStyleSheet("border:none;");

	ui->ScreenShotBtn_->setIconLeave(QIcon(":/res/QQIcon/screenshotNormal.png"));
	ui->ScreenShotBtn_->setIconEnter(QIcon(":/res/QQIcon/screenshotSelected.png"));
	ui->ScreenShotBtn_->setStyleSheet("border:none;");

	ui->FileMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/folderNormal.png"));
	ui->FileMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/folderSelected.png"));
	ui->FileMsgBtn_->setStyleSheet("border:none;");

	ui->ImageMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/imageNormal.png"));
	ui->ImageMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/imageSelect.png"));
	ui->ImageMsgBtn_->setStyleSheet("border:none;");

	ui->SpeechMsgBtn_->setIconLeave(QIcon(":/res/QQIcon/voiceNormal.png"));
	ui->SpeechMsgBtn_->setIconEnter(QIcon(":/res/QQIcon/voiceSelected.png"));
	ui->SpeechMsgBtn_->setStyleSheet("border:none;");

	ui->VoiceCallBtn_->setIconLeave(QIcon(":/res/QQIcon/voiceCallNormal.png"));
	ui->VoiceCallBtn_->setIconEnter(QIcon(":/res/QQIcon/voiceCallSelected.png"));
	ui->VoiceCallBtn_->setStyleSheet("border:none;");

	ui->VideoCallBtn_->setIconLeave(QIcon(":/res/QQIcon/videoCallNormal.png"));
	ui->VideoCallBtn_->setIconEnter(QIcon(":/res/QQIcon/videoCallSelected.png"));
	ui->VideoCallBtn_->setStyleSheet("border:none;");

	connectBtn();
}

void MainWindow::connectBtn()
{
	//
	QObject::connect(ui->ChatBtn_, &QPushButton::clicked, this, &MainWindow::onChatBtnClicked);
	QObject::connect(ui->FriendBtn_, &QPushButton::clicked, this, &MainWindow::onFriendBtnClicked);
}

void MainWindow::addFriendItem(QString userName, QString userId, QString head, QString lastMsg)
{
	ui->ChatListWidget_->setIconSize(QSize(50, 50)); // 设置图标的大小
	ui->FriendWid_->setIconSize(QSize(50, 50)); // 设置图标的大小

	QListWidgetItem* item = new QListWidgetItem(ui->ChatListWidget_);
	QListWidgetItem* item2 = new QListWidgetItem(ui->FriendWid_);
	item->setData(Qt::UserRole, userId); // 设置item的data, 用于获取item的userId
	item2->setData(Qt::UserRole, userId); // 设置item的data, 用于获取item的userId
	item->setSizeHint(QSize(0, 90)); // 设置item的大小
	item2->setSizeHint(QSize(0, 90)); // 设置item的大小
	FriendItem * friendItemChat = new FriendItem();
	friendItemChat->setHeadImage(head);
	friendItemChat->setName(userName);
	friendItemChat->setUserId(userId);
	friendItemChat->setMsg(lastMsg);
	friendItemChat->setTime("14:52");

	FriendItem* friendItemFriendWid = new FriendItem();  // 创建另一个 FriendItem 对象
	friendItemFriendWid->setHeadImage(head);
	friendItemFriendWid->setName(userName);
	friendItemFriendWid->setUserId(userId);
	friendItemFriendWid->setMsg(lastMsg);
	friendItemFriendWid->setTime("14:52");

	ui->ChatListWidget_->setItemWidget(item, friendItemChat);  // 设置item的widget
	ui->FriendWid_->setItemWidget(item2, friendItemFriendWid);  // 设置item的widget

	friendItems_.append(friendItemChat);
	
}

void MainWindow::addGroupItem(QString groupName, QString groupId, QString head, QString lastMsg)
{
	ui->GroupWid_->setIconSize(QSize(50, 50)); // 设置图标的大小

	QListWidgetItem* item = new QListWidgetItem(ui->ChatListWidget_);
	QListWidgetItem* item2 = new QListWidgetItem(ui->GroupWid_);
	item->setData(Qt::UserRole, groupId); // 设置item的data, 用于获取item的groupId
	item2->setData(Qt::UserRole, groupId); // 设置item的data, 用于获取item的groupId
	item->setSizeHint(QSize(0, 90)); // 设置item的大小
	item2->setSizeHint(QSize(0, 90)); // 设置item的大小

	FriendItem* groupItem = new FriendItem();
	groupItem->setHeadImage(head);
	groupItem->setName(groupName);
	groupItem->setUserId(groupId);
	groupItem->setMsg(lastMsg);
	groupItem->setTime("14:52");

	FriendItem* groupItem2 = new FriendItem();
	groupItem2->setHeadImage(head);
	groupItem2->setName(groupName);
	groupItem2->setUserId(groupId);
	groupItem2->setMsg(lastMsg);
	groupItem2->setTime("14:52");

	ui->ChatListWidget_->setItemWidget(item, groupItem);  // 设置item的widget
	ui->GroupWid_->setItemWidget(item2, groupItem2);  // 设置item的widget

	groupItems_.append(groupItem);
}

void MainWindow::loadFriendList()
{

	//bool flag = sqlContact_.getAllFriendInfo();
	auto futureResult = sqlManager_.contactExecute(&SQLiteContact::getAllFriendInfo, Singleton<SQLiteContact>::instance());
	auto flag = futureResult.get();

	if (flag) {
		// 如果存在就从本地获取好友信息
		Self &self = Singleton<Self>::instance();
		self.setSelf(UserList::findFriend(userId_));
		LOG_DEBUG << "success getAllFriendInfo";
		
	}
	else {
		// 如果不存在就从服务区获取, 并且在本地添加 : TODO
		
	}

	// 判断UserList是否为空
	if (!UserList::isEmpty())
	{
		for (auto& m_friend : UserList::getAllFriends())
		{
			FriendChatWidget* widget = new FriendChatWidget(m_friend->getUserName(), m_friend->getUserId());
			widget->setObjectName(m_friend->getUserId());
			chatPages_.insert(std::make_pair(m_friend->getUserId(), widget));
			connect(this, &MainWindow::emitEmojiChosen, widget, &FriendChatWidget::onEmojiChosen);
		}
	}
	else {
		LOG_DEBUG << ("UserList is empty!");
	}
}

void MainWindow::loadGroupList()
{
	auto futureResult = sqlManager_.contactExecute(&SQLiteContact::getAllGroupInfo, Singleton<SQLiteContact>::instance());
	auto flag = futureResult.get();

	if (flag) {
		// 如果存在就从本地获取群组信息
		LOG_DEBUG << ("success getAllGroupInfo");
	}
	else {
		LOG_DEBUG << ("Failure to getAllGroupInfo from database!");
		// 如果不存在就从服务区获取, 并且在本地添加

		if (GroupList::isEmpty())
		{
			LOG_DEBUG << ("Failure to get a server group information!");
			return;
		}
		else {
			// TODO: 在本地数据库中插入群组信息
		}
	}

	// 判断groupsVec_是否为空
	if (!GroupList::isEmpty())
	{
		for (auto& m_group : GroupList::getAllGroups())
		{
			FriendChatWidget* widget = new FriendChatWidget(m_group->getGroupName(), m_group->getGroupId());
			widget->setObjectName(m_group->getGroupId());
			chatPages_.insert(std::make_pair(m_group->getGroupId(), widget));

		}
	}
	else {
		LOG_DEBUG << ("GroupList is empty!");
	}
}

void MainWindow::setCurrentFriendCard(const QString& userId)
{
	User* friend_ = UserList::findFriend(userId);

	if (friend_ == nullptr)
	{
		LOG_DEBUG << ("friend is nullptr!");
		return;
	}

	// 设置好友名片信息
	ui->FriendCard_Head->setImage(QPixmap(friend_->getCustomFace()));  // 设置头像
	ui->FriendCard_Name->setText(friend_->getUserName());              // 设置昵称
	ui->FriendCard_Id->setText("QQ " + friend_->getUserId());          // 设置QQ号
	//ui->FriendCard_Status->setText(friend_->getStatusString());      // 设置状态
	ui->FriendCard_Sex->setText(friend_->getGender());                 // 设置性别
	ui->FriendCard_Birthday->setText(friend_->getBirthday());          // 设置生日
	ui->FriendCard_Address->setText(friend_->getAddress());            // 设置地址
	ui->FriendCard_Grade->setText("等级: 18");                          // 设置等级 
	ui->FriendCard_RemarkText->setText(friend_->getNickName());        // 设置备注
	ui->FriendCard_PacketText->setCurrentText("家人");                  // 设置分组 
	ui->FriendCard_SignatureText->setText(friend_->getSignature());    // 设置个性签名
}

void MainWindow::setCurrentGroupCard(const QString& groupId)
{
	Group* group = GroupList::findGroup(groupId);

	if (group == nullptr)
	{
		LOG_DEBUG << ("group is nullptr!");
		return;
	}

	ui->GroupCard_Head->setImage(QPixmap(group->getCustomFace()));
	ui->GroupCard_Name->setText(group->getGroupName());
	ui->GroupCard_Id->setText("GroupId " + group->getGroupId());

	ui->GroupCard_RemarkText->setText(group->getGroupName());      // 备注
	ui->GroupCard_MyGroupNameText->setText(userName_);            // 我在本群的昵称
	ui->GroupCard_IntroduceText->setText(group->getGroupNotice()); // 群介绍
	ui->GroupCard_NoticeText->setText(group->getGroupNotice());    // 群公告
	ui->GroupCard_GroupMemberText->setText(QString(group->getGroupMemberCount())); // 群成员数量
}

void MainWindow::loadFriends()
{
	loadFriendList();

	// 判断friends_是否为空
	if (UserList::isEmpty())
	{
		return;
	}
	// 添加好友列表item
	for (auto& friend_ : UserList::getAllFriends())
	{
		addFriendItem(friend_->getUserName(), friend_->getUserId(), friend_->getCustomFace(), friend_->getLastMsg());
	}
	
}
void MainWindow::loadGroups()
{
	// 获取群组列表
	loadGroupList();

	// 判断groups_是否为空
	if (GroupList::isEmpty())
	{
		LOG_DEBUG << ("GroupList is empty!");
		return;
	}

	// 添加群组列表item
	for (auto& group : GroupList::getAllGroups())
	{
		addGroupItem(group->getGroupName(), group->getGroupId(), group->getCustomFace(), group->getLastMsg());
	}
}
void MainWindow::InitConnect()
{
	// LoginWidget
	//LoginWidget &loginWidget = LoginWidgetInstance;
	//QObject::connect(&LoginWidgetInstance, &LoginWidget::signalUser, this, &MainWindow::handleUser);

	// 好友列表点击
	QObject::connect(ui->ChatListWidget_, &QListWidget::itemClicked, this, &MainWindow::onFriendChatItemClicked);
	QObject::connect(ui->ChatListWidget_, &QListWidget::itemClicked, this, &MainWindow::onGroupChatItemClicked);
	QObject::connect(ui->FriendWid_, &QListWidget::itemClicked, this, &MainWindow::onFriendCardItemClicked);
	QObject::connect(ui->GroupWid_, &QListWidget::itemClicked, this, &MainWindow::onGroupCardItemClicked);
	QObject::connect(ui->GroupWid_, &QListWidget::itemClicked, this, &MainWindow::onGroupChatItemClicked);
	QObject::connect(ui->FriendWid_, &QListWidget::itemClicked, this, &MainWindow::onFriendCardItemClicked);

	// 按钮点击
	QObject::connect(ui->FriendCard_SendMsgBtn, &QPushButton::clicked, this, &MainWindow::onFriendCardSendBtnClicked);
	QObject::connect(ui->GroupCard_SendMsgBtn, &QPushButton::clicked, this, &MainWindow::onGroupCardSendBtnClicked);
}
void MainWindow::InitStyle()
{
	// 设置滚动条
	ui->ChatListWidget_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 水平滚动条 一直关闭
	ui->FriendWid_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 水平滚动条 一直关闭
	ui->GroupWid_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 水平滚动条 一直关闭

	SmoothScrollBar* vScrollBar = new SmoothScrollBar();
	SmoothScrollBar* vScrollBar2 = new SmoothScrollBar();
	SmoothScrollBar* vScrollBar3 = new SmoothScrollBar();
	ui->ChatListWidget_->setVerticalScrollBar(vScrollBar); // 设置滚动条
	ui->FriendWid_->setVerticalScrollBar(vScrollBar2); // 设置滚动条
	ui->GroupWid_->setVerticalScrollBar(vScrollBar3); // 设置滚动条

	ui->ChatListWidget_->setVerticalScrollMode(QListWidget::ScrollPerPixel);
	ui->FriendWid_->setVerticalScrollMode(QListWidget::ScrollPerPixel);
	ui->GroupWid_->setVerticalScrollMode(QListWidget::ScrollPerPixel);

	ui->stackedWidget->setCurrentIndex(0); // 设置默认页面
	ui->ChatStackedWidget_->setCurrentWidget(ui->NormalPage); // 设置默认页面
	//ui->ChatStackedWidget_->setCurrentWidget(ui->GroupChatPage_); // 设置默认页面

}
void MainWindow::InitEmoji()
{
	auto& emojiDb = Singleton<EmojiDb>::instance();
	emojiDb.init(Config::getEmojiPath(), EmojiDb::EmojiSize::SIZE_32);

	auto& emojiWindow = Singleton<EmojiWindow>::instance();
	emojiWindow.init(emojiDb, false);
	QObject::connect(&emojiWindow, &EmojiWindow::canceled, this, &MainWindow::onCanceled);
	QObject::connect(&emojiWindow, &EmojiWindow::emojiChosen, this, &MainWindow::onEmojiChosen);
}
QPixmap MainWindow::PixmapToRound(QPixmap& src, int radius)
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

void MainWindow::closeEvent(QCloseEvent* event)
{
	emit emitClose();
	event->accept();
}


/*=============================================== slots ===============================================*/

void MainWindow::handleUser(const QString& userId)
{
	userId_ = userId;
}


void MainWindow::onFriendChatItemClicked(QListWidgetItem* item)
{
	// 获取item的userId
	QString userId = item->data(Qt::UserRole).toString();

	// 判断chatPages_是否为空
	if (chatPages_.empty())
	{
		return;
	}

	// 判断chatPages_中是否存在userId
	if (chatPages_.find(userId) == chatPages_.end())
	{
		// 如果不存在就添加
		FriendChatWidget *widget = new FriendChatWidget(UserList::findFriend(userId)->getUserName(), userId);
		//chatPages_.insert(std::make_pair(userId, clonePage(QString::fromStdString(friends_[userId].username_))));
		chatPages_.insert(std::make_pair(userId, widget));
		// 
		LOG_DEBUG << ("addPage:{}", userId.toStdString());
	}

	// 判断当前聊天窗口是否已经打开
	if (ui->ChatStackedWidget_->indexOf(chatPages_[userId]) == -1)
	{
		ui->ChatStackedWidget_->addWidget(chatPages_[userId]);
	}

	ui->ChatStackedWidget_->setCurrentWidget(chatPages_[userId]);
}

void MainWindow::onGroupChatItemClicked(QListWidgetItem* item)
{
	// 获取item的groupId
	QString groupId = item->data(Qt::UserRole).toString();

	// 判断chatPages_是否为空
	if (chatPages_.empty())
	{
		return;
	}

	// 判断chatPages_中是否存在groupId
	if (chatPages_.find(groupId) == chatPages_.end())
	{
		// 如果不存在就添加
		FriendChatWidget* widget = new FriendChatWidget(GroupList::findGroup(groupId)->getGroupName(), groupId);
		chatPages_.insert(std::make_pair(groupId, widget));
		// 
		LOG_DEBUG << ("addPage:{}", groupId.toStdString());
	}

	// 判断当前聊天窗口是否已经打开
	if (ui->ChatStackedWidget_->indexOf(chatPages_[groupId]) == -1)
	{
		ui->ChatStackedWidget_->addWidget(chatPages_[groupId]);
	}

	ui->ChatStackedWidget_->setCurrentWidget(chatPages_[groupId]);
}

void MainWindow::onFriendCardItemClicked(QListWidgetItem* item)
{
	// 获取item的userId
	QString userId = item->data(Qt::UserRole).toString();

	// 根据userId获取好友信息
	setCurrentFriendCard(userId);

	ui->CardStackWidget->setCurrentWidget(ui->FriendCard);
}

void MainWindow::onGroupCardItemClicked(QListWidgetItem* item)
{
	// 获取item的groupId
	QString groupId = item->data(Qt::UserRole).toString();

	// 根据groupId获取群组信息
	setCurrentGroupCard(groupId);

	ui->CardStackWidget->setCurrentWidget(ui->GroupCard);
}

void MainWindow::onChatBtnClicked()
{
	if (!ui->ChatBtn_->isChecked()) {
		ui->ChatBtn_->setChecked(true);
		ui->ChatBtn_->setIcon(QIcon(":/res/QQIcon/chatSelected.png"));
		ui->FriendBtn_->setChecked(false);
		ui->FriendBtn_->setIcon(QIcon(":/res/QQIcon/friendNormal.png"));

		ui->stackedWidget->setCurrentIndex(0);
	}
}

void MainWindow::onFriendBtnClicked()
{
	if (!ui->FriendBtn_->isChecked()) {
		ui->ChatBtn_->setChecked(false);
		ui->ChatBtn_->setIcon(QIcon(":/res/QQIcon/chatNormal.png"));
		ui->FriendBtn_->setChecked(true);
		ui->FriendBtn_->setIcon(QIcon(":/res/QQIcon/friendSelected.png"));

		ui->stackedWidget->setCurrentIndex(1);
	}
	
}

void MainWindow::onSettingBtnClicked()
{
}

void MainWindow::onFriendCardSendBtnClicked()
{
	QString friendId = ui->FriendCard_Id->text().remove("QQ ");
	onChatBtnClicked();
	ui->ChatStackedWidget_->setCurrentWidget(chatPages_[friendId]);
}

void MainWindow::onGroupCardSendBtnClicked()
{
	QString groupId = ui->GroupCard_Id->text().remove("GroupId ");
	onChatBtnClicked();
	ui->ChatStackedWidget_->setCurrentWidget(chatPages_[groupId]);
}

void MainWindow::onEmojiChosen(const Emoji& emoji, const std::optional<Emoji::SkinTone>& skinTone)
{
	// 获取当前聊天窗口 userId or groupId
	QString userId = ui->ChatStackedWidget_->currentWidget()->objectName();
	emit emitEmojiChosen(emoji, skinTone, userId);
}

void MainWindow::onCanceled()
{
	Singleton<EmojiWindow>::instance().close();
	Singleton<EmojiWindow>::instance().deleteLater(); // 释放内存
}

void MainWindow::onRecvMessage(const std::string& from, int type, const QByteArray& data)
{
	// 判断chatPages_是否为空
	if (chatPages_.empty())
	{
		return;
	}

	// 判断chatPages_中是否存在from
	if (chatPages_.find(QString::fromStdString(from)) == chatPages_.end())
	{
		// 如果不存在就添加
		FriendChatWidget* widget = new FriendChatWidget(UserList::findFriend(QString::fromStdString(from))->getUserName(), QString::fromStdString(from));
		chatPages_.insert(std::make_pair(QString::fromStdString(from), widget));
		// 
		LOG_DEBUG << ("addPage:{}", from);
	}

	// 判断当前聊天窗口是否已经打开
	if (ui->ChatStackedWidget_->indexOf(chatPages_[QString::fromStdString(from)]) == -1)
	{
		ui->ChatStackedWidget_->addWidget(chatPages_[QString::fromStdString(from)]);
	}

	//ui->ChatStackedWidget_->setCurrentWidget(chatPages_[QString::fromStdString(from)]);
	chatPages_[QString::fromStdString(from)]->onRecvMessage(from, type, data);
}

void MainWindow::onSendChatMsgResponse(bool success, const std::string& toUserId, uint32_t seq)
{
	if (success)
	{
		/// 提示发送成功，更新聊天窗口
		// 判断chatPages_是否为空
		if (chatPages_.empty()) return;
		// 判断chatPages_中是否存在toUserId
		if (chatPages_.find(QString::fromStdString(toUserId)) == chatPages_.end()) return;
		// 更新聊天窗口
		chatPages_[QString::fromStdString(toUserId)]->onSendMessageResponse(seq);
	}
	else
	{
		// 提示发送失败
		// TODO
		LOG_DEBUG << "send chat message failed!";
	}
}

void MainWindow::onUploadResponse(const std::string& from, const std::string& to, uint32_t seq, bool isSuccess)
{
	LOG_DEBUG << "onUploadResponse";
	if (isSuccess)
	{
		/// 提示发送成功，更新聊天窗口
		// 判断chatPages_是否为空
		if (chatPages_.empty()) return;
		// 判断chatPages_中是否存在toUserId
		if (chatPages_.find(QString::fromStdString(to)) == chatPages_.end()) return;
		// 更新聊天窗口
		chatPages_[QString::fromStdString(to)]->onUploadResponse(seq);
	}
	else
	{
		// 提示发送失败
		// TODO
		LOG_DEBUG << "upload file failed!";
	}
}

void MainWindow::onDownloadResponse(const std::string& from, const std::string& to, const std::string& fileName, uint32_t seq, bool isSuccess)
{
	if (isSuccess)
	{
		// 判断发送者, 如果是发送者则发送给to, 如果是接收者则发送给from
		std::string sendTo = from == userId_.toStdString() ? to : from;

		/// 提示发送成功，更新聊天窗口
		// 判断chatPages_是否为空
		if (chatPages_.empty()) return;
		// 判断chatPages_中是否存在toUserId
		if (chatPages_.find(QString::fromStdString(sendTo)) == chatPages_.end()) return;
		// 更新聊天窗口
		chatPages_[QString::fromStdString(sendTo)]->onDownloadResponse(seq);
		
	}
	else
	{
		// 提示发送失败
		// TODO
		LOG_DEBUG << "download file failed!";
	}
}



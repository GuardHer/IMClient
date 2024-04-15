#include "LoginWidget.h"

#include <QListView>
#include <QSettings>
#include <QDebug>

#include "../base/Singleton.h"
#include "../UserManager.h"
#include "../SQLiteManager.h"
#include "../base/Logging.h"
#include "../IMClient.h"
#include "../FileClient.h"
#include "../FileSession.h"


//#define USERMANAGER UserManagerSingleton::instance()

LoginWidget::LoginWidget(QWidget *parent)
	: QWidget(parent), rememberPassword_(true)
{
	ui.setupUi(this);
}

LoginWidget::~LoginWidget()
{

}

bool LoginWidget::init()
{
	initUser();
	initUI();
	initConnect();
	return true;
}

void LoginWidget::initUI()
{
	ui.UserId_->setView(new QListView());
	if (userId_.isEmpty() || password_.isEmpty())
	{
		ui.loginPages_->setCurrentIndex(0);
	}
	else
	{
		ui.loginPages_->setCurrentIndex(0);
	}

	

	ui.UserId_->setEditText(userId_);
	ui.PassWord_->setText(password_);
}

void LoginWidget::initConnect()
{
	QObject::connect(ui.LoginBtn_, &QPushButton::clicked, this, &LoginWidget::onLogin);
	QObject::connect(ui.LoginBtn2_, &QPushButton::clicked, this, &LoginWidget::onLogin);
}

void LoginWidget::initUser()
{
	/*
	[CurrentUserInfo]
	UserId=1291035260
	Password=044438
	UserName=GuardHer
	*/

	QSettings settings("../../res/configFile/login.ini", QSettings::IniFormat);
	userId_ = settings.value("CurrentUserInfo/UserId").toString();
	password_ = settings.value("CurrentUserInfo/Password").toString();
	userName_ = settings.value("CurrentUserInfo/UserName").toString();

	if (userId_.isEmpty() || password_.isEmpty()) return;

	

	//user_ = USERMANAGER.getUserInfo(userId_.toInt());
	user_.setUserId(userId_);
	user_.setPassword(password_);
	user_.setUserName(userName_);
	/*if (user_ == nullptr) return;*/

}



void LoginWidget::saveUser()
{
	QSettings settings("../../res/configFile/login.ini", QSettings::IniFormat);
	settings.beginGroup("CurrentUserInfo");                       // beginGroup与endGroup之间的内容都是属于一个组的
	settings.setValue("CurrentUserInfo/UserId", userId_);         // 保存userId
	settings.setValue("CurrentUserInfo/Password", password_);     // 保存password
	settings.endGroup();										  // endGroup与beginGroup对应，表示组的结束
}


/*==================================== slots ==========================================*/
void LoginWidget::onLogin()  // 处理登录按钮的点击事件
{
	if (rememberPassword_) {
		saveUser();
	}
	else {
		userId_.clear();
		password_.clear();
	}

	
	/// 登录
	if (login())
	{
		MainWindow& mainWindow_ = Singleton<MainWindow>::instance();
		IMClient& cli = Singleton<IMClient>::instance();
		FileClient& fCli = Singleton<FileClient>::instance();
		emit signalUser(userId_);
		this->hide();
		mainWindow_.init();
		mainWindow_.show(); // 显示主窗口
		QObject::connect(&mainWindow_, &MainWindow::emitClose, this, &LoginWidget::onCloseApp);
		QObject::connect(&cli, &IMClient::sigSendChatMsgResponse, &mainWindow_, &MainWindow::onSendChatMsgResponse);
		QObject::connect(&cli, &IMClient::sigRecvChatMsg, &mainWindow_, &MainWindow::onRecvMessage);
		QObject::connect(&fCli, &FileClient::sigUploadResponse, &mainWindow_, &MainWindow::onUploadResponse);
		QObject::connect(&fCli, &FileClient::sigDownloadResponse, &mainWindow_, &MainWindow::onDownloadResponse);

		FileSession::login(userId_.toStdString(), password_.toStdString());
	}
	else
	{
		LOG_DEBUG << "Login failed";
	}
}
bool LoginWidget::login()
{
	/// 向服务器发送登录请求
	ByteArray byteArray;
	Singleton<ClientSession>::instance().packLogin(&byteArray, userId_.toStdString(), password_.toStdString());
	IMClient& cli = Singleton<IMClient>::instance();
	if (!cli.isConnect()) return false;
	QObject::connect(&cli, &IMClient::sigLoginResponse, this, &LoginWidget::onLoginSuccess);
	cli.sendMessage(&byteArray);
	LOG_DEBUG << "LoginWidget sendMessage";
	
	/// 等待登录结果
	QEventLoop loop;
	connect(this, &LoginWidget::loginFinished, &loop, &QEventLoop::quit);
	loop.exec();

	return loginResponse_;
}
void LoginWidget::closeEvent(QCloseEvent* event)
{
	emit closeLoginWidget();
	event->accept();
}
void LoginWidget::onRegister()  // 处理注册按钮的点击事件
{
	//emit registerSuccess();

}

void LoginWidget::onLoginSuccess(bool success, const std::string& userId)
{
	LOG_DEBUG << "LoginWidget RecvLoginResponse: " << success;

	isRecvLoginResponse_ = true;
	loginResponse_ = success;

	emit loginFinished();
}

void LoginWidget::onCloseApp()
{
	LOG_DEBUG << "App Close"; 
	auto& sqlManager = Singleton<SQLiteManager>::instance();
	auto& emojiWindow = Singleton<EmojiWindow>::instance();
	auto& mainWindow = Singleton<MainWindow>::instance();
	mainWindow.close();
	emojiWindow.close();

	sqlManager.messageExecute(&SQLiteMessage::insertMultiMessage, Singleton<SQLiteMessage>::instance());
	sqlManager.Stop();
	
	this->close();
}

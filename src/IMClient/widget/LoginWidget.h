#pragma once

#include <QWidget>
#include <QObject>
#include "ui_LoginWidget.h"

#include "../manager/User.h"
#include "../base/noncopyable.h"
#include "../base/Singleton.h"
#include "MainWindow.h"


class LoginWidget : public QWidget
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = nullptr);
	~LoginWidget();

public:
	void initUI();
	void initConnect();
	void initUser();
	bool init();
	void saveUser();

	void setUserName(const QString& userName) { userName_ = userName; }
	void setPassword(const QString& password) { password_ = password; }
	void setUserId(const QString& userId) { userId_ = userId; }

	QString userName() const { return userName_; }
	QString password() const { return password_; }
	QString userId() const { return userId_; }

private:
	bool login();

protected:
	void closeEvent(QCloseEvent* event) override;

signals:
	void signalUser(const QString& userId);
	void loginFinished();
	void registerSuccess();

	void closeLoginWidget();

private slots:
	void onLogin();
	void onRegister();
	void onLoginSuccess(bool success, const std::string& userId);

	void onCloseApp();

private:
	Ui::LoginWidgetClass ui;
	//MainWindow* mainWindow_;

	QString userName_;
	QString password_;
	QString userId_;

	User user_;

	bool rememberPassword_;
	bool loginResponse_{false};
	bool isRecvLoginResponse_{false};
};

#define LoginWidgetInstance Singleton<LoginWidget>::instance()

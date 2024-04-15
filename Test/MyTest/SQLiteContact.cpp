#include "SQLiteContact.h"

#include <string>
#include <QThread>
#include <QDebug>

SQLiteContactTest::SQLiteContactTest()
{
	// drivers
	qDebug() << "Available drivers: ";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)
	{
		qDebug() << driver;
	}
}

SQLiteContactTest::~SQLiteContactTest()
{
}

QSqlDatabase SQLiteContactTest::createDatabase()
{
	QSqlDatabase db;
	QString connectionName = QString::number(reinterpret_cast<std::uintptr_t>(QThread::currentThreadId()));


	if (QSqlDatabase::contains(connectionName))  
	{
		db = QSqlDatabase::database(connectionName); 
		qDebug() << ("Database is already open!");
	}
	else
	{
		db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
		db.setDatabaseName("E:\\StudyVideo\\IMClient\\src\\IMClient\\File\\1291035260\\Contact\\Contact.db");
	}

	if (!db.open())
	{
		qDebug() << "Error: Failed to connect database." << db.lastError();
	}
	else
	{
		qDebug() << "Database is opened!";
	}

	return db;
	
}

bool SQLiteContactTest::openDatabase()
{
	return true;
}

void SQLiteContactTest::closeDatabase()
{

}

void SQLiteContactTest::setUserId(const QString& userId)
{
	userId_ = userId;
}

bool SQLiteContactTest::init()
{
	this->dbName_ = "E:\\StudyVideo\\IMClient\\src\\IMClient\\File\\1291035260\\Contact\\Contact.db";

	return true;
}

void SQLiteContactTest::queryFriendInfo()
{
	qDebug() << "queryFriendInfo";
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	query.prepare("SELECT * FROM FriendInfo");
	if (!query.exec())
	{
		qDebug() << "Error: Fail to query." << query.lastError();
	}
	else
	{
		while (query.next())
		{
			int LocalId = query.value(0).toInt();
			int FriendId = query.value(1).toInt();
			QString FriendName = query.value(2).toString();
			QString NickName = query.value(3).toString();
			QString PassWord = query.value(4).toString();
			int FaceType = query.value(5).toInt();
			QString CustomFace = query.value(6).toString();
			QString CustomFaceFmt = query.value(7).toString();

			qDebug() << "LocalId: " << LocalId << " FriendId: " << FriendId << " FriendName: " << FriendName << " NickName: " << NickName << " PassWord: " << PassWord << " FaceType: " << FaceType << " CustomFace: " << CustomFace << " CustomFaceFmt: " << CustomFaceFmt;
		}
	}

	if (db.isOpen())
	{
		db.close();
	}
}

std::vector<std::map<QString, QVariant>> SQLiteContactTest::queryAllFriendInfo(const QString& userId)
{
	std::vector<std::map<QString, QVariant>> friendInfoList;

	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	query.prepare("SELECT * FROM FriendInfo");
	if (!query.exec())
	{
		qDebug() << "Error: Fail to query." << query.lastError();
	}
	else
	{
		while (query.next())
		{
			std::map<QString, QVariant> friendInfo;
			friendInfo["LocalId"] = query.value(0).toInt();
			friendInfo["FriendId"] = query.value(1).toInt();
			friendInfo["FriendName"] = query.value(2).toString();
			friendInfo["NickName"] = query.value(3).toString();
			friendInfo["PassWord"] = query.value(4).toString();
			friendInfo["FaceType"] = query.value(5).toInt();
			friendInfo["CustomFace"] = query.value(6).toString();
			friendInfo["CustomFaceFmt"] = query.value(7).toString();

			friendInfoList.push_back(friendInfo);
		}
	}

	if (db.isOpen())
	{
		db.close();
	}

	return friendInfoList;
}

#include "SQLiteManager.h"

#include <QDebug>

SQLiteManager::SQLiteManager() : 
	sqliteContact_(Singleton<SQLiteContactTest>::instance())
	//,sqliteMessage_(Singleton<SQLiteMessage>::instance())
{
	contactThread_ = new SQLiteThread(1);
	//messageThread_ = new SQLiteThread(1);
}

SQLiteManager::~SQLiteManager()
{
	contactThread_->Stop();
	//messageThread_->Stop();
	delete contactThread_;
	//delete messageThread_;
}

bool SQLiteManager::Init(const QString& userId_)
{
	//this->contactExecute(&SQLiteContactTest::setUserId, sqliteContact_, userId_);
	//this->contactExecute(&SQLiteContactTest::init, sqliteContact_);
	//this->contactExecute(&SQLiteContactTest::createDatabase, sqliteContact_);
	//this->contactExecute(&SQLiteContactTest::openDatabase, sqliteContact_);
	this->contactExecute(&SQLiteContactTest::queryFriendInfo, sqliteContact_);

	auto future = this->contactExecute(&SQLiteContactTest::queryAllFriendInfo, sqliteContact_, userId_);
	auto result = future.get();
	for (auto& item : result)
	{
		for (auto& it : item)
		{
			qDebug() << it.first << " : " << it.second;
		}
	}

	return true;

}



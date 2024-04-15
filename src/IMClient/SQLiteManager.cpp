#include "SQLiteManager.h"

#include <QDebug>

SQLiteManager::SQLiteManager()
{
	contactThread_ = new SQLiteThread(1);
	messageThread_ = new SQLiteThread(1);
}

SQLiteManager::~SQLiteManager()
{
	Stop();
	delete contactThread_;
	delete messageThread_;
}

bool SQLiteManager::Init(const QString& userId_)
{
	return true;
}

void SQLiteManager::Stop()
{
	contactThread_->Stop();
	messageThread_->Stop();
}



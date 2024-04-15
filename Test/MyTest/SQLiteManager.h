#pragma once

#include "SQLiteThread.h"

class SQLiteContact;
class SQLiteMessage;


class SQLiteManager : noncopyable, nonmoveable
{
public:
	SQLiteManager();
	~SQLiteManager();

	bool Init(const QString& userId_);

private:
	SQLiteThread* contactThread_;
	//SQLiteThread* messageThread_;
	SQLiteContactTest& sqliteContact_;
	//SQLiteMessage& sqliteMessage_;

public: 

	template <class F, class... Args>
	auto contactExecute(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
		return contactThread_->Add(std::forward<F>(f), std::forward<Args>(args)...);
	}


};


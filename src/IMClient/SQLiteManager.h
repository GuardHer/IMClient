#pragma once

#include "SQLiteThread.h"


class SQLiteManager : noncopyable, nonmoveable
{
public:
	SQLiteManager();
	~SQLiteManager();

	bool Init(const QString& userId_);

	void Stop();

private:
	SQLiteThread* contactThread_;
	SQLiteThread* messageThread_;

public: 

	template <class F, class... Args>
	auto contactExecute(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
		return contactThread_->Add(std::forward<F>(f), std::forward<Args>(args)...);
	}

	template <class F, class... Args>
	auto messageExecute(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
		return messageThread_->Add(std::forward<F>(f), std::forward<Args>(args)...);
	}

};


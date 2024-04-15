#include "SQLiteThread.h"


SQLiteThread::SQLiteThread(unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i) {
        workers_.emplace_back(std::thread([this]() { // emplace_back 用于在容器末尾直接构造对象
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    condition_variable_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = tasks_.front();
                    tasks_.pop();
                }
                task();
            }
            }));
    }
}

SQLiteThread::~SQLiteThread() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_variable_.notify_all();
    for (std::thread& th : workers_) {
        if (th.joinable()) {
            th.join();
        }
    }
}

void SQLiteThread::Stop()
{
    {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		stop_ = true;
	}
	condition_variable_.notify_all();
    for (std::thread& th : workers_) {
        if (th.joinable()) {
			th.join();
		}
	}
}

//auto SQLiteThread::executeDatabaseOperation(std::function<void(SQLiteMessage&)> operation)
//{
//    auto futureResult = Add([this, operation]() {
//		operation(sqliteMessage_);
//	});
//
//    return futureResult;
//}
//
//auto SQLiteThread::executeDatabaseOperation(std::function<void(SQLiteContact&)> operation)
//{
//    auto futureResult = Add([this, operation]() {
//		operation(sqliteContact_);
//	});
//
//    return futureResult;
//}

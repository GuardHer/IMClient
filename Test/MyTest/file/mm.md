SQLiteThread.h:

```c++
#pragma once
#include <condition_variable>  // NOLINT
#include <functional>
#include <future>  // NOLINT: 
#include <memory>
#include <mutex>  // NOLINT
#include <queue>
#include <thread>  // NOLINT
#include <utility>
#include <vector>
#include "noncopyable.h"
#include "nonmoveable.h"
#include "Singleton.h"

#include "SQLiteContact.h"


class SQLiteThread : noncopyable, nonmoveable
{
public:

    //explicit SQLiteThread(unsigned int size = std::thread::hardware_concurrency()); // hardware_concurrency：返回支持的并发线程数
    explicit SQLiteThread(unsigned int size = 1);
    ~SQLiteThread();

    template <class F, class... Args>
    auto Add(F&& f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>;

    void Stop();

private:
    std::vector<std::thread> workers_;           // 线程池
    std::queue<std::function<void()>> tasks_;    // 任务队列
    std::mutex queue_mutex_; 				     // 任务队列锁
    std::condition_variable condition_variable_; // 条件变量
    std::atomic<bool> stop_{false}; 			 // 停止标志

};

// 不能放在cpp文件，C++编译器不支持模版的分离编译
template <class F, class... Args>
auto SQLiteThread::Add(F&& f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task =
        std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        // 停止池后不允许排队
        if (stop_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks_.emplace([task]() { (*task)(); });

    }
    condition_variable_.notify_one(); // 通知一个等待线程
    return res;
}



```

SQLiteThread.cpp:

```c++
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
```

SQLiteContact.cpp:

```c++
#include "SQLiteContact.h"

#include <string>
#include <QThread>
#include <QDebug>

SQLiteContactTest::SQLiteContactTest()
{
}

SQLiteContactTest::~SQLiteContactTest()
{
}

bool SQLiteContactTest::createDatabase()
{
	QString connectionName = "46465464";

	if (QSqlDatabase::contains(connectionName))  
	{
		db = QSqlDatabase::database(connectionName); 
		qDebug() << ("Database is already open!");
	}
	else
	{
		db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
		qDebug() << "dbName: " << dbName;
		db.setDatabaseName(dbName);
	}
	return true;
	
}

bool SQLiteContactTest::openDatabase()
{
	if (!db.open())
	{
		qDebug() << "Error: Failed to connect database." << db.lastError();
		return false;
	}
	else
	{
		qDebug() << "Database is opened!";
		return true;
	}
}

void SQLiteContactTest::closeDatabase()
{
	if (db.isOpen())
	{
		db.close();
	}
}

void SQLiteContactTest::setUserId(const QString& userId)
{
	userId_ = userId;
}

bool SQLiteContactTest::init()
{
	this->dbName = "E:\\StudyVideo\\IMClient\\src\\IMClient\\File\\1291035260\\Contact\\Contact.db";

	if (!createDatabase())
	{
		return false;
	}

	if (!openDatabase())
	{
		return false;
	}

	return true;
}

void SQLiteContactTest::queryFriendInfo()
{
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
}

```

Widget.cpp:

```c++
#include "Widget.h"

#include <QDebug>

#include "SQLiteManager.h"

Widget::Widget(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(800, 800);
	init();
}

Widget::~Widget()
{

}

void Widget::init()
{
	SQLiteManager* sqliteManager = new SQLiteManager();
	sqliteManager->Init("1291035260");

}

```


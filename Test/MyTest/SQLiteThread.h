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



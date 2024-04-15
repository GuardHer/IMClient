#pragma once
#include <functional>
#include <future>  // NOLINT: 
#include <memory>
#include <mutex>  // NOLINT
#include <queue>
#include <thread>  // NOLINT
#include <utility>
#include <vector>
#include "base/noncopyable.h"
#include "base/nonmoveable.h"

class Thread : noncopyable, nonmoveable
{
public:
    typedef std::function<void()> Task;

    explicit Thread();
    ~Thread();

    template <class F, class... Args>
    auto Add(F&& f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type>;

    void Stop();
    void Start();
    void Run();

    bool IsRunning() const { return running_; }
    std::thread::id GetThreadId() const { return threadId_; }


private:
    std::queue<Task> tasks_;    // 任务队列
    bool stop_{false}; 			// 停止标志
    bool running_{false}; 	    // 运行标志
    std::thread thread_;        // 线程
    std::thread::id threadId_;  // 线程ID

};

// 不能放在cpp文件，C++编译器不支持模版的分离编译
template <class F, class... Args>
auto Thread::Add(F&& f, Args &&...args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task =
        std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        // 停止池后不允许排队
        if (stop_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks_.emplace([task]() { (*task)(); });

    }
    return res;
}



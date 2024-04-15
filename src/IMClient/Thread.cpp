#include "Thread.h"

Thread::Thread()
{
}

Thread::~Thread()
{
	Stop();
}

void Thread::Start()
{
    // �����߳�
    if (running_) {
		return;
	}
    thread_ = std::thread([this]() {
		Run();
	});
	running_ = true;
	threadId_ = thread_.get_id();
}

void Thread::Run()
{
    while (true) {
        std::function<void()> task;
        {
            if (stop_ && tasks_.empty()) {
                return;
            }
            task = tasks_.front();
            tasks_.pop();
        }
        task();
    }
}

void Thread::Stop()
{
    stop_ = true;
    running_ = false;
    if (thread_.joinable()) {
		thread_.join();  // �ȴ��߳̽���
	}
}


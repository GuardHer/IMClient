#pragma once

#include <QObject>
#include <thread>
#include "base/Singleton.h"

#include "hloop.h"
#include "htime.h"

#include "EventLoop.h"

class FileClientThread : public QObject
{
    Q_OBJECT
public:
    FileClientThread(QObject* parent = nullptr);
    ~FileClientThread();

    void start();
    void stop();
    bool isRunning() { return isRunning_; }

    hv::EventLoopPtr getLoop() { return loop_; }

private:
    void run();

public slots:
    void onStop();

private:
    hv::EventLoopPtr loop_;
    std::thread thread_;

    bool isRunning_;
};


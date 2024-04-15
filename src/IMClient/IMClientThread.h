#pragma once

#include <QObject>
#include <thread>
#include "base/Singleton.h"

#include "hloop.h"
#include "htime.h"

#include "EventLoop.h"

class IMClientThread : public QObject
{
	Q_OBJECT
public:
    IMClientThread(QObject* parent = nullptr);
    ~IMClientThread();

    void start();
    void run();
    void stop();

    bool isRunning() { return isRunning_; }

    hv::EventLoopPtr getLoop() { return loop_; }

public slots:
    void onStop();

private:
    hv::EventLoopPtr loop_;
    std::thread thread_;

    bool isRunning_;
};


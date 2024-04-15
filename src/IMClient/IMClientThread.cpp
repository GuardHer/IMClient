#include "IMClientThread.h"
#include "base/Logging.h"
#include "IMClient.h"



IMClientThread::IMClientThread(QObject* parent)
	: QObject(parent), isRunning_(false)
{
}

IMClientThread::~IMClientThread()
{
	if (isRunning()) stop();
}


void IMClientThread::start()
{
	if (isRunning()) return;

	loop_ = std::make_shared<hv::EventLoop>();
	thread_ = std::thread([this] { run(); });

	isRunning_ = true;

}

void IMClientThread::run()
{
	
	LOG_DEBUG << "IMClientThread : loop_tid:" << loop_->tid();
	loop_->run();
	/// ����һ�� TcpClient ���󣬴��� EventLoop ����, ������TcpClient����ʱ, ���û�п�ʼloop->run(), ��ᴴ��һ���µ��̣߳�����ʼloop->run()
	//bool res = Singleton<IMClient>::instance().Init("172.23.64.57", 9999, loop_);
}

void IMClientThread::stop()
{
	LOG_DEBUG << "IMClientThread::stop";
	isRunning_ = false;

	if (loop_->isRunning()) loop_->stop();
	if (thread_.joinable()) {
		thread_.join();
	}
	
}

void IMClientThread::onStop()
{
	stop();
}

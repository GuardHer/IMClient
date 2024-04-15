#include "FileClientThread.h"
#include "base/Logging.h"
#include "IMClient.h"


FileClientThread::FileClientThread(QObject* parent)
	: QObject(parent), isRunning_(false)
{
}

FileClientThread::~FileClientThread()
{
	if (isRunning()) stop();
}


void FileClientThread::start()
{
	if (isRunning()) return;

	loop_ = std::make_shared<hv::EventLoop>();
	thread_ = std::thread([this] { run(); });

	isRunning_ = true;

}

void FileClientThread::run()
{

	LOG_DEBUG << "FileClientThread : loop_tid:" << loop_->tid();
	loop_->run();
	/// ����һ�� TcpClient ���󣬴��� EventLoop ����, ������TcpClient����ʱ, ���û�п�ʼloop->run(), ��ᴴ��һ���µ��̣߳�����ʼloop->run()
	//bool res = Singleton<IMClient>::instance().Init("172.23.64.57", 9999, loop_);
}

void FileClientThread::stop()
{
	LOG_DEBUG << "FileClientThread::stop";
	isRunning_ = false;

	if (loop_->isRunning()) loop_->stop();
	if (thread_.joinable()) {
		thread_.join();
	}

}

void FileClientThread::onStop()
{
	stop();
}

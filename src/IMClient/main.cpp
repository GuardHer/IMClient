#include <QtWidgets/QApplication>
#include <QMetaType>

#include "widget/LoginWidget.h"
#include "widget/MainWindow.h"

#include "SQLiteMessage.h"
#include "SQLiteContact.h"
#include "IMClientThread.h"
#include "FIleClientThread.h"
#include "IMClient.h"
#include "FileClient.h"
#include "FileManager.h"
#include "Protocol.h"
#include "ByteArray.h"

#include "base/TimeZone.h"
#include "base/crc64.h"
#include "base/Singleton.h"
#include "base/Logging.h"

#include "hloop.h"
#include "htime.h"
#include "hv.h"
#include "TcpClient.h"
#include "EventLoop.h"

#include "base/config.h"

#define TEST 0

static void onTimer(hv::TimerID timerID, int n) {
    printf("tid=%ld timerID=%lu time=%lus n=%d\n", hv_gettid(), (unsigned long)timerID, (unsigned long)time(NULL), n);
}
void TestClient()
{
    using namespace hv;
    LOG_DEBUG << "main tid=" << hv_gettid();

    // 创建一个 EventLoop 对象
    EventLoopPtr loop = std::make_shared<EventLoop>();

    // 创建一个 TcpClient 对象，传入 EventLoop 对象
    auto client = std::make_shared<TcpClient>(loop);
    int connfd = client->createsocket(9999, "172.23.64.57");

    // 配置 TcpClient 的回调函数
    client->onConnection = [](const SocketChannelPtr& channel) {
        printf("Connected to server\n");
        // 连接建立后发送数据
        channel->write("Hello, server!");
    };

    client->onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        LOG_DEBUG << "Received message from server: " << buf->data();
    };
    loop->setTimeout(10000, [&loop](hv::TimerID timerID) {
        loop->stop();
        });

    // 启动 TcpClient
    client->start();

    // 进入事件循环
    loop->run();
}
void TestClientEventLool()
{
    using namespace hv;
    HV_MEMCHECK;

    printf("main tid=%ld\n", hv_gettid());

    hv::EventLoopPtr loop = std::make_shared<hv::EventLoop>();

    std::thread loopThread([&]() {
        loop->run();
        });

    while (!loop->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // 输出loopThread id
    std::cout << "loopThread id = " << loopThread.get_id() << std::endl;
    LOG_DEBUG << "loop tid = " << loop->tid();
    std::shared_ptr<hv::TcpClient> client = std::make_shared<hv::TcpClient>(loop);

    int connfd = client->createsocket(9999, "172.23.64.57");
    //runAfter 10s
    loop->setTimeout(10000, [&loop](hv::TimerID timerID) {
        loop->stop();
        });

    client->onConnection = [&client](const SocketChannelPtr& channel) {
        LOG_DEBUG << "connected";
        client->send("Hello, World!");
    };
    client->onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        LOG_DEBUG << "< " << buf->data();
    };

    client->start();
    loopThread.join();  // 等待loopThread退出
    //loop->run();
}
void TestClientEventLool2()
{
    using namespace hv;
    HV_MEMCHECK;

    printf("main tid=%ld\n", hv_gettid());

    auto client = std::make_shared<TcpClient>();
    int connfd = client->createsocket(9999, "172.23.64.57");
    client->onConnection = [](const SocketChannelPtr& channel) {
		LOG_DEBUG << ("Connected to server\n");
		channel->write("Hello, server!");
	};
    client->onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
		LOG_DEBUG << ("Received message from server: ") << buf->data();
        channel->write("Hello, server!");
	};
	client->start();
    auto loop = client->loop();
    LOG_DEBUG << "loop status" << loop->status();
    loop->setTimeout(10000, [&loop](hv::TimerID timerID) {
		loop->stop();
		});
    LOG_DEBUG << "loop tid=" << loop->tid();
    
	std::this_thread::sleep_for(std::chrono::seconds(10));
}
void TestCrc64()
{
    const int32_t magic = 0x12345678;
    const int32_t seq = 0;
    const int32_t cmd = 1200;
    const int32_t len = 56;
    const std::string data = "Hello, World!";

    uint64_t crc1 = crc64(0, &magic, sizeof(magic));
    uint64_t crc2 = crc64(crc1, &seq, sizeof(seq));
    uint64_t crc3 = crc64(crc2, &cmd, sizeof(cmd));
    uint64_t crc4 = crc64(crc3, &len, sizeof(len));
	uint64_t crc5 = crc64(crc4, data);
    LOG_DEBUG << "crc5=" << crc5;

    ByteArray byteArray;
    byteArray.appendInt32(magic);
    byteArray.appendInt32(seq);
    byteArray.appendInt32(cmd);
    byteArray.appendInt32(len);
    byteArray.append(data);
    uint64_t crc6 = crc64(0, byteArray);
    byteArray.appendUint64(crc6);
    LOG_DEBUG << "crc6=" << crc6;

    QByteArray qByteArray;
    qByteArray.append((const char*)&magic, sizeof(magic));
    qByteArray.append((const char*)&seq, sizeof(seq));
    qByteArray.append((const char*)&cmd, sizeof(cmd));
    qByteArray.append((const char*)&len, sizeof(len));
    qByteArray.append(data.c_str(), data.size());
    uint64_t crc7 = crc64(0, qByteArray);
    LOG_DEBUG << "crc7=" << crc7;
}
void TestEventLoop()
{

    using namespace hv;
    HV_MEMCHECK;

    printf("main tid=%ld\n", hv_gettid());

    auto loop = std::make_shared<EventLoop>();

    // runEvery 1s
    loop->setInterval(1000, std::bind(onTimer, std::placeholders::_1, 100));

    // runAfter 10s
    loop->setTimeout(10000, [&loop](TimerID timerID) {
        loop->stop();
        });

    loop->queueInLoop([]() {
        printf("queueInLoop tid=%ld\n", hv_gettid());
        });

    loop->runInLoop([]() {
        printf("runInLoop tid=%ld\n", hv_gettid());
        });

    // run until loop stopped
    loop->run();
}
void TestEventLoopThread()
{
    using namespace hv;
    HV_MEMCHECK;

    printf("main tid=%ld\n", hv_gettid());

    EventLoopThread loop_thread;
    const EventLoopPtr& loop = loop_thread.loop();

    // runEvery 1s
    loop->setInterval(1000, std::bind(onTimer, std::placeholders::_1, 100));

    // runAfter 10s
    loop->setTimeout(10000, [&loop](TimerID timerID) {
        loop->stop();
        });

    loop_thread.start();

    loop->queueInLoop([]() {
        printf("queueInLoop tid=%ld\n", hv_gettid());
        });

    loop->runInLoop([]() {
        printf("runInLoop tid=%ld\n", hv_gettid());
        });

    // wait loop_thread exit
    loop_thread.join();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // LOG
    TimeZone timeZone(8 * 60 * 60, "China");
    Logger::setLogLevel(Logger::DEBUG);
    Logger::setTimeZone(timeZone);

    // Config
    Config::initPath(ABSOLUTE_FILE_PATH());

    crc64_init();

    if (TEST) {
        TestClientEventLool2();

    }   
   
    // IMClientThread && IMClient
    IMClientThread& clientThread = Singleton<IMClientThread>::instance();
    clientThread.start();
    IMClient& imclient = Singleton<IMClient>::instance();
    auto loop = clientThread.getLoop();
    while (!loop->isRunning()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }   // 等待loopThread启动
    LOG_DEBUG << "loop status " << loop->status();
    bool res = imclient.Init(SERVER_IP, SERVER_PORT, loop);

    // FileClientThread && FileClient && Manager
    FileClientThread& fileClientThread = Singleton<FileClientThread>::instance();
    FileManager& fileManager = Singleton<FileManager>::instance();
    FileClient& fileClient = Singleton<FileClient>::instance();
    fileManager.Init(Config::getFilesPath());
    fileClientThread.start();
    auto fileLoop = fileClientThread.getLoop();
    while (!fileLoop->isRunning()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }   // 等待loopThread启动
    LOG_DEBUG << "fileLoop status " << fileLoop->status();
    bool fileRes = fileClient.Init(SERVER_IP, FILE_SERVER_PORT, fileLoop);

    // UI
    LoginWidget& loginWidget = Singleton<LoginWidget>::instance();
    MainWindow& mainWindow = Singleton<MainWindow>::instance();
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<uint32_t>("uint32_t");
    QObject::connect(&loginWidget, &LoginWidget::signalUser, &mainWindow, &MainWindow::handleUser);
    QObject::connect(&mainWindow, &MainWindow::emitClose, &clientThread, &IMClientThread::onStop);
    QObject::connect(&mainWindow, &MainWindow::emitClose, &fileClientThread, &FileClientThread::onStop);
    QObject::connect(&mainWindow, &MainWindow::emitClose, &imclient, &IMClient::onClose);
    QObject::connect(&mainWindow, &MainWindow::emitClose, &fileClient, &FileClient::onClose);
    loginWidget.init();
    loginWidget.show();

    //int ret = 0;
    int ret = a.exec();
    return ret;
}

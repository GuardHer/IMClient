#include <QApplication>
#include <QListView>
#include <locale>
#include <codecvt>
#include <string>
#include <thread>
#include <QFileDialog>
#include <qdebug.h>

#include "Widget.h"

#include "FileManager.h"
#include "FileSession.h"
#include "FileClient.h"
#include "hlog.h"
#include "hloop.h"
#include "htime.h"
#include "crc64.h"

#define IP "172.23.69.52"
#define PORT 8888

// 全局日志器对象
logger_t* g_logger = NULL;

void initCrc64();
void testLogin();
void testUploadFile();
void testDownloadFile();
void init_logger();
void destroy_logger();

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    // 设置log输出级别
    init_logger();
    initCrc64();
    

    // 注册信号
    qRegisterMetaType<std::string>("std::string");

    // init manager
    std::string path = "E:/StudyVideo/IMClient/Test/MyTest/file";
    auto& manager = Singleton<FileManager>::instance();
    manager.Init(path);

    // init file client
    hv::EventLoopPtr loop = std::make_shared<hv::EventLoop>();
    std::thread t([loop]() {
		loop->run();
	});
    while (!loop->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    auto& fileClient = Singleton<FileClient>::instance();
    fileClient.Init(IP, PORT, loop);
    qDebug() << "fileClient init";

    loop->setTimeout(200000, [&loop](hv::TimerID timerID) {
        loop->stop();
        });

    //testLogin
    loop->setTimeout(2000, [&loop](hv::TimerID timerID) {
		testLogin();
		});

    loop->setTimeout(4000, [&loop](hv::TimerID timerID) {
        Singleton<FileClient>::instance().Close();
        });

    loop->setTimeout(6000, [&loop](hv::TimerID timerID) {
        Singleton<FileClient>::instance().Connect();
        });

    loop->setTimeout(8000, [&loop](hv::TimerID timerID) {
        testLogin();
        });

    // testUploadFile
	//loop->setTimeout(4000, [&loop](hv::TimerID timerID) {
 //       testDownloadFile();
	//	});

 //   // testDownloadFile
	//loop->setTimeout(4000, [&loop](hv::TimerID timerID) {
 //       testUploadFile();
	//	});


    //int ret = a.exec();
    t.join();

    // 销毁日志器
    destroy_logger();

    return 0;
}

void initCrc64()
{
	crc64_init();
}


void testLogin()
{
    qDebug() << "testLogin";
    ByteArray data;
    Singleton<FileSession>::instance().packLogin("1291035260", "044438", data);
    Singleton<FileClient>::instance().sendMessage(&data);
}

void testUploadFile()
{
    qDebug() << "testUploadFile";
    // 打开dialog选择文件
    //QString path = QFileDialog::getOpenFileName(nullptr, "选择文件", "E:/StudyVideo/IMClient/Test/MyTest/file", "All Files(*.*)");
    QString path = "E:/StudyVideo/IMClient/Test/MyTest/file/pdf1.pdf";
	std::string from = "1291035260";
	std::string to = "4025347148";
    ByteArray data;
    Singleton<FileSession>::instance().packUploadFile(path.toStdString(), from, to, data);
    Singleton<FileClient>::instance().sendMessage(&data);
}

void testDownloadFile()
{
    qDebug() << "testDownloadFile";
	std::string from = "1291035260";
	std::string to = "4025347148";
	std::string fileName = "pdf1.pdf";
	ByteArray data;
	Singleton<FileSession>::instance().packDownloadFile(fileName, from, to, data);
	Singleton<FileClient>::instance().sendMessage(&data);
}


// 初始化日志器
void init_logger() {
    g_logger = logger_create();
    logger_set_handler(g_logger, stdout_logger);
    logger_set_level(g_logger, LOG_LEVEL_VERBOSE);
    logger_set_format(g_logger, "%y-%m-%d %H:%M:%S %L %s");
}

// 销毁日志器
void destroy_logger() {
    logger_destroy(g_logger);
    g_logger = NULL;
}


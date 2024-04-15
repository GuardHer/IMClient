//
// Created by 12910 on 2023-12-10.
//
#include "net/tcpClient.h"
#include "net/eventLoop.h"
#include <base/logging.h>
#include <net/inetAddress.h>
#include "base/timeZone.h"

void onConnection(const net::TcpConnectionPtr& conn) {
    uint32_t magic = 0x12345678;
    uint32_t cmd = 1200;
    uint32_t seq = 0;

    std::string result = "{\"userid\": \"1291035260\",\"password\": \"044438\"}";
    uint32_t len = result.size();
    uint64_t crc = 0;

    net::Buffer buffer;
    buffer.appendInt32(magic);
    buffer.appendInt32(seq);
    buffer.appendInt32(cmd);
    buffer.appendInt32(len);
    buffer.append(result.c_str(), len);
    buffer.appendInt64(crc);

    if (conn->connected()) {
        conn->send(&buffer);
    }

    LOG_INFO << "onConnection: " << conn->name() << " is " << (conn->connected() ? "UP" : "DOWN");
}

void onMessage(const net::TcpConnectionPtr& conn,
    net::Buffer* buf,
    Timestamp receiveTime)
{

    uint32_t magic = 0;
    uint32_t seq = 0;
    uint32_t cmd = 0;
    uint32_t len = 0;
    std::string result;
    uint64_t crc = 0;

    magic = buf->readInt32();
    seq = buf->readInt32();
    cmd = buf->readInt32();
    len = buf->readInt32();
    result = buf->retrieveAsString(len);
    crc = buf->readInt64();

    LOG_INFO << "magic: " << magic << ", seq: " << seq << ", cmd: " << cmd << ", len: " << len << ", result: " << result << ", crc: " << crc;

    if (conn->connected()) {
        onConnection(conn);
	}
}


int main() {
    net::EventLoop loop;
    net::InetAddress serverAddr("172.24.83.224", 9999);
    net::TcpClient client(&loop, serverAddr, "TcpClient");

    Logger::setLogLevel(Logger::EINFO);

    client.connect();
    client.setConnectionCallback(onConnection);
    client.setMessageCallback(onMessage);


    loop.loop();
}


#include "Protocol.h"
#include <QDataStream>
#include <utility>
#include "base/crc64.h"

Request::Request()
	:m_seq(0), m_dataLen(0), m_data(""), m_magic(MAGIC)
	,m_crc(0), m_size(24)
{
}

Request::Request(Command::CommandEnum cmd, const std::string& data) :
	m_seq(0), m_cmd(cmd), m_dataLen(data.size()), m_msg(data), m_magic(MAGIC)
	,m_crc(0), m_size(24)
{
	m_size += m_dataLen;
	serializeToByteArray();
}

Request::Request(QByteArray& data) : m_magic(MAGIC)
,m_data(data), m_crc(0), m_size(24)
{
	parseFromByteArray(data);
}


uint64_t Request::calculateChecksum(const QByteArray& data)
{
	return crc64(0, data);
}

uint64_t Request::calculateChecksum(const std::string& data)
{
	return crc64(0, data);
}




bool Request::serializeToByteArray() {
	QDataStream stream(&m_data, QIODevice::WriteOnly);

	stream << m_magic;    // 4 字节
	stream << m_seq;      // 4 字节
	stream << m_cmd;      // 4 字节
	stream << m_dataLen;  // 4 字节
	stream.writeRawData(m_msg.data(), m_msg.size()); // 写入 std::string 中的原始数据

	m_crc = calculateChecksum(m_data);
	stream << m_crc;      // 8 字节
	if (stream.status() != QDataStream::Ok) {
		return false;
	}
	return true;
}

bool Request::serializeToByteArray(QByteArray& bytearray)
{
	QDataStream stream(&bytearray, QIODevice::WriteOnly);

	stream << m_magic;    // 4 字节
	stream << m_seq;      // 4 字节
	stream << int32_t(m_cmd);      // 4 字节
	stream << m_dataLen;  // 4 字节
	stream.writeRawData(m_msg.data(), m_msg.size()); // 写入 std::string 中的原始数据

	m_crc = calculateChecksum(bytearray);
	stream << m_crc;      // 8 字节
	if (stream.status() != QDataStream::Ok) {
		return false;
	}
	return true;
}

bool Request::parseFromByteArray(QByteArray& bytearray) {
	QDataStream stream(&bytearray, QIODevice::ReadOnly);

	stream >> m_seq;
	stream >> m_cmd;
	stream >> m_dataLen;
	QByteArray dataByteArray;
	stream.readRawData(dataByteArray.data(), m_dataLen); // 读取原始数据到 QByteArray 中
	m_msg = dataByteArray.toStdString(); // 将 QByteArray 转换为 std::string
	stream >> m_crc;

	if (stream.status() != QDataStream::Ok) {
		return false;
	}

	return true;
}

Response::Response()
	:m_seq(0)
	, m_cmd(0)
	, m_result(404)
	, m_resultStr("") 
	, m_magic(MAGIC)
	, m_crc(0)
{
}

bool Response::serializeToByteArray( QByteArray& bytearray) {
	QDataStream stream(&bytearray, QIODevice::WriteOnly);

	stream << m_seq;
	stream << m_cmd;
	stream << m_result;
	stream.writeRawData(m_resultStr.data(), m_resultStr.size()); // 写入 std::string 中的原始数据

	if (stream.status() != QDataStream::Ok) {
		return false;
	}
	return true;
}

bool Response::parseFromByteArray( QByteArray& bytearray) {
	QDataStream stream(&bytearray, QIODevice::ReadOnly);

	stream >> m_seq;
	stream >> m_cmd;
	stream >> m_result;
	QByteArray dataByteArray;
	stream.readRawData(dataByteArray.data(), m_result); // 读取原始数据到 QByteArray 中
	m_resultStr = dataByteArray.toStdString(); // 将 QByteArray 转换为 std::string

	if (stream.status() != QDataStream::Ok) {
		return false;
	}

	return true;
}

Notify::Notify()
	:m_notify(0) {
}

bool Notify::serializeToByteArray( QByteArray& bytearray) {
	
	return true;
}

bool Notify::parseFromByteArray( QByteArray& bytearray) {
	return true;
}
#include "FileSession.h"
#include "FileClient.h"
#include "FileManager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


void FileSession::packLogin(const std::string& userid, const std::string& password, ByteArray& data)
{
	/* {"userid": "123456", "password: ": "123456"} */
	QJsonObject json;
	json.insert("userid", QString::fromStdString(userid));
	json.insert("password", QString::fromStdString(password));
	QJsonDocument doc(json);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);
	std::string dataStr = bytes.toStdString();

	uint32_t magic = MAGIC;
	uint32_t cmd = FileCmd::FILE_CMD_LOGIN;
	uint32_t length = dataStr.length();
	uint64_t crc = crc64(0, dataStr);

	qDebug() << "packLogin: " << dataStr.c_str();
	qDebug() << "crc: " << crc;


	data.appendUint32(magic);
	data.appendUint32(cmd);
	data.appendUint32(length);
	data.append(dataStr);
	data.appendUint64(crc);
}

void FileSession::packLogout(std::string& userid, std::string& password, ByteArray& data)
{
}

void FileSession::packUploadFile(const std::string& filePath, const std::string& from, const std::string& to, ByteArray& data)
{
	// {"from": "", "to": "", "filename": "", "filesize": "", "md5":"", "data":""}
	//根据文件路径获取文件名
	std::string filename = filePath.substr(filePath.find_last_of("/") + 1);
	std::string fileData;
	uint32_t filesize = Singleton<FileManager>::instance().ReadFile(filePath, fileData);
	// 计算文件的md5 
	char md5[33] = { 0 };
	Singleton<FileManager>::instance().GetFileMd5(fileData, md5);

	QJsonObject json;
	json.insert("from", QString::fromStdString(from));
	json.insert("to", QString::fromStdString(to));
	json.insert("filename", QString::fromStdString(filename));
	json.insert("data", QString::fromStdString(fileData));
	qDebug() << "filesize: " << filesize;

	QJsonDocument doc(json);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);
	std::string dataStr = bytes.toStdString();

	uint32_t magic = MAGIC;
	uint32_t cmd = FileCmd::FILE_CMD_UPLOAD;
	uint32_t length = dataStr.length();
	uint64_t crc = crc64(0, dataStr);

	data.appendUint32(magic);
	data.appendUint32(cmd);
	data.appendUint32(length);
	data.append(dataStr);
	data.appendUint64(crc);

	qDebug() << "packUploadFile: " << fileData.size();
}

void FileSession::packDownloadFile(const std::string& filename, const std::string& from, const std::string& to, ByteArray& data)
{
	// {"from": "xxx", "to": "xxx", "filename": "xxx"}
	QJsonObject json;
	json.insert("from", QString::fromStdString(from));
	json.insert("to", QString::fromStdString(to));
	json.insert("filename", QString::fromStdString(filename));

	QJsonDocument doc(json);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);
	std::string dataStr = bytes.toStdString();

	uint32_t magic = MAGIC;
	uint32_t cmd = FileCmd::FILE_CMD_DOWNLOAD;
	uint32_t length = dataStr.length();
	uint64_t crc = crc64(0, dataStr);

	data.appendUint32(magic);
	data.appendUint32(cmd);
	data.appendUint32(length);
	data.append(dataStr);
	data.appendUint64(crc);

	qDebug() << "packDownloadFile: " << dataStr.size();

}

void FileSession::packDownloadFileAck(ByteArray& data)
{
}


#include "FileSession.h"
#include "FileClient.h"
#include "FileManager.h"
#include "Protocol.h"

#include "base/Logging.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


void FileSession::login(const std::string& userid, const std::string& password)
{
	ByteArray data;
	packLogin(userid, password, data);
	Singleton<FileClient>::instance().sendMessage(&data);
}

void FileSession::uploadFile(const std::string& filePath, const std::string& from, const std::string& to, uint32_t seq)
{
	ByteArray data;
	packUploadFile(filePath, from, to, data, seq);
	Singleton<FileClient>::instance().sendMessage(&data);
}

void FileSession::downloadFile(const std::string& filename, const std::string& from, const std::string& to, uint32_t seq)
{
	LOG_DEBUG << "downloadFile: " << filename << ", from: " << from << ", to: " << to << ", seq: " << seq;
	ByteArray data;
	packDownloadFile(filename, from, to, data, seq);
	Singleton<FileClient>::instance().sendMessage(&data);
}

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
	uint32_t seq = 0;
	uint32_t length = dataStr.length();
	uint64_t crc = crc64(0, dataStr);

	data.appendUint32(magic);
	data.appendUint32(cmd);
	data.appendUint32(seq);
	data.appendUint32(length);
	data.append(dataStr);
	data.appendUint64(crc);
}

void FileSession::packLogout(std::string& userid, std::string& password, ByteArray& data)
{
}

void FileSession::packUploadFile(const std::string& filePath, const std::string& from, const std::string& to, ByteArray& data, uint32_t seq)
{
	// {"from": "", "to": "", "filename": "", "filesize": "", "md5":"", "data":""}
	//�����ļ�·����ȡ�ļ���
	std::string filename = filePath.substr(filePath.find_last_of("/") + 1);
	std::string fileData;
	uint32_t filesize = Singleton<FileManager>::instance().ReadFile(filePath, fileData);
	// �����ļ���md5 
	char md5[33] = { 0 };
	Singleton<FileManager>::instance().GetFileMd5(fileData, md5);

	QJsonObject json;
	json.insert("from", QString::fromStdString(from));
	json.insert("to", QString::fromStdString(to));
	json.insert("filename", QString::fromStdString(filename));
	json.insert("data", QString::fromStdString(fileData));
	LOG_DEBUG << "filesize: " << filesize;

	QJsonDocument doc(json);
	QByteArray bytes = doc.toJson(QJsonDocument::Compact);
	std::string dataStr = bytes.toStdString();

	uint32_t magic = MAGIC;
	uint32_t cmd = FileCmd::FILE_CMD_UPLOAD;
	uint32_t length = dataStr.length();
	uint64_t crc = crc64(0, dataStr);

	data.appendUint32(magic);
	data.appendUint32(cmd);
	data.appendUint32(seq);
	data.appendUint32(length);
	data.append(dataStr);
	data.appendUint64(crc);

	LOG_DEBUG << "packUploadFile: " << fileData.size();
}

void FileSession::packDownloadFile(const std::string& filename, const std::string& from, const std::string& to, ByteArray& data, uint32_t seq)
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
	data.appendUint32(seq);
	data.appendUint32(length);
	data.append(dataStr);
	data.appendUint64(crc);

	LOG_DEBUG << "packDownloadFile: " << dataStr.size();

}

void FileSession::packDownloadFileAck(ByteArray& data)
{
}


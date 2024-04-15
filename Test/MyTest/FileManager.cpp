#include "FileManager.h"
#include "hpath.h"

#include <QDir>
#include <QDebug>

bool FileManager::Init(const std::string& basePath)
{
	basePath_ = basePath;
	// 检查目录是否存在 Qt
	QDir dir(QString::fromStdString(basePath_));
	if (!dir.exists()) {
		dir.mkpath(QString::fromStdString(basePath_));
	}
	
	return true;
}

bool FileManager::IsFileExist(const std::string& filename)
{
	// 检查文件是否存在 Qt
	std::string filepath = GetAbsolutePath(filename);
	QFileInfo file(QString::fromStdString(filepath));
	return file.exists();
}

bool FileManager::SaveFile(const std::string& filename, const std::string& data, bool isBase64)
{
	if (isBase64) {
		// base64解码
		QByteArray ba = QByteArray::fromBase64(QByteArray::fromStdString(data));
		return SaveFile(filename, ba, false);
	}
	else {
		return SaveFile(filename, QByteArray::fromStdString(data), false);
	}
}

bool FileManager::SaveFile(const std::string& filename, const QByteArray& data, bool isBase64)
{
	// 保存文件 Qt
	std::string filepath = GetAbsolutePath(filename);
	QFile file(QString::fromStdString(filepath));
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	if (isBase64) {
		// base64解码
		QByteArray ba = QByteArray::fromBase64(data);
		file.write(ba);
	}
	else {
		file.write(data);
	}
	file.close();
	return true;
}

uint32_t FileManager::ReadFile(const std::string& filepath, std::string& data)
{
	QFile file(QString::fromStdString(filepath));
	if (!file.open(QIODevice::ReadOnly)) {
		return 0;
	}
	data = file.readAll().toBase64().toStdString();
	file.close();
	return data.size();
}


std::string FileManager::GetAbsolutePath(const std::string& fileName)
{
	// 获取绝对路径 Qt
	return basePath_ + "/" + fileName;
}

void FileManager::GetFileMd5(const std::string& data, char* md5)
{
	// 计算文件的MD5值
	char md[33] = { 0 };
	hv_md5_hex((unsigned char*)data.c_str(), data.size(), md, sizeof(md));
	memcpy(md5, md, 33);
}



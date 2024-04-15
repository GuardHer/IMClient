#pragma once

#include <QObject>
#include <vector>
#include "md5.h"


class FileManager final : public QObject
{
	Q_OBJECT
public:
	FileManager() = default;
	~FileManager() = default;

	// 初始化
	bool Init(const std::string& basePath);
	// 检查文件是否存在
	bool IsFileExist(const std::string& filename);
	// 保存文件, isBase64为true表示data是base64编码
	bool SaveFile(const std::string& filename, const std::string& data, bool isBase64);
	bool SaveFile(const std::string& filename, const QByteArray& data, bool isBase64);
	// 读取文件, 并进行base64编码
	uint32_t ReadFile(const std::string& filepath, std::string& data);

	std::string GetAbsolutePath(const std::string& fileName);
	std::string GetBasePath() const { return basePath_; }

	// 获取文件md5, data是指base64编码后的数据
	void GetFileMd5(const std::string& data, char* md5);

private:
	std::string basePath_;
};


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

	// ��ʼ��
	bool Init(const std::string& basePath);
	// ����ļ��Ƿ����
	bool IsFileExist(const std::string& filename);
	// �����ļ�, isBase64Ϊtrue��ʾdata��base64����
	bool SaveFile(const std::string& filename, const std::string& data, bool isBase64);
	bool SaveFile(const std::string& filename, const QByteArray& data, bool isBase64);
	// ��ȡ�ļ�, ������base64����
	uint32_t ReadFile(const std::string& filepath, std::string& data);

	std::string GetAbsolutePath(const std::string& fileName);
	std::string GetBasePath() const { return basePath_; }

	// ��ȡ�ļ�md5, data��ָbase64����������
	void GetFileMd5(const std::string& data, char* md5);

private:
	std::string basePath_;
};


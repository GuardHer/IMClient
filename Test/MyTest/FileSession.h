#pragma once

#include "Singleton.h"
#include "ByteArray.h"
#include "crc64.h"

#include "md5.h"


class FileSession
{
public:
	friend class Singleton<FileSession>;



public:
	void packLogin(const std::string& userid, const std::string& password, ByteArray& data);
	void packLogout(std::string& userid, std::string& password, ByteArray& data);
	void packUploadFile(const std::string& filePath, const std::string& from, const std::string& to, ByteArray& data);
	void packDownloadFile(const std::string& filename, const std::string& from, const std::string& to, ByteArray& data);
	void packDownloadFileAck(ByteArray& data);


private:
	FileSession() = default;
	~FileSession() = default;
};


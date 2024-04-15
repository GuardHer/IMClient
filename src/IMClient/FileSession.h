#pragma once

#include "base/Singleton.h"
#include "base/noncopyable.h"
#include "base/crc64.h"

#include "md5.h"
#include "ByteArray.h"

class FileSession : public noncopyable
{
public:
	friend class Singleton<FileSession>;

public:
	static void login(const std::string& userid, const std::string& password);
	static void uploadFile(const std::string& filePath, const std::string& from, const std::string& to, uint32_t seq);
	static void downloadFile(const std::string& filename, const std::string& from, const std::string& to, uint32_t seq);

public:
	static void packLogin(const std::string& userid, const std::string& password, ByteArray& data);
	static void packLogout(std::string& userid, std::string& password, ByteArray& data);
	static void packUploadFile(const std::string& filePath, const std::string& from, const std::string& to, ByteArray& data, uint32_t seq);
	static void packDownloadFile(const std::string& filename, const std::string& from, const std::string& to, ByteArray& data, uint32_t seq);
	static void packDownloadFileAck(ByteArray& data);


private:
	FileSession() = default;
	~FileSession() = default;
};


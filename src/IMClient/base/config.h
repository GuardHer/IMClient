#pragma once

#include <string>

#define SERVER_IP "192.168.20.132"
#define SERVER_PORT 9999
#define FILE_SERVER_PORT 8888

//#define EMOJI_PATH "\\res\\emoji\\assets"
#define ABSOLUTE_FILE_PATH() (std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\")))


class Config
{
public:
	Config() = delete;
	~Config() = delete;

	static void initPath(const std::string& mainPath);
	static std::string getEmojiPath();
	static std::string getFilesPath();

public:
	static std::string PROJECT_PATH;
	static std::string EMOJI_PATH;
	static std::string FILES_PATH;
};



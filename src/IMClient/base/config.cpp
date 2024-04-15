#include "config.h"

std::string Config::PROJECT_PATH = "";
std::string Config::EMOJI_PATH = "";
std::string Config::FILES_PATH = "";


void Config::initPath(const std::string& mainPath)
{
	Config::PROJECT_PATH = mainPath;
	Config::EMOJI_PATH = Config::PROJECT_PATH + "/res/emoji/assets";
	Config::FILES_PATH = Config::PROJECT_PATH + "/File/ChatFiles";
}

std::string Config::getEmojiPath()
{
	return Config::EMOJI_PATH;
}

std::string Config::getFilesPath()
{
	return Config::FILES_PATH;
}

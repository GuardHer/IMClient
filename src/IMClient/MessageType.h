#pragma once
#include <QString>

class MessageType
{
public:
	enum Type
	{
		Text = 1,  //文本消息 
		Image = 3, //图片消息
		EmojiMsg = 47,     //表情消息
		File,  //文件消息
		Voice, //语音消息
		Video, //视频消息
		ShareCard, //分享名片
		System, //系统消息
		Undefine //未知消息

	};
};



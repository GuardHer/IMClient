#pragma once
#include <QString>

class MessageType
{
public:
	enum Type
	{
		Text = 1,  //�ı���Ϣ 
		Image = 3, //ͼƬ��Ϣ
		EmojiMsg = 47,     //������Ϣ
		File,  //�ļ���Ϣ
		Voice, //������Ϣ
		Video, //��Ƶ��Ϣ
		ShareCard, //������Ƭ
		System, //ϵͳ��Ϣ
		Undefine //δ֪��Ϣ

	};
};



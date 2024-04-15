#pragma once

#include "Message.h"
#include <vector>

class MessageCache
{
public:
	MessageCache();
	~MessageCache();

public:
	static void addMessageCache(const Message& message);                       // �����Ϣ������
	static void clearMessageCache();                                           // �����Ϣ����
	static void setMaxMessageCacheSize(int size);                              // ������Ϣ�������ֵ
	static int getMaxMessageCacheSize();                                       // ��ȡ��Ϣ�������ֵ
	static int getMessageCacheSize();                                          // ��ȡ��Ϣ�����С
	static std::vector<Message> getMessageCache();                             // ��ȡ��Ϣ����
	static void removeOneMessageCache(const Message& message);                                           // �Ƴ���Ϣ����

private:
	static std::vector<Message> m_messageCache;                                // ��Ϣ����
	static int m_maxMessageCacheSize;                                           // ��Ϣ�������ֵ
};


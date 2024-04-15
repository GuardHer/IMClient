#include "MessageCache.h"

// 初始化静态成员变量
std::vector<Message> MessageCache::m_messageCache;
int MessageCache::m_maxMessageCacheSize = 5;

MessageCache::MessageCache()
{
}

MessageCache::~MessageCache()
{

}

void MessageCache::addMessageCache(const Message& message)
{
	m_messageCache.push_back(message);
}

void MessageCache::clearMessageCache()
{
	m_messageCache.clear();
}

void MessageCache::setMaxMessageCacheSize(int size)
{
	m_maxMessageCacheSize = size;
}

int MessageCache::getMaxMessageCacheSize()
{
	return m_maxMessageCacheSize;
}

int MessageCache::getMessageCacheSize()
{
	return m_messageCache.size();
}

std::vector<Message> MessageCache::getMessageCache()
{
	return m_messageCache;
}

void MessageCache::removeOneMessageCache(const Message& message)
{
	for (auto it = m_messageCache.begin(); it != m_messageCache.end(); ++it)
	{
		if (*it == message)
		{
			m_messageCache.erase(it);
			break;
		}
	}
}

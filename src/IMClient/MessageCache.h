#pragma once

#include "Message.h"
#include <vector>

class MessageCache
{
public:
	MessageCache();
	~MessageCache();

public:
	static void addMessageCache(const Message& message);                       // 添加消息到缓存
	static void clearMessageCache();                                           // 清空消息缓存
	static void setMaxMessageCacheSize(int size);                              // 设置消息缓存最大值
	static int getMaxMessageCacheSize();                                       // 获取消息缓存最大值
	static int getMessageCacheSize();                                          // 获取消息缓存大小
	static std::vector<Message> getMessageCache();                             // 获取消息缓存
	static void removeOneMessageCache(const Message& message);                                           // 移除消息缓存

private:
	static std::vector<Message> m_messageCache;                                // 消息缓存
	static int m_maxMessageCacheSize;                                           // 消息缓存最大值
};


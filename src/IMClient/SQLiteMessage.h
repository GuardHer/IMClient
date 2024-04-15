#pragma once

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>  
#include <QVariant>  
#include <QVector>
#include <QMap>
#include <QPair>

#include "Message.h"

class SQLiteMessage
{
public:
	SQLiteMessage();
	~SQLiteMessage();
public:
	void addMessageCache(const Message& message);                       // 添加消息到缓存
	void clearMessageCache();                                           // 清空消息缓存
	void setMaxMessageCacheSize(int size);                              // 设置消息缓存最大值
	int getMaxMessageCacheSize();                                       // 获取消息缓存最大值
	int getMessageCacheSize();                                          // 获取消息缓存大小
	std::vector<Message> getMessageCache();                                 // 获取消息缓存


private:
	QSqlDatabase createDatabase();                                             // 创建数据库
	bool createTable(const std::vector<QString>& tables, const std::map<QString, QString>& tableSql);                                                // 创建表
	bool isTableExist(const QString& tableName);                       // 判断表是否存在
	bool isTableEmpty(const QString& tableName);                       // 判断表是否为空
	std::vector<QString> getTables();                                                  // 获取所有表名

public:

	/***************** Init ******************/
	void initTb(std::vector<QString>& tables, std::map<QString, QString>& tableSql); // 初始化表

	/********************* 查询 ************************/
	//<< msg, type > , IsSender >
	std::vector<std::pair<std::pair<QString, int>, int>> getFriendMessagesText(const QString& talker);            // 获取消息，根据talker, 根据时间排序
	std::vector<std::pair<std::pair<QByteArray, int>, int>> getFriendMessagesBinary(const QString& talker);       // 获取消息，根据talker和type, 根据时间排序
	std::vector<std::pair<std::pair<QString, int>, int>> getGroupMessagesText(const QString& talker);             // 获取消息，根据talker, 根据时间排序
	std::vector<std::pair<std::pair<QByteArray, int>, int>> getGroupMessagesBinary(const QString& talker);        // 获取消息，根据talker和type, 根据时间排序
	std::vector<QString> getAllMessages();                                                    // 获取所有消息
	std::vector<QString> getFriendMessagesByType(const QString& talker, MessageType::Type type);          // 获取消息，根据talker和type
	std::vector<QString> getFriendMessagesByKeyword(const QString& talker, const QString& keyword); // 获取消息，根据talker和keyword, 根据时间排序
	std::vector<QString> getMessagesCalendar(const QString& talker);                          // 获取消息日历，根据talker
	std::pair<std::pair<QString, int>, int> getLastMessage(const QString& talker);                                        // 获取最后一条消息，根据talker

	/********************* 插入 ************************/
	bool insertOneMessage(const Message& message);                                        // 插入消息，根据talker
	bool insertMultiMessage();                            // 插入多条消息，根据talker

	
};

/*
-- message.db
CREATE TABLE Msg (
    localId INTEGER PRIMARY KEY AUTOINCREMENT,   -- 本地消息 ID，自增长
    TalkerId INT DEFAULT 0,                      -- 对话者 ID，默认值为 0
    StrTalker TEXT,                              -- 对话者的字符串表示
    Type INT,                                    -- 消息类型
    IsSender INT,                                -- 是否发送者，1 表示是发送者
    Status INT,                                  -- 消息状态
    StrContent TEXT,                             -- 消息内容的字符串表示
    ByteContent BLOB,                            -- 压缩后的消息内容（二进制数据）
    IsFriend INT,                                -- 是否是好友消息
    CreateTime TEXT                              -- 消息创建时间
);

*/

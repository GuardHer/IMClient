#include "SQLiteMessage.h"

#include <QDir>
#include <QThread>
#include "base/Logging.h"
#include "MessageCache.h"

SQLiteMessage::SQLiteMessage()
{
}

SQLiteMessage::~SQLiteMessage()
{
}

void SQLiteMessage::addMessageCache(const Message& message)
{
	MessageCache::addMessageCache(message);

	if (getMessageCacheSize() > getMaxMessageCacheSize())
	{
		insertMultiMessage();
	}
}

void SQLiteMessage::clearMessageCache()
{
	MessageCache::clearMessageCache();
}

void SQLiteMessage::setMaxMessageCacheSize(int size)
{
	MessageCache::setMaxMessageCacheSize(size);
}

int SQLiteMessage::getMaxMessageCacheSize()
{
	return MessageCache::getMaxMessageCacheSize();
}

int SQLiteMessage::getMessageCacheSize()
{
	return MessageCache::getMessageCacheSize();
}

std::vector <Message> SQLiteMessage::getMessageCache()
{
	return MessageCache::getMessageCache();
}

QSqlDatabase SQLiteMessage::createDatabase()
{
	QSqlDatabase db;
	QString connectionName = QString::number(reinterpret_cast<std::uintptr_t>(QThread::currentThreadId()));


	if (QSqlDatabase::contains(connectionName))
	{
		db = QSqlDatabase::database(connectionName);
	}
	else
	{
		db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
		db.setDatabaseName("E:\\StudyVideo\\IMClient\\src\\IMClient\\File\\1291035260\\Msg\\Message.db");
	}

	if (!db.open())
	{
		LOG_DEBUG << "Error: Failed to connect database." << db.lastError();
	}

	return db;
}

bool SQLiteMessage::createTable(const std::vector<QString>& tables, const std::map<QString, QString>& tableSql)
{
	QSqlDatabase db = createDatabase();

	// 检测表是否存在
	for (QString table : tables) {
		if (!isTableExist(table)) {
			QSqlQuery query(db);
			//QString sql = createTableSqlMap[table.toString()];

			query.prepare(tableSql.at(table));
			if (!query.exec())
			{
				LOG_DEBUG << ("Error: Fail to create table:{}, error:{}", table , query.lastError());
				return false;
			}
			else
			{
				LOG_DEBUG << ("Table created: {}", table );
			}
			query.finish();
		}
	}

	if (db.isOpen()) db.close();

	return true;
}

bool SQLiteMessage::isTableExist(const QString& tableName)
{

	QSqlDatabase db = createDatabase();

	// 检测表是否存在
	QSqlQuery query(db);
	QString sql = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1'").arg(tableName);
	int cnt = 0;
	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error:{}", tableName , query.lastError());
		return false;
	}
	else
	{
		query.next();
		cnt = query.value(0).toInt();
	}

	if (db.isOpen()) db.close();

	return cnt == 1;
}


bool SQLiteMessage::isTableEmpty(const QString& tableName)
{
	QSqlDatabase db = createDatabase();

	// 检测表是否为空
	QSqlQuery query(db);
	QString sql = QString("SELECT COUNT(*) FROM %1").arg(tableName);

	int cnt = 0;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error:{}", tableName , query.lastError());
		return false;
	}
	else
	{
		query.next();
		cnt = query.value(0).toInt();
	}

	if (db.isOpen()) db.close();

	return cnt == 0;
}

std::vector<QString> SQLiteMessage::getTables()
{
	QSqlDatabase db = createDatabase();

	// 获取所有表名
	QSqlQuery query(db);
	QString sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name";
	std::vector<QString> tables;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error:{}", "Msg", query.lastError());
		return std::vector<QString>();
	}
	else
	{
		while (query.next())
		{
			QString table = query.value(0).toString();
			tables.push_back(table);
		}
	}

	if (db.isOpen()) db.close();

	return tables;
}


void SQLiteMessage::initTb(std::vector<QString>& tables, std::map<QString, QString>& tableSql)
{
	tables.push_back("Msg");
	tableSql["Msg"] = "CREATE TABLE Msg ( \
		localId INTEGER PRIMARY KEY AUTOINCREMENT, \
		TalkerId INT DEFAULT 0, \
		StrTalker TEXT, \
		Type INT, \
		IsSender INT, \
		Status INT,  \
		StrContent TEXT, \
		ByteContent BLOB, \
		IsFriend INT, \
		CreateTime TEXT \
		); ";
}

std::vector<std::pair<std::pair<QString, int>, int>> SQLiteMessage::getFriendMessagesText(const QString& talker)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT StrContent, Type, IsSender FROM Msg WHERE StrTalker = '%1' AND IsFriend = 1 ORDER BY CreateTime ASC;").arg(talker);

	std::vector<std::pair<std::pair<QString, int>, int>> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<std::pair<std::pair<QString, int>, int>>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			int type = query.value(1).toInt();
			int isSender = query.value(2).toInt();
			messages.push_back(std::pair<std::pair<QString, int>, int>(std::pair<QString, int>(content, type), isSender));
		}
	}

	if (db.isOpen()) db.close();

	return messages;

}

std::vector<std::pair<std::pair<QByteArray, int>, int>> SQLiteMessage::getFriendMessagesBinary(const QString& talker)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT ByteContent, Type, IsSender FROM Msg WHERE StrTalker = '%1' AND IsFriend = 1 ORDER BY CreateTime ASC;").arg(talker);

	std::vector<std::pair<std::pair<QByteArray, int>, int>> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<std::pair<std::pair<QByteArray, int>, int>>();
	}
	else
	{
		while (query.next())
		{
			QByteArray content = query.value(0).toByteArray();
			int type = query.value(1).toInt();
			int isSender = query.value(2).toInt();
			messages.push_back(std::pair<std::pair<QByteArray, int>, int>(std::pair<QByteArray, int>(content, type), isSender));
		}
	}

	if (db.isOpen()) db.close();

	return messages;
}

std::vector<std::pair<std::pair<QString, int>, int>> SQLiteMessage::getGroupMessagesText(const QString& talker)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT StrContent, Type, IsSender FROM Msg WHERE StrTalker = '%1' AND IsFriend = 0 ORDER BY CreateTime ASC;").arg(talker);

	std::vector<std::pair<std::pair<QString, int>, int>> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<std::pair<std::pair<QString, int>, int>>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			int type = query.value(1).toInt();
			int isSender = query.value(2).toInt();
			messages.push_back(std::pair<std::pair<QString, int>, int>(std::pair<QString, int>(content, type), isSender));
		}
	}

	if (db.isOpen()) db.close();

	return messages;
}

std::vector<std::pair<std::pair<QByteArray, int>, int>> SQLiteMessage::getGroupMessagesBinary(const QString& talker)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT ByteContent, Type, IsSender FROM Msg WHERE StrTalker = '%1' AND IsFriend = 0 ORDER BY CreateTime ASC;").arg(talker);

	std::vector<std::pair<std::pair<QByteArray, int>, int>> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<std::pair<std::pair<QByteArray, int>, int>>();
	}
	else
	{
		while (query.next())
		{
			QByteArray content = query.value(0).toByteArray();
			int type = query.value(1).toInt();
			int isSender = query.value(2).toInt();
			messages.push_back(std::pair<std::pair<QByteArray, int>, int>(std::pair<QByteArray, int>(content, type), isSender));
		}
	}

	if (db.isOpen()) db.close();

	return messages;

}


std::vector<QString> SQLiteMessage::getAllMessages()
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT StrContent FROM Msg ORDER BY CreateTime ASC;");
	std::vector<QString> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<QString>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			messages.push_back(content);
		}
	}

	if (db.isOpen()) db.close();

	return messages;
}

std::vector<QString> SQLiteMessage::getFriendMessagesByType(const QString& talker, MessageType::Type type)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT StrContent FROM Msg WHERE StrTalker = '%1' AND IsFriend = 1 AND Type = %2 ORDER BY CreateTime ASC;").arg(talker).arg(type);
	std::vector<QString> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<QString>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			messages.push_back(content);
		}
	}

	if (db.isOpen()) db.close();

	return messages;
}

std::vector<QString> SQLiteMessage::getFriendMessagesByKeyword(const QString& talker, const QString& keyword)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT StrContent FROM Msg WHERE StrTalker = '%1' AND IsFriend = 1 AND StrContent LIKE '%%2%' ORDER BY CreateTime ASC;").arg(talker).arg(keyword);
	std::vector<QString> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<QString>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			messages.push_back(content);
		}
	}

	if (db.isOpen()) db.close();

	return messages;
}

std::vector<QString> SQLiteMessage::getMessagesCalendar(const QString& talker)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);

	QString sql = QString("SELECT CreateTime FROM Msg WHERE StrTalker = '%1' ORDER BY CreateTime ASC;").arg(talker);
	std::vector<QString> messages;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::vector<QString>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			messages.push_back(content);
		}
	}

	if (db.isOpen()) db.close();

	return messages;
}

std::pair<std::pair<QString, int>, int> SQLiteMessage::getLastMessage(const QString& talker)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sql = QString("SELECT StrContent, Type, IsSender FROM Msg WHERE StrTalker = '%1' ORDER BY CreateTime DESC LIMIT 1;").arg(talker);

	std::pair<std::pair<QString, int>, int> message;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error: {}", "Msg", query.lastError());
		return std::pair<std::pair<QString, int>, int>();
	}
	else
	{
		while (query.next())
		{
			QString content = query.value(0).toString();
			int type = query.value(1).toInt();
			int isSender = query.value(2).toInt();
			message = std::pair<std::pair<QString, int>, int>(std::pair<QString, int>(content, type), isSender);
		}
	}

	if (db.isOpen()) db.close();

	return message;
}

bool SQLiteMessage::insertOneMessage(const Message& message)
{
	QSqlDatabase db = createDatabase();

	QSqlQuery query(db);
	QString sqlQuery = "INSERT INTO Msg (TalkerId, StrTalker, Type, IsSender, Status, StrContent, ByteContent, IsFriend, CreateTime) "
		"VALUES (:talkerId, :strTalker, :type, :isSender, :status, :strContent, :byteContent, :isFriend, :createTime)";
	query.prepare(sqlQuery);

	query.bindValue(":talkerId", message.getTalker().toInt());
	query.bindValue(":strTalker", message.getTalker());
	query.bindValue(":type", message.getType());
	query.bindValue(":isSender", message.getIsSender());
	query.bindValue(":status", message.getStatus());
	query.bindValue(":strContent", message.getContent());
	query.bindValue(":byteContent", message.getByteContent());
	query.bindValue(":isFriend", message.getIsFriend());
	query.bindValue(":createTime", message.getStrTime());

	if (!query.exec())
	{
		LOG_DEBUG << ("Error: Fail to insert message:{}, error: {}", message.getContent() , query.lastError());
		return false;
	}
	else
	{
		LOG_DEBUG << ("Succeed to insert message.");
	}

	if (db.isOpen()) db.close();

	return true;

}

bool SQLiteMessage::insertMultiMessage()
{
	QSqlDatabase db = createDatabase();

	std::vector messagesCache = MessageCache::getMessageCache();

	if (messagesCache.size() == 0) {
		return false;
	}

	QSqlQuery query(db);
	QString sqlQuery = "INSERT INTO Msg (TalkerId, StrTalker, Type, IsSender, Status, StrContent, ByteContent, IsFriend, CreateTime) "
		"VALUES (:talkerId, :strTalker, :type, :isSender, :status, :strContent, :byteContent, :isFriend, :createTime)";
	query.prepare(sqlQuery);

	for (const Message& message : messagesCache) {
		query.bindValue(":talkerId", message.getTalker().toInt());
		query.bindValue(":strTalker", message.getTalker());
		query.bindValue(":type", message.getType());
		query.bindValue(":isSender", message.getIsSender());
		query.bindValue(":status", message.getStatus());
		query.bindValue(":strContent", message.getContent());
		query.bindValue(":byteContent", message.getByteContent());
		query.bindValue(":isFriend", message.getIsFriend());
		query.bindValue(":createTime", message.getStrTime());

		if (!query.exec())
		{
			LOG_DEBUG << ("Error: Fail to insert message:{}, error: {}", message.getContent() , query.lastError());
			return false;
		}
		else
		{
			LOG_DEBUG << ("Succeed to insert message.");
			// 插入成功，删除缓存
			MessageCache::removeOneMessageCache(message);
		}
	}

	if (db.isOpen()) db.close();

	return true;
}

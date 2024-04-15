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
	void addMessageCache(const Message& message);                       // �����Ϣ������
	void clearMessageCache();                                           // �����Ϣ����
	void setMaxMessageCacheSize(int size);                              // ������Ϣ�������ֵ
	int getMaxMessageCacheSize();                                       // ��ȡ��Ϣ�������ֵ
	int getMessageCacheSize();                                          // ��ȡ��Ϣ�����С
	std::vector<Message> getMessageCache();                                 // ��ȡ��Ϣ����


private:
	QSqlDatabase createDatabase();                                             // �������ݿ�
	bool createTable(const std::vector<QString>& tables, const std::map<QString, QString>& tableSql);                                                // ������
	bool isTableExist(const QString& tableName);                       // �жϱ��Ƿ����
	bool isTableEmpty(const QString& tableName);                       // �жϱ��Ƿ�Ϊ��
	std::vector<QString> getTables();                                                  // ��ȡ���б���

public:

	/***************** Init ******************/
	void initTb(std::vector<QString>& tables, std::map<QString, QString>& tableSql); // ��ʼ����

	/********************* ��ѯ ************************/
	//<< msg, type > , IsSender >
	std::vector<std::pair<std::pair<QString, int>, int>> getFriendMessagesText(const QString& talker);            // ��ȡ��Ϣ������talker, ����ʱ������
	std::vector<std::pair<std::pair<QByteArray, int>, int>> getFriendMessagesBinary(const QString& talker);       // ��ȡ��Ϣ������talker��type, ����ʱ������
	std::vector<std::pair<std::pair<QString, int>, int>> getGroupMessagesText(const QString& talker);             // ��ȡ��Ϣ������talker, ����ʱ������
	std::vector<std::pair<std::pair<QByteArray, int>, int>> getGroupMessagesBinary(const QString& talker);        // ��ȡ��Ϣ������talker��type, ����ʱ������
	std::vector<QString> getAllMessages();                                                    // ��ȡ������Ϣ
	std::vector<QString> getFriendMessagesByType(const QString& talker, MessageType::Type type);          // ��ȡ��Ϣ������talker��type
	std::vector<QString> getFriendMessagesByKeyword(const QString& talker, const QString& keyword); // ��ȡ��Ϣ������talker��keyword, ����ʱ������
	std::vector<QString> getMessagesCalendar(const QString& talker);                          // ��ȡ��Ϣ����������talker
	std::pair<std::pair<QString, int>, int> getLastMessage(const QString& talker);                                        // ��ȡ���һ����Ϣ������talker

	/********************* ���� ************************/
	bool insertOneMessage(const Message& message);                                        // ������Ϣ������talker
	bool insertMultiMessage();                            // ���������Ϣ������talker

	
};

/*
-- message.db
CREATE TABLE Msg (
    localId INTEGER PRIMARY KEY AUTOINCREMENT,   -- ������Ϣ ID��������
    TalkerId INT DEFAULT 0,                      -- �Ի��� ID��Ĭ��ֵΪ 0
    StrTalker TEXT,                              -- �Ի��ߵ��ַ�����ʾ
    Type INT,                                    -- ��Ϣ����
    IsSender INT,                                -- �Ƿ����ߣ�1 ��ʾ�Ƿ�����
    Status INT,                                  -- ��Ϣ״̬
    StrContent TEXT,                             -- ��Ϣ���ݵ��ַ�����ʾ
    ByteContent BLOB,                            -- ѹ�������Ϣ���ݣ����������ݣ�
    IsFriend INT,                                -- �Ƿ��Ǻ�����Ϣ
    CreateTime TEXT                              -- ��Ϣ����ʱ��
);

*/

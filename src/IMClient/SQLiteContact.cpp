#include "SQLiteContact.h"

#include <QDir>
#include <QThread>
#include <cassert>

#include "UserList.h"
#include "GroupList.h"
#include "Status.h"

#include "base/Logging.h"


SQLiteContact::SQLiteContact()
{
}

SQLiteContact::~SQLiteContact()
{
}

QSqlDatabase SQLiteContact::createDatabase()
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
		db.setDatabaseName("E:\\StudyVideo\\IMClient\\src\\IMClient\\File\\1291035260\\Contact\\Contact.db");
	}

	if (!db.open())
	{
		LOG_DEBUG << "Error: Failed to connect database." << db.lastError();
	}


	return db;

}

bool SQLiteContact::createTable(const std::vector<QString>& tables, const std::map<QString, QString>& tableSql)
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
				LOG_DEBUG << ("Error: Fail to create table:{}, error:{}", table, query.lastError());
				return false;
			}
			else
			{
				LOG_DEBUG << ("Table created: {}", table);
			}
			query.finish();
		}
	}

	if (db.isOpen()) db.close();

	return true;
}

bool SQLiteContact::isTableExist(const QString& tableName)
{
	QSqlDatabase db = createDatabase();

	// 检测表是否存在
	QSqlQuery query(db);
	QString sql = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1'").arg(tableName);
	int cnt = 0;
	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error:{}", tableName, query.lastError());
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

bool SQLiteContact::isTableEmpty(const QString& tableName)
{
	QSqlDatabase db = createDatabase();

	// 检测表是否为空
	QSqlQuery query(db);
	QString sql = QString("SELECT COUNT(*) FROM %1").arg(tableName);

	int cnt = 0;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error:{}", tableName, query.lastError() );
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


void SQLiteContact::initTb(std::vector<std::string>& tables, std::map<std::string, std::string> tableSql)
{
	tables.push_back("FriendInfo");
	tables.push_back("GroupInfo");
	tables.push_back("ContactHeadImg");

	tableSql["FriendInfo"] = "CREATE TABLE `FriendInfo` ( \
		LocalId INTEGER PRIMARY KEY AUTOINCREMENT, \
		FriendId INTEGER NOT NULL, \
		FriendName TEXT NOT NULL, \
		PassWord TEXT NOT NULL, \
		FaceType INTEGER DEFAULT 0, \
		CustomFace TEXT DEFAULT NULL, \
		CustomFaceFmt TEXT DEFAULT NULL, \
		Gender INTEGER DEFAULT 0, \
		Birthday INTEGER DEFAULT 0, \
		Signature TEXT DEFAULT NULL, \
		Address TEXT DEFAULT NULL, \
		PhoneNumber TEXT DEFAULT NULL, \
		Mail TEXT DEFAULT NULL, \
		OwnerId INTEGER DEFAULT 0, \
		RegisterTime DATETIME NOT NULL, \
		Remark TEXT DEFAULT NULL,\
		UpdateTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, \
		Status INT DEFAULT 0 \
		); ";
	tableSql["GroupInfo"] = "CREATE TABLE `GroupInfo` ( \
		LocalId INTEGER PRIMARY KEY AUTOINCREMENT, \
		GroupId INTEGER NOT NULL, \
		GroupName TEXT NOT NULL, \
		OwnerId INTEGER NOT NULL, \
		OwnerName TEXT NOT NULL, \
		Notice TEXT DEFAULT NULL, \
		MemberCnt INTEGER DEFAULT 0, \
		UserIdList TEXT DEFAULT NULL, \
		DisplayNameList TEXT DEFAULT NULL, \
		SelfDisplayName TEXT DEFAULT NULL, \
		CreateTime DATETIME NOT NULL, \
		UpdateTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, \
		CustomFace TEXT DEFAULT NULL  \
		); ";
	tableSql["ContactHeadImg"] = "CREATE TABLE `ContactHeadImg` ( \
		LocalId INTEGER PRIMARY KEY AUTOINCREMENT, \
		ContactId INTEGER NOT NULL, \
		IsFriend INT NOT NULL, \
		SmallHeadImgUrl TEXT, \
		BigHeadImgUrl TEXT, \
		HeadImgMd5 TEXT, \
		HeadImgData BLOB \
		); ";
}

std::vector<QString> SQLiteContact::getTables()
{
	QSqlDatabase db = createDatabase();

	// 获取所有表名
	QSqlQuery query(db);
	QString sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name";
	std::vector<QString> tables;

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, error:{}", "sqlite_master", query.lastError() );
	}
	else
	{
		while (query.next()) {
			tables.push_back(query.value(0).toString());
		}
	}

	if (db.isOpen()) db.close();

	return tables;
}

bool SQLiteContact::getAllFriendInfo()
{
	QSqlDatabase db = createDatabase();


	// 查询所有好友信息
	QSqlQuery query(db);
	QString sql = QString("SELECT * FROM FriendInfo");

	if (!query.exec(sql))
	{
		LOG_DEBUG << "Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() ;
		return false;
	}
	else
	{
		while (query.next()) {
			QVariant FriendId = query.value("FriendId");
			QVariant FriendName = query.value("FriendName");
			User *user = UserList::addFriend(FriendId.toString(), FriendName.toString());
			user->setPassword(query.value("PassWord").toString());
			user->setFaceType(query.value("FaceType").toString());
			user->setCustomFace(query.value("CustomFace").toString());
			user->setCustomFaceFmt(query.value("CustomFaceFmt").toString());
			user->setGender(query.value("Gender").toString());
			user->setBirthday(query.value("Birthday").toString());
			user->setSignature(query.value("Signature").toString());
			user->setAddress(query.value("Address").toString());
			user->setPhoneNumber(query.value("PhoneNumber").toString());
			user->setMail(query.value("Mail").toString());
			user->setOwnerId(query.value("OwnerId").toString());
			user->setRegisterTime(query.value("RegisterTime").toString());
			user->setStatus(query.value("Status").toInt());
		}
		assert(!UserList::isEmpty());
		LOG_DEBUG << "FriendInfo Loaded Successful!";
	}

	if (db.isOpen()) db.close();

	return true;
}

bool SQLiteContact::getAllGroupInfo()
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT * FROM GroupInfo");

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "GroupInfo", query.lastError() );
		return false;
	}
	else
	{
		while (query.next()) {
			QVariant GroupId = query.value("GroupId");
			QVariant GroupName = query.value("GroupName");
			Group *group = GroupList::addGroup(GroupId.toString(), GroupName.toString());
			group->updateGroupOwnerId(query.value("OwnerId").toString());
			group->updateGroupOwnerName(query.value("OwnerName").toString());
			group->setGroupNotice(query.value("Notice").toString());
			group->setGroupMemberCount(query.value("MemberCnt").toInt());
			group->setGroupMemberListForString(query.value("UserIdList").toString());
			group->setDisplayNameListForString(query.value("DisplayNameList").toString());
			group->setSelfDisplayName(query.value("SelfDisplayName").toString());
			group->setGroupCreateTime(query.value("CreateTime").toString());
			group->setCustomFace(query.value("CustomFace").toString());
		}
		assert(!GroupList::isEmpty());
		LOG_DEBUG << "GroupInfo Loaded Successful!";
	}

	if (db.isOpen()) db.close();

	return true;
}

QString SQLiteContact::getFriendNameById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT FriendName FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString name = query.value("FriendName").toString();

	if (db.isOpen()) db.close();

	return name;

}

QString SQLiteContact::getFriendCustomFaceById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT CustomFace FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString customFace = query.value("CustomFace").toString();

	if (db.isOpen()) db.close();

	return customFace;
}

QString SQLiteContact::getFriendCustomFaceFmtById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT CustomFaceFmt FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString customFaceFmt = query.value("CustomFaceFmt").toString();

	if (db.isOpen()) db.close();

	return customFaceFmt;

}

QString SQLiteContact::getFriendSignatureById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT Signature FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString signature = query.value("Signature").toString();

	if (db.isOpen()) db.close();

	return signature;
}

QString SQLiteContact::getFriendAddressById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT Address FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString address = query.value("Address").toString();

	if (db.isOpen()) db.close();

	return address;
}

QString SQLiteContact::getFriendPhoneNumberById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT PhoneNumber FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString phoneNumber = query.value("PhoneNumber").toString();

	if (db.isOpen()) db.close();

	return phoneNumber;
}

QString SQLiteContact::getFriendMailById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT Mail FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString mail = query.value("Mail").toString();

	if (db.isOpen()) db.close();

	return mail;
}

QString SQLiteContact::getFriendRemarkById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT Remark FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString remark = query.value("Remark").toString();

	if (db.isOpen()) db.close();

	return remark;
}

QString SQLiteContact::getFriendRegisterTimeById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT RegisterTime FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}

	QString registerTime = query.value("RegisterTime").toString();

	if (db.isOpen()) db.close();

	return registerTime;
}

QString SQLiteContact::getFriendUpdateTimeById(const QString& friendId)
{
	QSqlDatabase db = createDatabase();

	// 查询所有群信息
	QSqlQuery query(db);
	QString sql = QString("SELECT UpdateTime FROM FriendInfo WHERE FriendId = %1").arg(friendId);

	if (!query.exec(sql))
	{
		LOG_DEBUG << ("Error: Fail to query table:{}, errof:{}", "FriendInfo", query.lastError() );
		return QString();
	}
	else
	{
		query.next();
	}


	QString updateTime = query.value("UpdateTime").toString();

	if (db.isOpen()) db.close();

	return updateTime;
}

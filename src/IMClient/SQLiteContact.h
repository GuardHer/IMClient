#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>  
#include <QVector>
#include <QVariant> 
#include <QDateTime>
#include <QMap>
#include <vector>
#include <map>

#include "./manager/User.h"
#include "./base/noncopyable.h"
#include "./base/Singleton.h"
#include "Group.h"

class SQLiteContact
{
public:
	SQLiteContact();
	~SQLiteContact();

private:
    QSqlDatabase createDatabase();                        // 创建数据库

    bool createTable(const std::vector<QString>& tables, const std::map<QString, QString>& tableSql);                        // 创建表
    bool isTableExist(const QString& tableName);                       // 判断表是否存在
    bool isTableEmpty(const QString& tableName);                       // 判断表是否为空



public:

    /***************** Init ******************/
    void initTb(std::vector<std::string> &tables, std::map<std::string, std::string> tableSql);
    std::vector<QString> getTables();                                                  // 获取所有表名

    /***************** 查询 ******************/
    bool getAllFriendInfo();                  // 获取所有好友信息
    bool getAllGroupInfo();                   // 获取所有群组信息
    QString getFriendNameById(const QString& friendId);                // 通过好友ID获取好友名字
    QString getFriendCustomFaceById(const QString& friendId);          // 通过好友ID获取好友自定义头像
    QString getFriendCustomFaceFmtById(const QString& friendId);       // 通过好友ID获取好友自定义头像格式
    QString getFriendSignatureById(const QString& friendId);           // 通过好友ID获取好友个性签名
    QString getFriendAddressById(const QString& friendId);             // 通过好友ID获取好友地址
    QString getFriendPhoneNumberById(const QString& friendId);         // 通过好友ID获取好友电话号码
    QString getFriendMailById(const QString& friendId);                // 通过好友ID获取好友邮箱
    QString getFriendRemarkById(const QString& friendId);              // 通过好友ID获取好友备注
    QString getFriendRegisterTimeById(const QString& friendId);        // 通过好友ID获取好友注册时间
    QString getFriendUpdateTimeById(const QString& friendId);          // 通过好友ID获取好友更新时间


    /***************** 插入 ******************/



};

/*

CREATE TABLE FriendInfo (
    LocalId INTEGER PRIMARY KEY AUTOINCREMENT,      -- 本地ID，自增主键
    FriendId INTEGER NOT NULL,                        -- 用户ID，不能为空
    FriendName TEXT NOT NULL,                         -- 用户名，不能为空
    NickName TEXT NOT NULL,                         -- 昵称，不能为空
    PassWord TEXT NOT NULL,                         -- 密码，不能为空
    FaceType INTEGER DEFAULT 0,                     -- 脸部类型，默认为0
    CustomFace TEXT DEFAULT NULL,                   -- 自定义头像路径，默认为空
    CustomFaceFmt TEXT DEFAULT NULL,                -- 自定义头像格式，默认为空
    Gender INTEGER DEFAULT 0,                       -- 性别，默认为0
    Birthday INTEGER DEFAULT 0,                     -- 生日，默认为0
    Signature TEXT DEFAULT NULL,                    -- 个性签名，默认为空
    Address TEXT DEFAULT NULL,                      -- 地址，默认为空
    PhoneNumber TEXT DEFAULT NULL,                  -- 电话号码，默认为空
    Mail TEXT DEFAULT NULL,                         -- 邮箱，默认为空
    OwnerId INTEGER DEFAULT 0,                      -- 所有者ID，默认为0
    RegisterTime DATETIME NOT NULL,                 -- 注册时间，不能为空
    Remark TEXT DEFAULT NULL,                       -- 备注，默认为空
    UpdateTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 更新时间，自动使用当前时间戳
    Status INT DEFAULT 0                            -- 状态，默认为0
);

CREATE TABLE GroupInfo (
    LocalId INTEGER PRIMARY KEY AUTOINCREMENT,      -- 本地ID，自增主键
    GroupId INTEGER NOT NULL,                       -- 群组ID，不能为空
    GroupName TEXT NOT NULL,                        -- 群组名，不能为空
    OwnerId INTEGER NOT NULL,                       -- 群组所有者ID，不能为空
    OwnerName TEXT NOT NULL,                        -- 群组所有者名字，不能为空
    Notice TEXT DEFAULT NULL,                       -- 公告，默认为空
    MemberCnt INTEGER DEFAULT 0,                    -- 成员数量，默认为0
    UserIdList TEXT DEFAULT NULL,                   -- 用户ID列表，可以为空
    DisplayNameList TEXT DEFAULT NULL,              -- 显示名字列表，可以为空
    SelfDisplayName TEXT DEFAULT NULL,              -- 自己在群里的显示名字，可以为空
    CreateTime DATETIME NOT NULL,                   -- 创建时间，不能为空
    UpdateTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 更新时间，自动使用当前时间戳
    CustomFace TEXT DEFAULT NULL                    -- 自定义头像，默认为空
);

CREATE TABLE ContactHeadImg (
    LocalId INTEGER PRIMARY KEY AUTOINCREMENT,      -- 本地ID，自增主键
    ContactId INTEGER NOT NULL,                     -- 联系人ID，不能为空
    IsFriend INT NOT NULL,                          -- 是否是好友头像
    SmallHeadImgUrl TEXT,                           -- 小头像URL，可以为空
    BigHeadImgUrl TEXT,                             -- 大头像URL，可以为空
    HeadImgMd5 TEXT,                                -- 头像MD5，可以为空
    HeadImgData BLOB                                -- 头像数据，BLOB类型
);
*/






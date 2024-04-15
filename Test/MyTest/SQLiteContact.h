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


class SQLiteContactTest
{
public:
    SQLiteContactTest();
	~SQLiteContactTest();

public:
    QSqlDatabase createDatabase();                        // �������ݿ�
    bool openDatabase();                                               // �����ݿ�
    void closeDatabase();                                              // �ر����ݿ�



public:
    void setUserId(const QString& userId);                             // �����û�ID
    bool init();                                                       // ��ʼ�����ݿ�

    void queryFriendInfo();                                            // ��ѯ������Ϣ
    std::vector<std::map<QString, QVariant>> queryAllFriendInfo(const QString& userId);  // ��ѯ������Ϣ


private:
    QSqlDatabase db_;
    QString dbName_;


    QString userId_;

};

/*

CREATE TABLE FriendInfo (
    LocalId INTEGER PRIMARY KEY AUTOINCREMENT,      -- ����ID����������
    FriendId INTEGER NOT NULL,                        -- �û�ID������Ϊ��
    FriendName TEXT NOT NULL,                         -- �û���������Ϊ��
    NickName TEXT NOT NULL,                         -- �ǳƣ�����Ϊ��
    PassWord TEXT NOT NULL,                         -- ���룬����Ϊ��
    FaceType INTEGER DEFAULT 0,                     -- �������ͣ�Ĭ��Ϊ0
    CustomFace TEXT DEFAULT NULL,                   -- �Զ���ͷ��·����Ĭ��Ϊ��
    CustomFaceFmt TEXT DEFAULT NULL,                -- �Զ���ͷ���ʽ��Ĭ��Ϊ��
    Gender INTEGER DEFAULT 0,                       -- �Ա�Ĭ��Ϊ0
    Birthday INTEGER DEFAULT 0,                     -- ���գ�Ĭ��Ϊ0
    Signature TEXT DEFAULT NULL,                    -- ����ǩ����Ĭ��Ϊ��
    Address TEXT DEFAULT NULL,                      -- ��ַ��Ĭ��Ϊ��
    PhoneNumber TEXT DEFAULT NULL,                  -- �绰���룬Ĭ��Ϊ��
    Mail TEXT DEFAULT NULL,                         -- ���䣬Ĭ��Ϊ��
    OwnerId INTEGER DEFAULT 0,                      -- ������ID��Ĭ��Ϊ0
    RegisterTime DATETIME NOT NULL,                 -- ע��ʱ�䣬����Ϊ��
    Remark TEXT DEFAULT NULL,                       -- ��ע��Ĭ��Ϊ��
    UpdateTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- ����ʱ�䣬�Զ�ʹ�õ�ǰʱ���
    Status INT DEFAULT 0                            -- ״̬��Ĭ��Ϊ0
);

CREATE TABLE GroupInfo (
    LocalId INTEGER PRIMARY KEY AUTOINCREMENT,      -- ����ID����������
    GroupId INTEGER NOT NULL,                       -- Ⱥ��ID������Ϊ��
    GroupName TEXT NOT NULL,                        -- Ⱥ����������Ϊ��
    OwnerId INTEGER NOT NULL,                       -- Ⱥ��������ID������Ϊ��
    OwnerName TEXT NOT NULL,                        -- Ⱥ�����������֣�����Ϊ��
    Notice TEXT DEFAULT NULL,                       -- ���棬Ĭ��Ϊ��
    MemberCnt INTEGER DEFAULT 0,                    -- ��Ա������Ĭ��Ϊ0
    UserIdList TEXT DEFAULT NULL,                   -- �û�ID�б�����Ϊ��
    DisplayNameList TEXT DEFAULT NULL,              -- ��ʾ�����б�����Ϊ��
    SelfDisplayName TEXT DEFAULT NULL,              -- �Լ���Ⱥ�����ʾ���֣�����Ϊ��
    CreateTime DATETIME NOT NULL,                   -- ����ʱ�䣬����Ϊ��
    UpdateTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- ����ʱ�䣬�Զ�ʹ�õ�ǰʱ���
    CustomFace TEXT DEFAULT NULL                    -- �Զ���ͷ��Ĭ��Ϊ��
);

CREATE TABLE ContactHeadImg (
    LocalId INTEGER PRIMARY KEY AUTOINCREMENT,      -- ����ID����������
    ContactId INTEGER NOT NULL,                     -- ��ϵ��ID������Ϊ��
    IsFriend INT NOT NULL,                          -- �Ƿ��Ǻ���ͷ��
    SmallHeadImgUrl TEXT,                           -- Сͷ��URL������Ϊ��
    BigHeadImgUrl TEXT,                             -- ��ͷ��URL������Ϊ��
    HeadImgMd5 TEXT,                                -- ͷ��MD5������Ϊ��
    HeadImgData BLOB                                -- ͷ�����ݣ�BLOB����
);
*/






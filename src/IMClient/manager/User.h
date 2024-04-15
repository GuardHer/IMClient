#pragma once

#include <memory>
#include <string>
#include <QString>
#include <set>
#include <QObject>

#include "../Status.h"

class User
{
    
public:

    User() : userid_(QString("")), facetype_(QString("")), gender_(QString(""))
        , birthday_(QString("")), ownerid_(QString("")), status_(Status::Offline) {}

    User(const QString& userid, const QString& username) : userid_(userid), username_(username), status_(Status::Offline) {}

    User(const QString& userid, const QString& username, const QString& customface, const QString& lastMsg):
            userid_(userid), username_(username), customface_(customface), lastMsg_(lastMsg)
        ,facetype_(QString("")), gender_(QString("")), birthday_(QString("")), ownerid_(QString("")), status_(Status::Offline) {}

    ~User() = default;

public:
    void setLastMsg(const QString& lastmsg);
    void setUserId(const QString& userid);
    void setUserName(const QString& username);
    void setPassword(const QString& password);
    void setNickName(const QString& nickname);
    void setFaceType(const QString& facetype);
    void setCustomFace(const QString& customface);
    void setCustomFaceFmt(const QString& customfacefmt);
    void setGender(const QString& gender);
    void setBirthday(const QString& birthday);
    void setSignature(const QString& signature);
    void setAddress(const QString& address);
    void setPhoneNumber(const QString& phonenumber);
    void setMail(const QString& mail);
    void setOwnerId(const QString& ownerid);
    void setStatus(Status status);
    void setStatus(int status);
    void setRegisterTime(const QString& registertime);
    void setStatusString(const QString& statusString);

public:
    QString getLastMsg() const;
    QString getUserId() const;
    QString getUserName() const;
    QString getPassword() const;
    QString getNickName() const;
    QString getFaceType() const;
    QString getCustomFace() const;
    QString getCustomFaceFmt() const;
    QString getGender() const;
    QString getBirthday() const;
    QString getSignature() const;
    QString getAddress() const;
    QString getPhoneNumber() const;
    QString getMail() const;
    QString getOwnerId() const;
    QString getRegisterTime() const;
    Status getStatus() const;
    QString getStatusString() const;
//
//signals:
//    // 用户名发生变化的信号
//    void nameChanged(const QString& userId, const QString& userName);
//
//    // 用户状态发生变化的信号
//    void statusChanged(const QString& userId, Status::Status status);
//
//    // 用户上下线状态发生变化的信号
//    void onlineOfflineChanged(const QString& userId, bool isOnline);
//
//    // 用户状态消息发生变化的信号
//    void statusMessageChanged(const QString& userId, const QString& message);
//
//    // 加载聊天历史记录的信号
//    void loadChatHistory();

private:
    QString lastMsg_;          // 最后一条消息
    QString userid_;           // 用户ID
    QString username_;         // 用户名
    QString password_;         // 密码
    QString nickname_;         // 昵称
    QString facetype_;         // 头像
    QString customface_;       // 自定义头像
    QString customfacefmt_;    // 自定义头像格式
    QString gender_;           // 性别
    QString birthday_;         // 生日
    QString signature_;        // 个性签名
    QString address_;          // 地址
    QString phonenumber_;      // 电话
    QString mail_;             // 邮箱
    QString ownerid_;          // 群账号群主ID
    QString registertime_;     // 注册日期
    Status status_;

    // TODO: 分组, 

    std::set<QString> friendList_; // 好友列表

};
typedef std::shared_ptr<User> UserPtr;


class UserExtendInfo 
{

};
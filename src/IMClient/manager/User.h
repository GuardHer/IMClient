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
//    // �û��������仯���ź�
//    void nameChanged(const QString& userId, const QString& userName);
//
//    // �û�״̬�����仯���ź�
//    void statusChanged(const QString& userId, Status::Status status);
//
//    // �û�������״̬�����仯���ź�
//    void onlineOfflineChanged(const QString& userId, bool isOnline);
//
//    // �û�״̬��Ϣ�����仯���ź�
//    void statusMessageChanged(const QString& userId, const QString& message);
//
//    // ����������ʷ��¼���ź�
//    void loadChatHistory();

private:
    QString lastMsg_;          // ���һ����Ϣ
    QString userid_;           // �û�ID
    QString username_;         // �û���
    QString password_;         // ����
    QString nickname_;         // �ǳ�
    QString facetype_;         // ͷ��
    QString customface_;       // �Զ���ͷ��
    QString customfacefmt_;    // �Զ���ͷ���ʽ
    QString gender_;           // �Ա�
    QString birthday_;         // ����
    QString signature_;        // ����ǩ��
    QString address_;          // ��ַ
    QString phonenumber_;      // �绰
    QString mail_;             // ����
    QString ownerid_;          // Ⱥ�˺�Ⱥ��ID
    QString registertime_;     // ע������
    Status status_;

    // TODO: ����, 

    std::set<QString> friendList_; // �����б�

};
typedef std::shared_ptr<User> UserPtr;


class UserExtendInfo 
{

};
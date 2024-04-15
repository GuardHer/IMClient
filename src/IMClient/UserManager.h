#pragma once

#include "base/noncopyable.h"
#include "manager/User.h"
#include "base/singleton.h"
#include <memory>

class UserManager: public noncopyable
{
public:
    UserManager() = default;
    UserManager(const UserManager&) = delete;            // ��ֹ��������
    UserManager& operator=(const UserManager&) = delete; // ��ֹ��ֵ
    ~UserManager() = default;


public:
    // �����û�ID��ȡ�û���Ϣ
    UserPtr getUserInfo(int32_t userid);

    bool init();
};
typedef Singleton<UserManager> UserManagerSingleton;
#define USERMANAGER Singleton<UserManager>::instance()  // ����꣬����ʹ�õ���ģʽ


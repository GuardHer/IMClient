#pragma once

#include "base/noncopyable.h"
#include "manager/User.h"
#include "base/singleton.h"
#include <memory>

class UserManager: public noncopyable
{
public:
    UserManager() = default;
    UserManager(const UserManager&) = delete;            // 禁止拷贝构造
    UserManager& operator=(const UserManager&) = delete; // 禁止赋值
    ~UserManager() = default;


public:
    // 根据用户ID获取用户信息
    UserPtr getUserInfo(int32_t userid);

    bool init();
};
typedef Singleton<UserManager> UserManagerSingleton;
#define USERMANAGER Singleton<UserManager>::instance()  // 定义宏，方便使用单例模式


#pragma once

#include <string>
#include <assert.h>
#include <mutex>
#include <cstdlib> // atexit
#include "noncopyable.h"

// 
template<typename T>
struct has_no_destroy
{
    template <typename C> static char test(decltype(&C::no_destroy));
    template <typename C> static int32_t test(...);
    const static bool value = sizeof(test<T>(0)) == 1;
};

template<typename T>
class Singleton : noncopyable
{
public:
    Singleton() = delete;
    ~Singleton() = delete;

    static T& instance()
    {
        std::call_once(ponce_, &Singleton::init);
        assert(value_ != nullptr);
        return *value_;
    }

private:
    static void init()
    {
        value_ = new T();
        if (!has_no_destroy<T>::value)
        {
            std::atexit(destroy); // ×¢²áÏú»Ùº¯Êý
        }
    }

    static void destroy()
    {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy; (void)dummy;

        delete value_;
        value_ = nullptr;
    }

private:
    static std::once_flag ponce_;
    static T* value_;
};

template<typename T>
std::once_flag Singleton<T>::ponce_;

template<typename T>
T* Singleton<T>::value_ = nullptr;

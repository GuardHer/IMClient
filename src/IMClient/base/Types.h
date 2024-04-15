#pragma once

//#define FD_SETSIZE 2048

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdint.h>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <memory>
#include <mutex>
#include <inaddr.h>
#include <assert.h>


#define __thread _declspec(thread)

using std::string;
using uniqueLock = std::unique_lock<std::mutex>;

inline void memZero(void* p, size_t n)
{
    memset(p, 0, n);
}

template<typename To, typename From>
inline To implicit_cast(From const& f)
{
    return f;
}

template<typename To, typename From>     // use like this: down_cast<T*>(foo);
inline To down_cast(From* f)                     // so we only accept pointers
{

    if (false)
    {
        implicit_cast<From*, To>(0);
    }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
    assert(f == NULL || dynamic_cast<To>(f) != NULL);  // RTTI: debug mode only!
#endif
    return static_cast<To>(f);
}



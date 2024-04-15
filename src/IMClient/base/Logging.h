#pragma once
#include "LogStream.h"
#include "Timestamp.h"
#include <source_location>

class TimeZone;

class Logger {
public:
    enum LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    // compile time calculation of basename of source file
    class SourceFile {
    public:
        template<int N>
        SourceFile(const char(&arr)[N])
            : data_(arr),
            size_(N - 1)
        {
            const char* slash = strrchr(data_, '/'); // builtin function
            if (slash) {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char* filename)
            : data_(filename)
        {
            const char* slash = strrchr(filename, '/');
            if (slash) {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }

        const char* data_;
        int size_;
    };

    Logger(const std::source_location& loc = std::source_location::current());
    Logger(LogLevel level, const std::source_location& loc = std::source_location::current());
    Logger(LogLevel level, const std::source_location& loc, const char* func);
    Logger(LogLevel level, const std::source_location& loc, bool toAbort);
    ~Logger();

    LogStream& stream() { return impl_.stream_; }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
    static void setTimeZone(const TimeZone& tz);

private:
    const char* extractFileName(const char* filePath);

private:

    class Impl {
    public:
        typedef Logger::LogLevel LogLevel;
        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
        void formatTime();
        void finish();

        Timestamp time_;
        LogStream stream_;
        LogLevel level_;
        int line_;
        SourceFile basename_;

    private:
        std::string GetThreadID();
    };

    Impl impl_;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
    return g_logLevel;
}

#define LOG_TRACE if (Logger::logLevel() <= Logger::TRACE) \
  Logger(Logger::TRACE, std::source_location::current()).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG) \
  Logger(Logger::DEBUG, std::source_location::current()).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO) \
  Logger(Logger::INFO, std::source_location::current()).stream()
#define LOG_WARN Logger(Logger::WARN, std::source_location::current()).stream()
#define LOG_ERROR Logger(Logger::ERR, std::source_location::current()).stream()
#define LOG_FATAL Logger(Logger::FATAL, std::source_location::current()).stream()
#define LOG_SYSERR Logger(Logger::ERR, std::source_location::current(), false).stream()
#define LOG_SYSFATAL Logger(Logger::FATAL, std::source_location::current(), true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::CheckNotNull(std::source_location::current(), "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(const std::source_location& loc, const char* names, T* ptr)
{
    if (ptr == NULL)
    {
        Logger(Logger::FATAL, loc).stream() << names;
    }
    return ptr;
}

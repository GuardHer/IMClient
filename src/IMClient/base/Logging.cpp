#include "Logging.h"
#include "Timestamp.h"
#include "TimeZone.h"

#include <errno.h>
#include <stdio.h>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <thread>

// ANSI颜色转义序列
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_TRACE   "\x1b[36m" // 青色
#define ANSI_COLOR_DEBUG   "\x1b[32m" // 蓝色
#define ANSI_COLOR_INFO    "\x1b[34m" // 绿色
#define ANSI_COLOR_WARN    "\x1b[33m" // 黄色
#define ANSI_COLOR_ERROR   "\x1b[31m" // 红色
#define ANSI_COLOR_FATAL   "\x1b[35m" // 紫色

thread_local char t_errnobuf[512];
thread_local char t_time[64];
thread_local time_t t_lastSecond;


const char* strerror_tl(int savedErrno)
{
	return std::strerror(savedErrno);
}

Logger::LogLevel initLogLevel()
{
	if (::getenv("LOG_TRACE"))
		return Logger::TRACE;
	else if (::getenv("MLOG_DEBUG"))
		return Logger::DEBUG;
	else
		return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
  "[TRACE] ",
  "[DEBUG] ",
  "[INFO]  ",
  "[WARN]  ",
  "[ERROR] ",
  "[FATAL] ",
};

// 根据日志级别获取对应颜色的函数
const char* ColorForLogLevel[Logger::NUM_LOG_LEVELS] = 
{
  ANSI_COLOR_TRACE,
  ANSI_COLOR_DEBUG,
  ANSI_COLOR_INFO,
  ANSI_COLOR_WARN,
  ANSI_COLOR_ERROR,
  ANSI_COLOR_FATAL,
};

// helper class for known string length at compile time
class T
{
public:
	T(const char* str, unsigned len)
		:str_(str),
		len_(len)
	{
		//std::cout << str  << ":" << strlen(str) << std::endl;
		assert(strlen(str) == len_);
	}

	const char* str_;
	const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v)
{
	s.append(v.str_, v.len_);
	return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
	s.append(v.data_, v.size_);
	return s;
}

void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
	//FIXME check n
	(void)n;
}

void defaultFlush()
{
	fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
TimeZone g_logTimeZone;


Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
	: time_(Timestamp::now()),
	stream_(),
	level_(level),
	line_(line),
	basename_(file)
{
	formatTime();
	stream_ << T(ColorForLogLevel[level], 5) << T(LogLevelName[level], 8) << ANSI_COLOR_RESET;
	stream_ << GetThreadID();
	stream_ << "[" << basename_ << ':' << line_ << "] - ";
	if (savedErrno != 0)
	{
		stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
	}
}

void Logger::Impl::formatTime()
{
	int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
	int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
	if (seconds != t_lastSecond)
	{
		t_lastSecond = seconds;
		struct DateTime dt;
		if (g_logTimeZone.valid())
		{
			dt = g_logTimeZone.toLocalTime(seconds);
		}
		else
		{
			dt = TimeZone::toUtcTime(seconds);
		}

		int len = snprintf(t_time, sizeof(t_time), "%4d-%02d-%02d %02d:%02d:%02d",
			dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
		assert(len == 19); (void)len;
	}

	if (g_logTimeZone.valid())
	{
		Fmt us(".%06d ", microseconds);
		assert(us.length() == 8);
		stream_ << T(t_time, 19) << T(us.data(), 8);
	}
	else
	{
		Fmt us(".%06dZ ", microseconds);
		assert(us.length() == 9);
		stream_ << T(t_time, 19) << T(us.data(), 9);
	}
}

void Logger::Impl::finish()
{
	stream_ << '\n';
}

std::string Logger::Impl::GetThreadID() {
	std::ostringstream oss;

	oss << "[Thread:" << std::setw(5) << std::setfill(' ') << std::this_thread::get_id() << "] ";

	return oss.str();
}
const char* Logger::extractFileName(const char* filePath) {
	const char* lastSlash = strrchr(filePath, '/');
	const char* lastBackslash = strrchr(filePath, '\\');

	const char* lastSeparator = (lastSlash != nullptr && lastBackslash != nullptr) ?
		(lastSlash > lastBackslash ? lastSlash : lastBackslash) :
		(lastSlash != nullptr ? lastSlash : lastBackslash);



	if (lastSeparator != nullptr) {
		return lastSeparator + 1;
	}

	return filePath;
}

Logger::Logger(const std::source_location& loc)
	: impl_(INFO, 0, SourceFile(extractFileName(loc.file_name())), loc.line())
{
}

Logger::Logger(LogLevel level, const std::source_location& loc)
	: impl_(level, 0, SourceFile(extractFileName(loc.file_name())), loc.line())
{
}

Logger::Logger(LogLevel level, const std::source_location& loc, const char* func)
	: impl_(level, 0, SourceFile(extractFileName(loc.file_name())), loc.line())
{
	impl_.stream_ << func << ' ';
}

Logger::Logger(LogLevel level, const std::source_location& loc, bool toAbort)
	: impl_(toAbort ? FATAL : ERR, errno, SourceFile(extractFileName(loc.file_name())), loc.line())
{
}


Logger::~Logger()
{
	impl_.finish();
	const LogStream::Buffer& buf(stream().buffer());
	g_output(buf.data(), buf.length());
	if (impl_.level_ == FATAL)
	{
		g_flush();
		abort();
	}
}

void Logger::setLogLevel(Logger::LogLevel level)
{
	g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
	g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
	g_flush = flush;
}

void Logger::setTimeZone(const TimeZone& tz)
{
	g_logTimeZone = tz;
}

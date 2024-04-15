#include "Timestamp.h"

#include <time.h>
#include <chrono>
#include <stdio.h>
#include <inttypes.h>

static_assert(sizeof(Timestamp) == sizeof(int64_t),
    "Timestamp should be same size as int64_t");

string Timestamp::toString() const
{
    char buf[32] = { 0 };
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds) const
{
    using namespace std::chrono;

    auto timePoint = time_point<system_clock>(microseconds(microSecondsSinceEpoch_));
    auto timeInTimeT = system_clock::to_time_t(timePoint);
    auto tmTime = std::gmtime(&timeInTimeT);

    std::ostringstream oss;
    oss << std::put_time(tmTime, "%Y%m%d %H:%M:%S");

    if (showMicroseconds) {
        auto microsecondss = duration_cast<microseconds>(timePoint.time_since_epoch()).count() % kMicroSecondsPerSecond;
        oss << "." << std::setw(6) << std::setfill('0') << microsecondss;
    }

    return oss.str();
}

Timestamp Timestamp::now()
{
    using namespace std::chrono;

    // 获取当前时刻的时间点
    auto now = system_clock::now();

    // 将时间点转换为时间戳
    auto duration = now.time_since_epoch();
    auto secondss = duration_cast<seconds>(duration);
    auto microsecondss = duration_cast<microseconds>(duration - secondss);

    // 计算总微秒数
    int64_t totalMicroseconds = secondss.count() * kMicroSecondsPerSecond + microsecondss.count();

    return Timestamp(totalMicroseconds);
}

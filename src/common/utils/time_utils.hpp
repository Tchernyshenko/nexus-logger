#pragma once

#include <time.h>

namespace nexus::utils::time {

inline timespec GetCurrentTime() {
    timespec ts{};
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts;
}

inline timespec GetMonotonicTime() {
    timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts;
}

inline std::string ToString(const timespec& ts) {
    tm time_info{};
    localtime_r(&ts.tv_sec, &time_info);

    char time_buffer[100];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S",
             &time_info);

    char ms_buffer[10];
    snprintf(ms_buffer, sizeof(ms_buffer), ".%03ld", ts.tv_nsec / 1000000);

    return std::string(time_buffer) + std::string(ms_buffer);
}

// Преобразование timespec в миллисекунды (для интервалов)
inline int64_t ToMilliseconds(const timespec& ts) {
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

// Разница между двумя временными метками
inline timespec TimeDifference(const timespec& start, const timespec& end) {
    timespec diff{};
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_nsec = end.tv_nsec - start.tv_nsec;

    if (diff.tv_nsec < 0) {
        diff.tv_sec--;
        diff.tv_nsec += 1000000000LL;
    }

    return diff;
}

} // namespace nexus::utils::time
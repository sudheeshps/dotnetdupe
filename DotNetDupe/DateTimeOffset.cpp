#include "pch.h"
#include "System/DateTimeOffset.h"
#include "System/TimeProvider.h"
#include "System/String.h"
#include <ctime>
#include <cstdio>

namespace DotNetDupe {
    namespace System {
        DateTimeOffset DateTimeOffset::Now() {
            /// Query current local time from the configured system time provider.
            return TimeProvider::GetSystem()->GetLocalNow();
        }

        DateTimeOffset DateTimeOffset::UtcNow() {
            /// Query current UTC time from the configured system time provider.
            return TimeProvider::GetSystem()->GetUtcNow();
        }

        String DateTimeOffset::ToString() const {
            /// Format using ISO 8601 extended standard pattern.
            return ToString("yyyy-MM-dd HH:mm:ss");
        }

        String DateTimeOffset::ToString(const String& sFormat) const {
            /// Convert ticks to total seconds offset by the Unix epoch (1970-01-01).
            int64_t iSeconds = _ticks / 10000000LL - 62135596800LL;
            if (iSeconds < 0) iSeconds = 0;

            /// Convert seconds to platform calendar breakdown.
            std::time_t tTime = static_cast<std::time_t>(iSeconds);
            std::tm tmTime{};
#if defined(_WIN32)
            localtime_s(&tmTime, &tTime);
#else
            localtime_r(&tTime, &tmTime);
#endif

            /// Format timestamp into fixed-width buffer and return String.
            char szBuf[128] = { 0 };
            std::snprintf(szBuf, sizeof(szBuf), "%04d-%02d-%02d %02d:%02d:%02d",
                tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
                tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
            return String(szBuf);
        }
    }
}

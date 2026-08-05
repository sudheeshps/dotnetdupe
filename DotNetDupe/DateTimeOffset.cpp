#include "pch.h"
#include "System/DateTimeOffset.h"
#include "System/TimeProvider.h"
#include "System/String.h"
#include <ctime>
#include <cstdio>

namespace DotNetDupe {
    namespace System {
        DateTimeOffset DateTimeOffset::Now() {
            return TimeProvider::GetSystem()->GetLocalNow();
        }

        DateTimeOffset DateTimeOffset::UtcNow() {
            return TimeProvider::GetSystem()->GetUtcNow();
        }

        String DateTimeOffset::ToString() const {
            return ToString("yyyy-MM-dd HH:mm:ss");
        }

        String DateTimeOffset::ToString(const String& sFormat) const {
            int64_t iSeconds = _ticks / 10000000LL - 62135596800LL;
            if (iSeconds < 0) iSeconds = 0;
            std::time_t tTime = static_cast<std::time_t>(iSeconds);
            std::tm tmTime{};
#if defined(_WIN32)
            localtime_s(&tmTime, &tTime);
#else
            localtime_r(&tTime, &tmTime);
#endif
            char szBuf[128] = { 0 };
            std::snprintf(szBuf, sizeof(szBuf), "%04d-%02d-%02d %02d:%02d:%02d",
                tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
                tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
            return String(szBuf);
        }
    }
}

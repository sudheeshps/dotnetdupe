#include "pch.h"
#include "System/TimeProvider.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <chrono>
#endif

namespace DotNetDupe {
    namespace System {

        class SystemTimeProvider : public TimeProvider {
        public:
            DateTimeOffset GetUtcNow() const override {
#if defined(_WIN32)
                FILETIME ft;
                GetSystemTimePreciseAsFileTime(&ft);
                int64_t ticks = (((int64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
                // Windows FILETIME starts from 1601-01-01, .NET Ticks start from 0001-01-01.
                // Difference is 504911232000000000 ticks.
                return DateTimeOffset(ticks + 504911232000000000LL);
#else
                auto now = std::chrono::system_clock::now();
                auto duration = now.time_since_epoch();
                auto ticks = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() / 100;
                // Unix epoch is 1970-01-01. .NET Ticks start from 0001-01-01.
                // Difference is 621355968000000000 ticks.
                return DateTimeOffset(ticks + 621355968000000000LL);
#endif
            }

            DateTimeOffset GetLocalNow() const override {
#if defined(_WIN32)
                SYSTEMTIME st;
                GetLocalTime(&st);
                FILETIME ft;
                SystemTimeToFileTime(&st, &ft);
                int64_t ticks = (((int64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
                return DateTimeOffset(ticks + 504911232000000000LL);
#else
                // Simple implementation for local now on POSIX
                return GetUtcNow(); // TODO: Add offset
#endif
            }

            int64_t GetTimestamp() const override {
#if defined(_WIN32)
                LARGE_INTEGER li;
                QueryPerformanceCounter(&li);
                return li.QuadPart;
#else
                auto now = std::chrono::steady_clock::now();
                return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
#endif
            }

            int64_t GetTimestampFrequency() const override {
#if defined(_WIN32)
                LARGE_INTEGER li;
                QueryPerformanceFrequency(&li);
                return li.QuadPart;
#else
                return 1000000000LL; // Nanoseconds
#endif
            }
        };

        TimeSpan TimeProvider::GetElapsedTime(int64_t startingTimestamp) const {
            return GetElapsedTime(startingTimestamp, GetTimestamp());
        }

        TimeSpan TimeProvider::GetElapsedTime(int64_t startingTimestamp, int64_t endingTimestamp) const {
            double ticks = (double)(endingTimestamp - startingTimestamp) * TimeSpan::TicksPerSecond / GetTimestampFrequency();
            return TimeSpan((int64_t)ticks);
        }

        TimeProviderPtr TimeProvider::GetSystem() {
            static auto system = SmartPointer<SystemTimeProvider>::NewShared();
            return system.template DynamicCast<TimeProvider>();
        }
    }
}

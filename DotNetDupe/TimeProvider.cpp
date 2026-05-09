#include "pch.h"
#include "System/TimeProvider.h"
#include <windows.h>

namespace DotNetDupe {
    namespace System {

        class SystemTimeProvider : public TimeProvider {
        public:
            DateTimeOffset GetUtcNow() const override {
                FILETIME ft;
                GetSystemTimePreciseAsFileTime(&ft);
                int64_t ticks = (((int64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
                // Windows FILETIME starts from 1601-01-01, .NET Ticks start from 0001-01-01.
                // Difference is 504911232000000000 ticks.
                return DateTimeOffset(ticks + 504911232000000000LL);
            }

            DateTimeOffset GetLocalNow() const override {
                SYSTEMTIME st;
                GetLocalTime(&st);
                FILETIME ft;
                SystemTimeToFileTime(&st, &ft);
                int64_t ticks = (((int64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
                return DateTimeOffset(ticks + 504911232000000000LL);
            }

            int64_t GetTimestamp() const override {
                LARGE_INTEGER li;
                QueryPerformanceCounter(&li);
                return li.QuadPart;
            }

            int64_t GetTimestampFrequency() const override {
                LARGE_INTEGER li;
                QueryPerformanceFrequency(&li);
                return li.QuadPart;
            }
        };

        TimeSpan TimeProvider::GetElapsedTime(int64_t startingTimestamp) const {
            return GetElapsedTime(startingTimestamp, GetTimestamp());
        }

        TimeSpan TimeProvider::GetElapsedTime(int64_t startingTimestamp, int64_t endingTimestamp) const {
            double ticks = (double)(endingTimestamp - startingTimestamp) * TimeSpan::TicksPerSecond / GetTimestampFrequency();
            return TimeSpan((int64_t)ticks);
        }

        std::shared_ptr<TimeProvider> TimeProvider::GetSystem() {
            static auto system = std::make_shared<SystemTimeProvider>();
            return system;
        }
    }
}

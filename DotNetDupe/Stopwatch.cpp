#include "pch.h"
#include "System/Diagnostics/Stopwatch.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <chrono>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            const long long Stopwatch::Frequency = []() {
#if defined(_WIN32)
                LARGE_INTEGER li;
                QueryPerformanceFrequency(&li);
                return li.QuadPart;
#else
                return 1000000000LL; // Nanoseconds
#endif
            }();

            const bool Stopwatch::IsHighResolution = true;

            Stopwatch::Stopwatch() : Stopwatch(TimeProvider::GetSystem()) {}

            Stopwatch::Stopwatch(const std::shared_ptr<TimeProvider>& timeProvider)
                : _timeProvider(timeProvider), _elapsedTicks(0), _startTimeStamp(0), _isRunning(false) {}

            void Stopwatch::Start() {
                if (!_isRunning) {
                    _startTimeStamp = _timeProvider->GetTimestamp();
                    _isRunning = true;
                }
            }

            void Stopwatch::Stop() {
                if (_isRunning) {
                    long long endTimeStamp = _timeProvider->GetTimestamp();
                    _elapsedTicks += (endTimeStamp - _startTimeStamp);
                    _isRunning = false;
                }
            }

            void Stopwatch::Reset() {
                _elapsedTicks = 0;
                _isRunning = false;
                _startTimeStamp = 0;
            }

            void Stopwatch::Restart() {
                _elapsedTicks = 0;
                _startTimeStamp = _timeProvider->GetTimestamp();
                _isRunning = true;
            }

            TimeSpan Stopwatch::Elapsed() const {
                return _timeProvider->GetElapsedTime(0, GetRawElapsedTicks());
            }

            long long Stopwatch::ElapsedMilliseconds() const {
                return (long long)Elapsed().GetTotalMilliseconds();
            }

            long long Stopwatch::ElapsedTicks() const {
                return (long long)Elapsed().GetTicks();
            }

            long long Stopwatch::GetRawElapsedTicks() const {
                long long elapsed = _elapsedTicks;
                if (_isRunning) {
                    elapsed += (_timeProvider->GetTimestamp() - _startTimeStamp);
                }
                return elapsed;
            }

            Stopwatch Stopwatch::StartNew() {
                Stopwatch sw;
                sw.Start();
                return sw;
            }

            Stopwatch Stopwatch::StartNew(const std::shared_ptr<TimeProvider>& timeProvider) {
                Stopwatch sw(timeProvider);
                sw.Start();
                return sw;
            }

            long long Stopwatch::GetTimestamp() {
#if defined(_WIN32)
                LARGE_INTEGER li;
                QueryPerformanceCounter(&li);
                return li.QuadPart;
#else
                auto now = std::chrono::steady_clock::now();
                return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
#endif
            }
        }
    }
}

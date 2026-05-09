#include "pch.h"
#include "System/Diagnostics/Stopwatch.h"
#include <windows.h>

namespace DotNetDupe {
    namespace System {
        namespace Diagnostics {

            const long long Stopwatch::Frequency = []() {
                LARGE_INTEGER li;
                QueryPerformanceFrequency(&li);
                return li.QuadPart;
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
                LARGE_INTEGER li;
                QueryPerformanceCounter(&li);
                return li.QuadPart;
            }
        }
    }
}

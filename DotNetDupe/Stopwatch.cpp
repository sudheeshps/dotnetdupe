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

            struct Stopwatch::Impl {
                TimeProviderPtr timeProvider;
                long long elapsedTicks;
                long long startTimeStamp;
                bool isRunning;
            };

            Stopwatch::Stopwatch() : Stopwatch(TimeProvider::GetSystem()) {}

            Stopwatch::Stopwatch(const TimeProviderPtr& timeProvider)
                : m_pImpl(SmartPointer<Impl>::NewShared()) {
                m_pImpl->timeProvider = timeProvider;
                m_pImpl->elapsedTicks = 0;
                m_pImpl->startTimeStamp = 0;
                m_pImpl->isRunning = false;
            }

            Stopwatch::~Stopwatch() = default;

            void Stopwatch::Start() {
                if (!m_pImpl->isRunning) {
                    m_pImpl->startTimeStamp = m_pImpl->timeProvider->GetTimestamp();
                    m_pImpl->isRunning = true;
                }
            }

            void Stopwatch::Stop() {
                if (m_pImpl->isRunning) {
                    long long endTimeStamp = m_pImpl->timeProvider->GetTimestamp();
                    m_pImpl->elapsedTicks += (endTimeStamp - m_pImpl->startTimeStamp);
                    m_pImpl->isRunning = false;
                }
            }

            void Stopwatch::Reset() {
                m_pImpl->elapsedTicks = 0;
                m_pImpl->isRunning = false;
                m_pImpl->startTimeStamp = 0;
            }

            void Stopwatch::Restart() {
                m_pImpl->elapsedTicks = 0;
                m_pImpl->startTimeStamp = m_pImpl->timeProvider->GetTimestamp();
                m_pImpl->isRunning = true;
            }

            bool Stopwatch::IsRunning() const {
                return m_pImpl->isRunning;
            }

            TimeSpan Stopwatch::Elapsed() const {
                return m_pImpl->timeProvider->GetElapsedTime(0, GetRawElapsedTicks());
            }

            long long Stopwatch::ElapsedMilliseconds() const {
                return (long long)Elapsed().GetTotalMilliseconds();
            }

            long long Stopwatch::ElapsedTicks() const {
                return (long long)Elapsed().GetTicks();
            }

            long long Stopwatch::GetRawElapsedTicks() const {
                long long elapsed = m_pImpl->elapsedTicks;
                if (m_pImpl->isRunning) {
                    elapsed += (m_pImpl->timeProvider->GetTimestamp() - m_pImpl->startTimeStamp);
                }
                return elapsed;
            }

            Stopwatch Stopwatch::StartNew() {
                Stopwatch sw;
                sw.Start();
                return sw;
            }

            Stopwatch Stopwatch::StartNew(const TimeProviderPtr& timeProvider) {
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

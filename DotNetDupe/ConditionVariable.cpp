#include "pch.h"
#include "System/Threading/ConditionVariable.h"
#include "System/Threading/CriticalSection.h"
#include <condition_variable>
#include <mutex>
#include <chrono>

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            
            struct CSLockAdapter {
                CriticalSection& cs;
                void lock() { cs.Enter(); }
                void unlock() { cs.Leave(); }
            };

            struct ConditionVariable::Impl : public Object {
                std::condition_variable_any cv;
            };

            ConditionVariable::ConditionVariable() {
                m_pImpl = SmartPointer<Impl>::NewShared();
            }

            ConditionVariable::ConditionVariable(ConditionVariable&& other) noexcept = default;

            ConditionVariable& ConditionVariable::operator=(ConditionVariable&& other) noexcept = default;

            ConditionVariable::~ConditionVariable() {
            }

            void ConditionVariable::Wait(CriticalSection& cs) {
                CSLockAdapter adapter{cs};
                m_pImpl->cv.wait(adapter);
            }

            bool ConditionVariable::Wait(CriticalSection& cs, int millisecondsTimeout) {
                if (millisecondsTimeout < 0) {
                    Wait(cs);
                    return true;
                }
                CSLockAdapter adapter{cs};
                auto status = m_pImpl->cv.wait_for(adapter, std::chrono::milliseconds(millisecondsTimeout));
                return status == std::cv_status::no_timeout;
            }

            void ConditionVariable::Pulse() {
                m_pImpl->cv.notify_one();
            }

            void ConditionVariable::PulseAll() {
                m_pImpl->cv.notify_all();
            }
        }
    }
}

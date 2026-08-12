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

            ConditionVariable::ConditionVariable() {
                _pData = new std::condition_variable_any();
            }

            ConditionVariable::~ConditionVariable() {
                delete static_cast<std::condition_variable_any*>(_pData);
            }

            void ConditionVariable::Wait(CriticalSection& cs) {
                CSLockAdapter adapter{cs};
                static_cast<std::condition_variable_any*>(_pData)->wait(adapter);
            }

            bool ConditionVariable::Wait(CriticalSection& cs, int millisecondsTimeout) {
                if (millisecondsTimeout < 0) {
                    Wait(cs);
                    return true;
                }
                CSLockAdapter adapter{cs};
                auto status = static_cast<std::condition_variable_any*>(_pData)->wait_for(adapter, std::chrono::milliseconds(millisecondsTimeout));
                return status == std::cv_status::no_timeout;
            }

            void ConditionVariable::Pulse() {
                static_cast<std::condition_variable_any*>(_pData)->notify_one();
            }

            void ConditionVariable::PulseAll() {
                static_cast<std::condition_variable_any*>(_pData)->notify_all();
            }
        }
    }
}

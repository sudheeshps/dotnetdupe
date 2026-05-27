#include "pch.h"
#include "System/Threading/CriticalSection.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            CriticalSection::CriticalSection() {
            }

            CriticalSection::~CriticalSection() {
            }

            void CriticalSection::Enter() {
                _mutex.lock();
            }

            void CriticalSection::Leave() {
                _mutex.unlock();
            }

            bool CriticalSection::TryEnter() {
                return _mutex.try_lock();
            }
        }
    }
}
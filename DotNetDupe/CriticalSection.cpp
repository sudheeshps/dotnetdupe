#include "pch.h"
#include "System/Threading/CriticalSection.h"
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            struct CriticalSection::Impl {
                std::recursive_mutex mtx;
            };

            CriticalSection::CriticalSection() : m_pImpl(new Impl()) {}

            CriticalSection::~CriticalSection() {
                /// Release internal mutex allocation.
                if (m_pImpl) {
                    delete m_pImpl;
                    m_pImpl = nullptr;
                }
            }

            void CriticalSection::Enter() {
                /// Acquire recursive lock.
                if (m_pImpl) {
                    m_pImpl->mtx.lock();
                }
            }

            void CriticalSection::Leave() {
                /// Release recursive lock.
                if (m_pImpl) {
                    m_pImpl->mtx.unlock();
                }
            }

            bool CriticalSection::TryEnter() {
                /// Attempt non-blocking recursive lock acquisition.
                return m_pImpl ? m_pImpl->mtx.try_lock() : false;
            }
        }
    }
}
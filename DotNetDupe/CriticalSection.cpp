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
                if (m_pImpl) {
                    delete m_pImpl;
                    m_pImpl = nullptr;
                }
            }

            void CriticalSection::Enter() {
                if (m_pImpl) {
                    m_pImpl->mtx.lock();
                }
            }

            void CriticalSection::Leave() {
                if (m_pImpl) {
                    m_pImpl->mtx.unlock();
                }
            }

            bool CriticalSection::TryEnter() {
                return m_pImpl ? m_pImpl->mtx.try_lock() : false;
            }
        }
    }
}
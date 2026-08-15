#include "pch.h"
#include "System/Threading/Thread.h"
#include "System/TimeoutException.h"
#include "System/UnknownException.h"
#include "System/SmartPointer.h"
#include <chrono>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            struct Thread::Impl {
                std::unique_ptr<std::thread> internalThread;
                ThreadStart start;
                ParameterizedThreadStart parameterizedStart;
                String name;
                std::atomic<bool> isAlive{false};
                bool completed{false};
                std::mutex joinMutex;
                std::condition_variable joinCv;
            };

            thread_local Thread* Thread::_currentThread = nullptr;
            static thread_local SmartPointer<Thread> s_pCurrentThreadStorage(nullptr);

            Thread::Thread(ThreadStart start)
                : m_pImpl(new Impl()) {
                m_pImpl->start = start;
            }

            Thread::Thread(ParameterizedThreadStart start)
                : m_pImpl(new Impl()) {
                m_pImpl->parameterizedStart = start;
            }

            Thread::Thread()
                : m_pImpl(new Impl()) {
                m_pImpl->isAlive = true;
            }

            static void SafelyJoinOrDetach(const std::unique_ptr<std::thread>& pThread) {
                if (!pThread || !pThread->joinable()) return;
                try {
                    if (pThread->get_id() == std::this_thread::get_id()) {
                        pThread->detach();
                    } else {
                        pThread->join();
                    }
                } catch (...) {
                    (void)0; // Suppress exceptions during thread teardown to ensure noexcept safety
                }
            }

            Thread::~Thread() {
                if (m_pImpl) {
                    SafelyJoinOrDetach(m_pImpl->internalThread);
                    delete m_pImpl;
                    m_pImpl = nullptr;
                }
            }

            void Thread::Start() {
                Start(nullptr);
            }

            void Thread::Start(Object* parameter) {
                if (!m_pImpl || m_pImpl->isAlive) return;
                m_pImpl->isAlive = true;
                m_pImpl->completed = false;
                m_pImpl->internalThread = std::make_unique<std::thread>(&Thread::ThreadMain, this, parameter);
            }

            void Thread::Join() {
                if (!m_pImpl || !m_pImpl->internalThread || !m_pImpl->internalThread->joinable()) return;
                if (m_pImpl->internalThread->get_id() == std::this_thread::get_id()) return;
                m_pImpl->internalThread->join();
            }

            bool Thread::Join(int millisecondsTimeout) {
                if (!m_pImpl) return true;
                if (!m_pImpl->isAlive && m_pImpl->completed) return true;
                if (!m_pImpl->internalThread) return true;

                std::unique_lock<std::mutex> lock(m_pImpl->joinMutex);
                if (m_pImpl->joinCv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return m_pImpl->completed; })) {
                    if (m_pImpl->internalThread->joinable()) {
                        m_pImpl->internalThread->join();
                    }
                    return true;
                }
                
                throw TimeoutException("The thread did not terminate within the allotted time.");
            }

            void Thread::Sleep(int millisecondsTimeout) {
                std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsTimeout));
            }

            bool Thread::IsAlive() const {
                return m_pImpl ? m_pImpl->isAlive.load() : false;
            }

            String Thread::GetName() const {
                return m_pImpl ? m_pImpl->name : String("");
            }

            void Thread::SetName(const String& name) {
                if (m_pImpl) m_pImpl->name = name;
            }

            Thread* Thread::GetCurrentThread() {
                if (_currentThread == nullptr) {
                    s_pCurrentThreadStorage = CreateCurrentThreadWrapper();
                    _currentThread = s_pCurrentThreadStorage.Get();
                }
                return _currentThread;
            }

            int Thread::GetCurrentThreadId() {
#if defined(_WIN32)
                return static_cast<int>(::GetCurrentThreadId());
#else
                return static_cast<int>(::syscall(__NR_gettid));
#endif
            }

            SmartPointer<Thread> Thread::CreateCurrentThreadWrapper() {
                return SmartPointer<Thread>(new Thread(), true);
            }

            void Thread::ThreadMain(Object* parameter) {
                _currentThread = this;
                try {
                    if (m_pImpl->start) m_pImpl->start();
                    else if (m_pImpl->parameterizedStart) m_pImpl->parameterizedStart(parameter);
                } catch (...) {
                    (void)0;
                }
                {
                    std::lock_guard<std::mutex> lock(m_pImpl->joinMutex);
                    m_pImpl->completed = true;
                    m_pImpl->isAlive = false;
                }
                m_pImpl->joinCv.notify_all();
            }
        }
    }
}

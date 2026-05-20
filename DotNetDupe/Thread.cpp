#include "pch.h"
#include "System/Threading/Thread.h"
#include "System/TimeoutException.h"
#include <chrono>

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            thread_local Thread* Thread::_currentThread = nullptr;
            static thread_local std::unique_ptr<Thread> _currentThreadStorage;

            Thread::Thread(ThreadStart start)
                : _start(start), _isAlive(false), _completed(false) {
            }

            Thread::Thread(ParameterizedThreadStart start)
                : _parameterizedStart(start), _isAlive(false), _completed(false) {
            }

            Thread::Thread()
                : _isAlive(true), _completed(false) {
            }

            Thread::~Thread() {
                if (_internalThread && _internalThread->joinable()) {
                    _internalThread->detach();
                }
            }

            void Thread::Start() {
                Start(nullptr);
            }

            void Thread::Start(Object* parameter) {
                if (_isAlive) return;
                _isAlive = true;
                _completed = false;
                _internalThread = std::make_unique<std::thread>(&Thread::ThreadMain, this, parameter);
            }

            void Thread::Join() {
                if (_internalThread && _internalThread->joinable()) {
                    _internalThread->join();
                }
            }

            bool Thread::Join(int millisecondsTimeout) {
                if (!_isAlive && _completed) return true;
                if (!_internalThread) return true;

                std::unique_lock<std::mutex> lock(_joinMutex);
                if (_joinCv.wait_for(lock, std::chrono::milliseconds(millisecondsTimeout), [this]() { return _completed; })) {
                    if (_internalThread->joinable()) {
                        _internalThread->join();
                    }
                    return true;
                }
                
                throw TimeoutException(_T("The thread did not terminate within the allotted time."));
            }

            void Thread::Sleep(int millisecondsTimeout) {
                std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsTimeout));
            }

            bool Thread::IsAlive() const {
                return _isAlive;
            }

            String Thread::GetName() const {
                return _name;
            }

            void Thread::SetName(const String& name) {
                _name = name;
            }

            Thread* Thread::GetCurrentThread() {
                if (_currentThread == nullptr) {
                    _currentThreadStorage = std::unique_ptr<Thread>(new Thread());
                    _currentThread = _currentThreadStorage.get();
                }
                return _currentThread;
            }

            void Thread::ThreadMain(Object* parameter) {
                _currentThread = this;
                try {
                    if (_start) {
                        _start();
                    }
                    else if (_parameterizedStart) {
                        _parameterizedStart(parameter);
                    }
                } catch (...) {
                    // Silently absorb or handle thread-level exceptions if needed
                }

                {
                    std::lock_guard<std::mutex> lock(_joinMutex);
                    _completed = true;
                    _isAlive = false;
                }
                _joinCv.notify_all();
            }
        }
    }
}

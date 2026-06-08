#include "pch.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"
#include "System/Environment.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/Threading/EventWaitHandle.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /**
             * @brief Represents a single task in the thread pool queue.
             */
            struct ThreadPoolTask {
                WaitCallback Callback;
                Object* State;
            };

            /**
             * @brief Internal implementation of the thread pool manager.
             * Manages worker threads and task dispatching using DotNetDupe primitives.
             */
            class ThreadPoolInternal {
            public:
                static ThreadPoolInternal& GetInstance() {
                    static ThreadPoolInternal instance;
                    return instance;
                }

                bool QueueTask(WaitCallback callback, Object* pState) {
                    {
                        Lock<CriticalSection> lock(m_csSync);
                        if (m_bIsShuttingDown) return false;
                        
                        ThreadPoolTask objTask;
                        objTask.Callback = callback;
                        objTask.State = pState;
                        m_qTasks.Add(objTask);
                    }
                    m_evtWorkAvailable.Set();
                    return true;
                }

            private:
                ThreadPoolInternal() 
                    : m_bIsShuttingDown(false), m_evtWorkAvailable(false, false) {
                    
                    int iThreadCount = Environment::GetProcessorCount();
                    if (iThreadCount <= 0) iThreadCount = 4;

                    for (int i = 0; i < iThreadCount; ++i) {
                        SmartPointer<Thread> pWorker = SmartPointer<Thread>::NewShared(ThreadStart([this]() { WorkerLoop(); }));
                        pWorker->Start();
                        m_vWorkerThreads.Add(std::move(pWorker));
                    }
                }

                ~ThreadPoolInternal() {
                    {
                        Lock<CriticalSection> lock(m_csSync);
                        m_bIsShuttingDown = true;
                    }
                    // Wake up all threads so they can exit. 
                    // AutoResetEvent only wakes one per Set(), so we need to set it for each thread.
                    for (int i = 0; i < m_vWorkerThreads.GetCount(); ++i) {
                        m_evtWorkAvailable.Set();
                    }

                    for (int i = 0; i < m_vWorkerThreads.GetCount(); ++i) {
                        SmartPointer<Thread> pWorker = m_vWorkerThreads[i];
                        if (!pWorker.IsNull() && pWorker->IsAlive()) {
                            pWorker->Join();
                        }
                    }
                }

                void WorkerLoop() {
                    while (true) {
                        ThreadPoolTask objTask;
                        bool bHasTask = false;

                        {
                            Lock<CriticalSection> lock(m_csSync);
                            if (m_qTasks.GetCount() > 0) {
                                objTask = m_qTasks[0];
                                m_qTasks.RemoveAt(0);
                                bHasTask = true;
                                
                                // If there's more work, signal another thread
                                if (m_qTasks.GetCount() > 0) {
                                    m_evtWorkAvailable.Set();
                                }
                            } else if (m_bIsShuttingDown) {
                                // Signal the next thread to wake up and exit
                                m_evtWorkAvailable.Set();
                                return;
                            }
                        }

                        if (bHasTask) {
                            if (objTask.Callback) {
                                try {
                                    objTask.Callback(objTask.State);
                                } catch (...) {
                                    // Task exceptions are absorbed to prevent worker thread death
                                }
                            }
                        } else {
                            // Wait for work or shutdown signal
                            m_evtWorkAvailable.WaitOne();
                        }
                    }
                }

                Collections::Generic::List<SmartPointer<Thread>> m_vWorkerThreads;
                Collections::Generic::List<ThreadPoolTask> m_qTasks;
                CriticalSection m_csSync;
                EventWaitHandle m_evtWorkAvailable;
                bool m_bIsShuttingDown;
            };

            bool ThreadPool::QueueUserWorkItem(WaitCallback callback) {
                return QueueUserWorkItem(callback, nullptr);
            }

            bool ThreadPool::QueueUserWorkItem(WaitCallback callback, Object* pState) {
                return ThreadPoolInternal::GetInstance().QueueTask(callback, pState);
            }
        }
    }
}

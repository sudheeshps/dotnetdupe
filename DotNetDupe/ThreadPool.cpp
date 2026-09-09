#include "pch.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Thread.h"
#include "System/Environment.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"
#include "System/Threading/EventWaitHandle.h"
#include "System/Collections/Generic/List.h"
#include "System/UnknownException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {

            /// \struct ThreadPoolTask
            /// \brief Represents a single task in the thread pool queue.
            struct ThreadPoolTask {
                WaitCallback Callback;
                Object* State;
            };

            /// \class ThreadPoolInternal
            /// \brief Internal implementation of the thread pool manager.
            /// Manages worker threads and task dispatching using DotNetDupe primitives.
            class ThreadPoolInternal {
            public:
                static ThreadPoolInternal& GetInstance() {
                    /// Step: Return singleton thread pool instance.
                    static ThreadPoolInternal instance;
                    return instance;
                }

                bool QueueTask(WaitCallback callback, Object* pState) {
                    /// Guard: Check shutdown state and enqueue task under synchronization lock.
                    {
                        Lock<CriticalSection> lock(m_csSync);
                        if (m_bIsShuttingDown) return false;
                        
                        ThreadPoolTask objTask;
                        objTask.Callback = callback;
                        objTask.State = pState;
                        m_qTasks.Add(objTask);
                    }
                    /// Step: Signal waiting worker thread that work is available.
                    m_evtWorkAvailable.Set();
                    return true;
                }

                bool SetMinThreads(int iMinThreads) {
                    /// Guard: Validate minimum thread count.
                    if (iMinThreads <= 0) return false;
                    Lock<CriticalSection> lock(m_csSync);
                    if (m_bIsShuttingDown) return false;
                    /// Step: Spawn worker threads up to minimum threshold.
                    while (m_pvWorkerThreads.GetCount() < iMinThreads) {
                        SmartPointer<Thread> pWorker = SmartPointer<Thread>::NewShared(ThreadStart([this]() { WorkerLoop(); }));
                        pWorker->Start();
                        m_pvWorkerThreads.Add(std::move(pWorker));
                    }
                    return true;
                }

            private:
                ThreadPoolInternal() 
                    : m_bIsShuttingDown(false), m_evtWorkAvailable(false, false) {
                    /// Step: Query processor count and pre-allocate worker threads.
                    int iThreadCount = Environment::GetProcessorCount();
                    if (iThreadCount < 10) iThreadCount = 10;

                    for (int i = 0; i < iThreadCount; ++i) {
                        SmartPointer<Thread> pWorker = SmartPointer<Thread>::NewShared(ThreadStart([this]() { WorkerLoop(); }));
                        pWorker->Start();
                        m_pvWorkerThreads.Add(std::move(pWorker));
                    }
                }

                ~ThreadPoolInternal() {
                    /// Step: Signal shutdown state and wake all worker threads.
                    {
                        Lock<CriticalSection> lock(m_csSync);
                        m_bIsShuttingDown = true;
                    }
                    // Wake up all threads so they can exit. 
                    // AutoResetEvent only wakes one per Set(), so we need to set it for each thread.
                    for (int i = 0; i < m_pvWorkerThreads.GetCount(); ++i) {
                        m_evtWorkAvailable.Set();
                    }

                    for (int i = 0; i < m_pvWorkerThreads.GetCount(); ++i) {
                        SmartPointer<Thread> pWorker = m_pvWorkerThreads[i];
                        if (!pWorker.IsNull()) {
                            pWorker->Join();
                        }
                    }
                }

                void WorkerLoop() {
                    /// Step: Continuously retrieve and process tasks until shutdown.
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
                                } catch (const Exception&) {
                                    // DotNetDupe exception
                                } catch (const std::exception& ex) {
                                    (void)UnknownException(ex.what());
                                } catch (...) {
                                    (void)UnknownException("An unhandled exception occurred during ThreadPool task execution.");
                                }
                            }
                        } else {
                            // Wait for work or shutdown signal
                            m_evtWorkAvailable.WaitOne();
                        }
                    }
                }

                Collections::Generic::List<SmartPointer<Thread>> m_pvWorkerThreads;
                Collections::Generic::List<ThreadPoolTask> m_qTasks;
                CriticalSection m_csSync;
                EventWaitHandle m_evtWorkAvailable;
                bool m_bIsShuttingDown;
            };

            bool ThreadPool::QueueUserWorkItem(WaitCallback callback) {
                /// Forward to QueueUserWorkItem with null state parameter.
                return QueueUserWorkItem(callback, nullptr);
            }

            bool ThreadPool::QueueUserWorkItem(WaitCallback callback, Object* pState) {
                /// Forward task to ThreadPoolInternal singleton instance.
                return ThreadPoolInternal::GetInstance().QueueTask(callback, pState);
            }

            bool ThreadPool::SetMinThreads(int iMinThreads) {
                /// Forward min threads configuration to ThreadPoolInternal singleton.
                return ThreadPoolInternal::GetInstance().SetMinThreads(iMinThreads);
            }
        }
    }
}

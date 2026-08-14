#include "pch.h"
#include "System/Threading/Tasks/Task.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Lock.h"
#include "System/Collections/Generic/List.h"
#include "System/SystemException.h"
#include "System/InvalidOperationException.h"
#include "System/UnknownException.h"
#include "System/TimeoutException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            namespace Tasks {

                static Collections::Generic::List<SmartPointer<Task>> s_pvActiveTasks;
                static CriticalSection s_csActiveTasks;

                Task::Task(Action<> objAction)
                    : m_objAction(objAction), m_eStatus(TaskStatus::Created) {
                    m_pCompletionEvent = SmartPointer<ManualResetEvent>::NewShared(false);
                }

                Task::~Task() {
                }

                void Task::Start() {
                    {
                        Lock<CriticalSection> lock(m_csSync);
                        if (m_eStatus != TaskStatus::Created) {
                            throw InvalidOperationException("Task has already been started or executed.");
                        }
                        m_eStatus = TaskStatus::WaitingToRun;
                    }
                    
                    ThreadPool::QueueUserWorkItem(&Task::ThreadPoolCallback, this);
                }

                void Task::Wait() {
                    m_pCompletionEvent->WaitOne();
                }

                bool Task::Wait(int iMillisecondsTimeout) {
                    try {
                        return m_pCompletionEvent->WaitOne(iMillisecondsTimeout);
                    } catch (const TimeoutException&) {
                        return false;
                    }
                }

                TaskStatus Task::GetStatus() const {
                    Lock<CriticalSection> lock(const_cast<CriticalSection&>(m_csSync));
                    return m_eStatus;
                }

                bool Task::GetIsCompleted() const {
                    Lock<CriticalSection> lock(const_cast<CriticalSection&>(m_csSync));
                    return m_eStatus == TaskStatus::RanToCompletion || 
                           m_eStatus == TaskStatus::Faulted || 
                           m_eStatus == TaskStatus::Canceled;
                }

                bool Task::GetIsFaulted() const {
                    Lock<CriticalSection> lock(const_cast<CriticalSection&>(m_csSync));
                    return m_eStatus == TaskStatus::Faulted;
                }

                bool Task::GetIsCanceled() const {
                    Lock<CriticalSection> lock(const_cast<CriticalSection&>(m_csSync));
                    return m_eStatus == TaskStatus::Canceled;
                }

                SmartPointer<Task> Task::Run(Action<> objAction) {
                    SmartPointer<Task> pTask = SmartPointer<Task>::NewShared(objAction);
                    RetainTask(pTask);
                    pTask->Start();
                    return pTask;
                }

                void Task::Execute() {
                    {
                        Lock<CriticalSection> lock(m_csSync);
                        m_eStatus = TaskStatus::Running;
                    }
                    try {
                        if (m_objAction) m_objAction();
                        Lock<CriticalSection> lock(m_csSync);
                        m_eStatus = TaskStatus::RanToCompletion;
                    } catch (...) {
                        Lock<CriticalSection> lock(m_csSync);
                        m_eStatus = TaskStatus::Faulted;
                    }
                    m_pCompletionEvent->Set();
                    ReleaseTask(this);
                }

                void Task::ThreadPoolCallback(Object* pState) {
                    Task* pTask = static_cast<Task*>(pState);
                    if (pTask == nullptr) return;

                    // Retain shared ownership to prevent UAF when ReleaseTask is called inside Execute()
                    SmartPointer<Task> spSelf(nullptr);
                    {
                        Lock<CriticalSection> lock(s_csActiveTasks);
                        for (int i = 0; i < s_pvActiveTasks.GetCount(); ++i) {
                            if (s_pvActiveTasks[i].Get() == pTask) {
                                spSelf = s_pvActiveTasks[i];
                                break;
                            }
                        }
                    }

                    if (!spSelf.IsNull()) {
                        spSelf->Execute();
                    }
                }

                void Task::RetainTask(SmartPointer<Task> pTask) {
                    Lock<CriticalSection> lock(s_csActiveTasks);
                    s_pvActiveTasks.Add(pTask);
                }

                void Task::ReleaseTask(Task* pTask) {
                    Lock<CriticalSection> lock(s_csActiveTasks);
                    for (int i = 0; i < s_pvActiveTasks.GetCount(); ++i) {
                        if (s_pvActiveTasks[i].Get() == pTask) {
                            s_pvActiveTasks.RemoveAt(i);
                            break;
                        }
                    }
                }

            }
        }
    }
}

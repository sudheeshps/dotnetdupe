#include "pch.h"
#include "System/Threading/Tasks/Task.h"
#include "System/Threading/ThreadPool.h"
#include "System/Threading/Lock.h"
#include "System/Collections/Generic/List.h"
#include "System/SystemException.h"
#include "System/TimeoutException.h"

namespace DotNetDupe {
    namespace System {
        namespace Threading {
            namespace Tasks {

                static Collections::Generic::List<SmartPointer<Task>> s_vActiveTasks;
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
                            throw SystemException("Task has already been started.");
                        }
                        m_eStatus = TaskStatus::WaitingToRun;
                    }
                    
                    ThreadPool::QueueUserWorkItem(WaitCallback(&Task::ThreadPoolCallback), this);
                }

                void Task::Wait() {
                    m_pCompletionEvent->WaitOne();
                }

                bool Task::Wait(int iMillisecondsTimeout) {
                    try {
                        m_pCompletionEvent->WaitOne(iMillisecondsTimeout);
                        return true;
                    } catch (const TimeoutException&) {
                        return false;
                    }
                }

                TaskStatus Task::GetStatus() {
                    Lock<CriticalSection> lock(m_csSync);
                    return m_eStatus;
                }

                bool Task::GetIsCompleted() {
                    Lock<CriticalSection> lock(m_csSync);
                    return m_eStatus == TaskStatus::RanToCompletion || 
                           m_eStatus == TaskStatus::Faulted || 
                           m_eStatus == TaskStatus::Canceled;
                }

                bool Task::GetIsFaulted() {
                    Lock<CriticalSection> lock(m_csSync);
                    return m_eStatus == TaskStatus::Faulted;
                }

                bool Task::GetIsCanceled() {
                    Lock<CriticalSection> lock(m_csSync);
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
                        if (m_objAction) {
                            m_objAction();
                        }
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
                    if (pTask != nullptr) {
                        pTask->Execute();
                    }
                }

                void Task::RetainTask(SmartPointer<Task> pTask) {
                    Lock<CriticalSection> lock(s_csActiveTasks);
                    s_vActiveTasks.Add(pTask);
                }

                void Task::ReleaseTask(Task* pTask) {
                    Lock<CriticalSection> lock(s_csActiveTasks);
                    for (int i = 0; i < s_vActiveTasks.GetCount(); ++i) {
                        if (s_vActiveTasks[i].Get() == pTask) {
                            s_vActiveTasks.RemoveAt(i);
                            break;
                        }
                    }
                }

            }
        }
    }
}

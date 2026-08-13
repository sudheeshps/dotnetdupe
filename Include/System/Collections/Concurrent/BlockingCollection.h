#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/ConditionVariable.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename T>
                class BlockingCollection : public Object {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Threading::ConditionVariable m_cvAdd;
                    Threading::ConditionVariable m_cvTake;
                    Generic::LinkedList<T> m_list;
                    int m_iBoundedCapacity;
                    bool m_bIsAddingCompleted;

                public:
                    BlockingCollection() : m_iBoundedCapacity(-1), m_bIsAddingCompleted(false) {}
                    
                    explicit BlockingCollection(int iBoundedCapacity) : m_iBoundedCapacity(iBoundedCapacity), m_bIsAddingCompleted(false) {
                        if (iBoundedCapacity <= 0) {
                            throw System::ArgumentException("Bounded capacity must be greater than zero.");
                        }
                    }

                    void Add(const T& item) {
                        m_csLock.Enter();
                        
                        if (m_bIsAddingCompleted) {
                            m_csLock.Leave();
                            throw System::InvalidOperationException("The collection has been marked as complete for adding.");
                        }

                        if (m_iBoundedCapacity > 0) {
                            while (!m_bIsAddingCompleted && m_list.GetCount() >= m_iBoundedCapacity) {
                                m_cvAdd.Wait(m_csLock);
                            }

                            if (m_bIsAddingCompleted) {
                                m_csLock.Leave();
                                throw System::InvalidOperationException("The collection has been marked as complete for adding.");
                            }
                        }

                        m_list.AddLast(item);
                        m_cvTake.Pulse();
                        m_csLock.Leave();
                    }

                    bool TryAdd(const T& item, int iTimeoutMilliseconds = 0) {
                        m_csLock.Enter();
                        
                        if (m_bIsAddingCompleted) {
                            m_csLock.Leave();
                            return false;
                        }

                        if (m_iBoundedCapacity > 0 && m_list.GetCount() >= m_iBoundedCapacity) {
                            if (iTimeoutMilliseconds <= 0) {
                                m_csLock.Leave();
                                return false;
                            }

                            if (m_bIsAddingCompleted || m_list.GetCount() < m_iBoundedCapacity) {
                                // Condition already met
                            } else {
                                bool bWaitResult = m_cvAdd.Wait(m_csLock, iTimeoutMilliseconds);
                                if (!bWaitResult || m_bIsAddingCompleted || m_list.GetCount() >= m_iBoundedCapacity) {
                                    m_csLock.Leave();
                                    return false;
                                }
                            }
                        }

                        m_list.AddLast(item);
                        m_cvTake.Pulse();
                        m_csLock.Leave();
                        return true;
                    }

                    T Take() {
                        T item;
                        if (!TryTake(item, -1)) {
                            throw System::InvalidOperationException("The collection is empty and has been marked as complete for adding.");
                        }

                        return item;
                    }

                    bool TryTake(T& item, int iTimeoutMilliseconds = 0) {
                        m_csLock.Enter();

                        if (m_list.GetCount() == 0) {
                            if (m_bIsAddingCompleted) {
                                m_csLock.Leave();
                                return false;
                            }

                            if (iTimeoutMilliseconds == 0) {
                                m_csLock.Leave();
                                return false;
                            }

                            if (iTimeoutMilliseconds < 0) {
                                while (!m_bIsAddingCompleted && m_list.GetCount() == 0) {
                                    m_cvTake.Wait(m_csLock);
                                }
                            } else {
                                if (!m_bIsAddingCompleted && m_list.GetCount() == 0) {
                                    m_cvTake.Wait(m_csLock, iTimeoutMilliseconds);
                                }
                            }

                            if (m_list.GetCount() == 0) {
                                m_csLock.Leave();
                                return false;
                            }
                        }

                        item = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();

                        if (m_iBoundedCapacity > 0) {
                            m_cvAdd.Pulse();
                        }

                        m_csLock.Leave();
                        return true;
                    }

                    void CompleteAdding() {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_bIsAddingCompleted = true;
                        m_cvTake.PulseAll();
                        m_cvAdd.PulseAll();
                    }

                    bool IsAddingCompleted() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_bIsAddingCompleted;
                    }

                    bool IsCompleted() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_bIsAddingCompleted && m_list.GetCount() == 0;
                    }

                    int GetCount() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_list.GetCount();
                    }

                    int GetBoundedCapacity() const {
                        return m_iBoundedCapacity;
                    }

                    Array<T> ToArray() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_list.ToArray();
                    }
                };

            }
        }
    }
}

/// \file BlockingCollection.h
/// \brief Thread-safe blocking collection mirroring .NET System.Collections.Concurrent.BlockingCollection<T>.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Concurrent/ConcurrentEnumerator.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/ConditionVariable.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                /// \class BlockingCollection
                /// \brief Provides blocking and bounding capabilities for thread-safe collections implementing the Producer-Consumer pattern.
                ///
                /// \tparam T The type of elements in the collection.
                /// \note Conforms to ECMA-335 Partition IV and .NET Concurrent Collections architecture.
                ///       Coordinates concurrent producer and consumer threads using ConditionVariable signaling and optional bounded capacity limits.
                template <typename T>
                class BlockingCollection : public virtual Generic::IReadOnlyCollection<T> {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Threading::ConditionVariable m_cvAdd;
                    Threading::ConditionVariable m_cvTake;
                    Generic::LinkedList<T> m_list;
                    int m_iBoundedCapacity;
                    bool m_bIsAddingCompleted;

                public:
                    using typename Generic::IEnumerable<T>::Iterator;
                    using Generic::IEnumerable<T>::begin;
                    using Generic::IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the BlockingCollection class without an upper-bound.
                    BlockingCollection() : m_iBoundedCapacity(-1), m_bIsAddingCompleted(false) {}
                    
                    /// \brief Initializes a new instance of the BlockingCollection class with the specified upper-bound.
                    /// \param iBoundedCapacity The bounded size of the collection.
                    /// \throws ArgumentException iBoundedCapacity is less than or equal to zero.
                    explicit BlockingCollection(int iBoundedCapacity) : m_iBoundedCapacity(iBoundedCapacity), m_bIsAddingCompleted(false) {
                        if (iBoundedCapacity <= 0) {
                            throw System::ArgumentException("Bounded capacity must be greater than zero.");
                        }
                    }

                    /// \brief Adds an item to the BlockingCollection.
                    /// \param item The item to be added to the collection.
                    /// \throws InvalidOperationException The collection has been marked as complete for adding.
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

                    /// \brief Attempts to add the specified item to the BlockingCollection within the specified time period.
                    /// \param item The item to be added to the collection.
                    /// \param iTimeoutMilliseconds The number of milliseconds to wait.
                    /// \return True if the item could be added; otherwise false.
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

                    /// \brief Removes an item from the BlockingCollection.
                    /// \return The item removed from the collection.
                    T Take() {
                        T item;
                        if (!TryTake(item, -1)) {
                            throw System::InvalidOperationException("The collection is empty and has been marked as complete for adding.");
                        }

                        return item;
                    }

                    /// \brief Attempts to remove an item from the BlockingCollection within the specified time period.
                    /// \param item The item removed from the collection.
                    /// \param iTimeoutMilliseconds The number of milliseconds to wait.
                    /// \return True if an item could be removed; otherwise false.
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

                    /// \brief Marks the BlockingCollection instances as not accepting any more additions.
                    void CompleteAdding() {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_bIsAddingCompleted = true;
                        m_cvTake.PulseAll();
                        m_cvAdd.PulseAll();
                    }

                    /// \brief Gets whether this BlockingCollection has been marked as complete for adding.
                    /// \return True if adding is completed; otherwise false.
                    bool IsAddingCompleted() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_bIsAddingCompleted;
                    }

                    /// \brief Gets whether this BlockingCollection has been marked as complete for adding and is empty.
                    /// \return True if completed; otherwise false.
                    bool IsCompleted() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_bIsAddingCompleted && m_list.GetCount() == 0;
                    }

                    /// \brief Gets the number of elements contained in the BlockingCollection.
                    /// \return Number of elements.
                    int GetCount() const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_list.GetCount();
                    }

                    /// \brief Gets the bounded capacity of this BlockingCollection instance.
                    /// \return The bounded capacity.
                    int GetBoundedCapacity() const {
                        return m_iBoundedCapacity;
                    }

                    /// \brief Copies the elements stored in the BlockingCollection to a new array.
                    /// \return A new array containing a snapshot of elements.
                    Array<T> ToArray() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_list.ToArray();
                    }

                    /// \brief Copies the elements of the collection to an Array starting at index.
                    /// \param array Destination array.
                    /// \param index Starting index.
                    void CopyTo(Array<T>& array, int index) const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_list.CopyTo(array, index);
                    }

                    /// \brief Returns a snapshot enumerator that iterates through the collection.
                    /// \return A SmartPointer to an IEnumerator<T> for the BlockingCollection.
                    SmartPointer<Generic::IEnumerator<T>> GetEnumerator() const override {
                        return SmartPointer<Generic::IEnumerator<T>>(
                            new ConcurrentEnumerator<T>(ToArray()), true);
                    }
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

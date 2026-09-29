/// \file ConcurrentQueue.h
/// \brief Thread-safe FIFO queue mirroring .NET System.Collections.Concurrent.ConcurrentQueue<T>.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Collections/Concurrent/IProducerConsumerCollection.h"
#include "System/Collections/Concurrent/ConcurrentEnumerator.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                /// \class ConcurrentQueue
                /// \brief Represents a thread-safe first-in, first-out (FIFO) collection.
                /// \tparam T The type of the elements contained in the queue.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                /// Provides thread-safe concurrent enqueuing, dequeuing, and peeking
                /// with RAII critical section synchronization.
                template <typename T>
                class ConcurrentQueue : public virtual IProducerConsumerCollection<T> {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::LinkedList<T> m_list;

                public:
                    using typename Generic::IEnumerable<T>::Iterator;
                    using Generic::IEnumerable<T>::begin;
                    using Generic::IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the ConcurrentQueue class.
                    ConcurrentQueue() = default;

                    /// \brief Adds an object to the end of the ConcurrentQueue.
                    /// \param item The object to add to the queue.
                    void Enqueue(const T& item) {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Append item to the end of the list.
                        m_list.AddLast(item);
                    }

                    /// \brief Attempts to remove and return the object at the beginning of the queue.
                    /// \param result When this method returns, contains the object removed from the queue.
                    /// \return True if an element was removed and returned; otherwise false.
                    bool TryDequeue(T& result) {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        /// Step: Retrieve value and remove head node.
                        result = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();
                        return true;
                    }

                    /// \brief Attempts to return an object from the beginning of the queue without removing it.
                    /// \param result When this method returns, contains the object at the beginning of the queue.
                    /// \return True if an element was read successfully; otherwise false.
                    bool TryPeek(T& result) const {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        /// Return: Peek at the front element.
                        result = m_list.GetFirst()->Value;
                        return true;
                    }

                    /// \brief Removes all objects from the ConcurrentQueue.
                    void Clear() {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Clear internal list.
                        m_list.Clear();
                    }

                    /// \brief Gets the number of elements contained in the ConcurrentQueue.
                    /// \return Total number of elements.
                    int GetCount() const override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: Element count.
                        return m_list.GetCount();
                    }

                    /// \brief Gets a value that indicates whether the ConcurrentQueue is empty.
                    /// \return True if the queue is empty; otherwise false.
                    bool IsEmpty() const {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: True if count is zero.
                        return m_list.GetCount() == 0;
                    }

                    /// \brief Copies the elements stored in the ConcurrentQueue to a new array.
                    /// \return A new array containing a snapshot of elements from the queue.
                    Array<T> ToArray() const override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: Array snapshot.
                        return m_list.ToArray();
                    }

                    /// \brief Copies the elements of the collection to an Array starting at index.
                    /// \param array The destination Array.
                    /// \param index The zero-based destination index.
                    void CopyTo(Array<T>& array, int index) const override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Copy snapshot elements.
                        m_list.CopyTo(array, index);
                    }

                    /// \brief Attempts to add an object to the collection.
                    /// \param item The object to add.
                    /// \return True if added successfully.
                    bool TryAdd(const T& item) override {
                        Enqueue(item);
                        return true;
                    }

                    /// \brief Attempts to remove and return an object from the collection.
                    /// \param item Output receiving removed item.
                    /// \return True if removed successfully; otherwise false.
                    bool TryTake(T& item) override {
                        return TryDequeue(item);
                    }

                    /// \brief Returns a snapshot enumerator that iterates through the collection.
                    /// \return A SmartPointer to an IEnumerator<T> for the ConcurrentQueue.
                    SmartPointer<Generic::IEnumerator<T>> GetEnumerator() const override {
                        return SmartPointer<Generic::IEnumerator<T>>(
                            new ConcurrentEnumerator<T>(ToArray()), true);
                    }
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

/// \file ConcurrentStack.h
/// \brief Thread-safe LIFO stack mirroring .NET System.Collections.Concurrent.ConcurrentStack<T>.

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

                /// \class ConcurrentStack
                /// \brief Represents a thread-safe last-in-first-out (LIFO) collection.
                /// \tparam T The type of the elements in the stack.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                /// Thread safety is guaranteed using CriticalSection locks on stack mutations.
                template <typename T>
                class ConcurrentStack : public virtual IProducerConsumerCollection<T> {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::LinkedList<T> m_list;

                public:
                    using typename Generic::IEnumerable<T>::Iterator;
                    using Generic::IEnumerable<T>::begin;
                    using Generic::IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the ConcurrentStack class that is empty.
                    ConcurrentStack() = default;

                    /// \brief Inserts an object at the top of the ConcurrentStack.
                    /// \param item The object to push onto the ConcurrentStack.
                    void Push(const T& item) {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Push to top of list.
                        m_list.AddFirst(item);
                    }

                    /// \brief Attempts to pop and return the object at the top of the ConcurrentStack.
                    /// \param result Output parameter receiving the removed object.
                    /// \return True if an element was removed successfully; otherwise false.
                    bool TryPop(T& result) {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        /// Step: Retrieve top item and pop.
                        result = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();
                        return true;
                    }

                    /// \brief Attempts to return an object from the top of the ConcurrentStack without removing it.
                    /// \param result Output parameter receiving the top object.
                    /// \return True if an object was returned successfully; otherwise false.
                    bool TryPeek(T& result) const {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        /// Return: Peek at top value.
                        result = m_list.GetFirst()->Value;
                        return true;
                    }

                    /// \brief Removes all objects from the ConcurrentStack.
                    void Clear() {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Clear list.
                        m_list.Clear();
                    }

                    /// \brief Gets the number of elements contained in the ConcurrentStack.
                    /// \return Total element count.
                    int GetCount() const override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: Element count.
                        return m_list.GetCount();
                    }

                    /// \brief Gets a value indicating whether the ConcurrentStack is empty.
                    /// \return True if empty; otherwise false.
                    bool IsEmpty() const {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: True if count is zero.
                        return m_list.GetCount() == 0;
                    }

                    /// \brief Copies the elements of the ConcurrentStack to a new array.
                    /// \return A new array containing a snapshot of elements from the stack.
                    Array<T> ToArray() const override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: Array snapshot in LIFO order.
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
                    /// \param item The object to push onto the stack.
                    /// \return True if added successfully.
                    bool TryAdd(const T& item) override {
                        Push(item);
                        return true;
                    }

                    /// \brief Attempts to remove and return an object from the collection.
                    /// \param item Output receiving popped item.
                    /// \return True if popped successfully; otherwise false.
                    bool TryTake(T& item) override {
                        return TryPop(item);
                    }

                    /// \brief Returns a snapshot enumerator that iterates through the collection.
                    /// \return A SmartPointer to an IEnumerator<T> for the ConcurrentStack.
                    SmartPointer<Generic::IEnumerator<T>> GetEnumerator() const override {
                        return SmartPointer<Generic::IEnumerator<T>>(
                            new ConcurrentEnumerator<T>(ToArray()), true);
                    }
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

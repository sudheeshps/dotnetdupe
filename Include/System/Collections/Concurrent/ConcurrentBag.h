/// \file ConcurrentBag.h
/// \brief Thread-safe unordered bag collection mirroring .NET System.Collections.Concurrent.ConcurrentBag<T>.

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

                /// \class ConcurrentBag
                /// \brief Represents a thread-safe, unordered collection of objects.
                /// \tparam T The type of the elements to be stored in the collection.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                /// Bags are useful for storing objects when ordering doesn't matter and operations are thread-safe.
                template <typename T>
                class ConcurrentBag : public virtual IProducerConsumerCollection<T> {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::LinkedList<T> m_list;

                public:
                    using typename Generic::IEnumerable<T>::Iterator;
                    using Generic::IEnumerable<T>::begin;
                    using Generic::IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the ConcurrentBag class that is empty.
                    ConcurrentBag() = default;

                    /// \brief Destructor.
                    ~ConcurrentBag() override = default;

                    /// \brief Adds an object to the ConcurrentBag.
                    /// \param item The object to be added to the ConcurrentBag.
                    void Add(const T& item) {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Add to internal list.
                        m_list.AddFirst(item);
                    }

                    /// \brief Attempts to remove and return an object from the ConcurrentBag.
                    /// \param result Output parameter receiving the removed object.
                    /// \return True if an object was removed successfully; otherwise false.
                    bool TryTake(T& result) override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        /// Step: Take first item.
                        result = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();
                        return true;
                    }

                    /// \brief Attempts to return an object from the ConcurrentBag without removing it.
                    /// \param result Output parameter receiving the peeked object.
                    /// \return True if an object was returned successfully; otherwise false.
                    bool TryPeek(T& result) const {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        /// Return: Peek at first item.
                        result = m_list.GetFirst()->Value;
                        return true;
                    }

                    /// \brief Removes all objects from the ConcurrentBag.
                    void Clear() {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Step: Clear list.
                        m_list.Clear();
                    }

                    /// \brief Gets the number of elements contained in the ConcurrentBag.
                    /// \return Total number of elements.
                    int GetCount() const override {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: Count of elements.
                        return m_list.GetCount();
                    }

                    /// \brief Gets a value indicating whether the ConcurrentBag is empty.
                    /// \return True if empty; otherwise false.
                    bool IsEmpty() const {
                        /// Guard: Acquire critical section lock.
                        Threading::CriticalSectionLock lock(m_csLock);
                        /// Return: True if count is zero.
                        return m_list.GetCount() == 0;
                    }

                    /// \brief Copies the elements stored in the ConcurrentBag to a new array.
                    /// \return A new array containing a snapshot of elements from the bag.
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
                        Add(item);
                        return true;
                    }

                    /// \brief Returns a snapshot enumerator that iterates through the collection.
                    /// \return A SmartPointer to an IEnumerator<T> for the ConcurrentBag.
                    SmartPointer<Generic::IEnumerator<T>> GetEnumerator() const override {
                        return SmartPointer<Generic::IEnumerator<T>>(
                            new ConcurrentEnumerator<T>(ToArray()), true);
                    }
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class Queue
                /// \brief Represents a first-in, first-out collection of objects.
                ///
                /// \tparam T Specifies the type of elements in the queue.
                /// \note Conforms to ECMA-335 Partition IV Section 5.40 (System.Collections.Generic.Queue<T>).
                ///       Implements FIFO collection semantics with Enqueue, Dequeue, Peek, and non-throwing Try* variants.
                template <typename T>
                class Queue : public Object {
                private:
                    List<T> m_lstItems;

                public:
                    /// \brief Initializes a new instance of the Queue class that is empty.
                    Queue() = default;

                    /// \brief Gets the number of elements contained in the Queue.
                    /// \return The number of elements contained in the Queue.
                    int GetCount() const { return m_lstItems.GetCount(); }

                    /// \brief Adds an object to the end of the Queue.
                    /// \param item The object to add to the Queue.
                    void Enqueue(const T& item) {
                        m_lstItems.Add(item);
                    }

                    /// \brief Removes and returns the object at the beginning of the Queue.
                    /// \return The object that is removed from the beginning of the Queue.
                    /// \throws InvalidOperationException The Queue is empty.
                    T Dequeue() {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Queue is empty.");
                        }
                        T item = m_lstItems[0];
                        m_lstItems.RemoveAt(0);
                        return item;
                    }

                    /// \brief Returns the object at the beginning of the Queue without removing it.
                    /// \return The object at the beginning of the Queue.
                    /// \throws InvalidOperationException The Queue is empty.
                    T Peek() const {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Queue is empty.");
                        }
                        return m_lstItems[0];
                    }

                    /// \brief Removes the object at the beginning of the Queue, and copies it to the result parameter.
                    /// \param result Output parameter receiving the removed object.
                    /// \return true if the object was successfully removed; false if the Queue is empty.
                    bool TryDequeue(T& result) {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        result = m_lstItems[0];
                        m_lstItems.RemoveAt(0);
                        return true;
                    }

                    /// \brief Returns a value that indicates whether there is an object at the beginning of the Queue, and if one is present, copies it to the result parameter.
                    /// \param result Output parameter receiving the peeked object.
                    /// \return true if there is an object at the beginning of the Queue; false if the Queue is empty.
                    bool TryPeek(T& result) const {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        result = m_lstItems[0];
                        return true;
                    }

                    /// \brief Removes all objects from the Queue.
                    void Clear() {
                        m_lstItems.Clear();
                    }

                    /// \brief Determines whether an element is in the Queue.
                    /// \param item The object to locate in the Queue.
                    /// \return true if item is found in the Queue; otherwise, false.
                    bool Contains(const T& item) const {
                        return m_lstItems.Contains(item);
                    }

                    /// \brief Copies the Queue elements to a new array.
                    /// \return A new array containing elements copied from the Queue.
                    Array<T> ToArray() const {
                        return m_lstItems.ToArray();
                    }
                };

            }
        }
    }
}

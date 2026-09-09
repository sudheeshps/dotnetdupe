#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/LinkedList.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                /// \class ConcurrentStack
                /// \brief Represents a thread-safe last-in-first-out (LIFO) collection.
                ///
                /// \tparam T The type of the elements in the stack.
                /// \note Conforms to ECMA-335 Partition IV and .NET Concurrent Collections architecture.
                ///       Thread safety is guaranteed using CriticalSection locks on stack mutations.
                template <typename T>
                class ConcurrentStack : public Object {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::LinkedList<T> m_list;

                public:
                    /// \brief Initializes a new instance of the ConcurrentStack class that is empty.
                    ConcurrentStack() = default;

                    /// \brief Inserts an object at the top of the ConcurrentStack.
                    /// \param item The object to push onto the ConcurrentStack.
                    void Push(const T& item) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_list.AddFirst(item);
                    }

                    /// \brief Attempts to pop and return the object at the top of the ConcurrentStack.
                    /// \param result Output parameter receiving the removed object.
                    /// \return true if an element was removed and returned from the top of the ConcurrentStack successfully; otherwise, false.
                    bool TryPop(T& result) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        result = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();
                        return true;
                    }

                    /// \brief Attempts to return an object from the top of the ConcurrentStack without removing it.
                    /// \param result Output parameter receiving the top object.
                    /// \return true if an object was returned successfully; otherwise, false.
                    bool TryPeek(T& result) const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) {
                            return false;
                        }

                        result = m_list.GetFirst()->Value;
                        return true;
                    }

                    void Clear() {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_list.Clear();
                    }

                    int GetCount() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_list.GetCount();
                    }

                    bool IsEmpty() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_list.GetCount() == 0;
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

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

                /// \class ConcurrentBag
                /// \brief Represents a thread-safe, unordered collection of objects.
                ///
                /// \tparam T The type of the elements to be stored in the collection.
                /// \note Conforms to ECMA-335 Partition IV and .NET Concurrent Collections architecture.
                ///       Bags are useful for storing objects when ordering doesn't matter and operations are thread-safe.
                template <typename T>
                class ConcurrentBag : public Object {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::LinkedList<T> m_list;

                public:
                    /// \brief Initializes a new instance of the ConcurrentBag class that is empty.
                    ConcurrentBag() = default;
                    
                    /// \brief Destructor.
                    ~ConcurrentBag() override = default;

                    /// \brief Adds an object to the ConcurrentBag.
                    /// \param item The object to be added to the ConcurrentBag.
                    void Add(const T& item) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_list.AddFirst(item);
                    }

                    /// \brief Attempts to remove and return an object from the ConcurrentBag.
                    /// \param result Output parameter receiving the removed object.
                    /// \return true if an object was removed successfully; otherwise, false.
                    bool TryTake(T& result) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) return false;
                        
                        result = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();
                        return true;
                    }

                    /// \brief Attempts to return an object from the ConcurrentBag without removing it.
                    /// \param result Output parameter receiving the peeked object.
                    /// \return true if an object was returned successfully; otherwise, false.
                    bool TryPeek(T& result) const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) return false;
                        
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

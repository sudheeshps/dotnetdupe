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

                template <typename T>
                class ConcurrentBag : public Object {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::LinkedList<T> m_list;

                public:
                    ConcurrentBag() = default;
                    
                    ~ConcurrentBag() override = default;

                    void Add(const T& item) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_list.AddFirst(item);
                    }

                    bool TryTake(T& result) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_list.GetCount() == 0) return false;
                        
                        result = m_list.GetFirst()->Value;
                        m_list.RemoveFirst();
                        return true;
                    }

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

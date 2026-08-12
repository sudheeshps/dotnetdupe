#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/List.h"
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename T>
                class ConcurrentBag : public Object {
                private:
                    mutable std::mutex m_mtxLock;
                    Generic::List<T> m_lstItems;

                public:
                    ConcurrentBag() = default;
                    
                    ~ConcurrentBag() override = default;

                    void Add(const T& item) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_lstItems.Add(item);
                    }

                    bool TryTake(T& result) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        if (m_lstItems.GetCount() == 0) return false;
                        
                        result = std::move(m_lstItems[m_lstItems.GetCount() - 1]);
                        m_lstItems.RemoveAt(m_lstItems.GetCount() - 1);
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        if (m_lstItems.GetCount() == 0) return false;
                        
                        result = m_lstItems[m_lstItems.GetCount() - 1];
                        return true;
                    }

                    void Clear() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_lstItems.Clear();
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_lstItems.GetCount();
                    }

                    bool IsEmpty() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_lstItems.GetCount() == 0;
                    }

                    Array<T> ToArray() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_lstItems.ToArray();
                    }
                };

            }
        }
    }
}

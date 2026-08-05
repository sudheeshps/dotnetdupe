#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <vector>
#include <mutex>
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename T>
                class ConcurrentBag : public Object {
                private:
                    mutable std::mutex m_mtxLock;
                    std::vector<T> m_vItems;

                public:
                    ConcurrentBag() = default;

                    void Add(const T& item) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_vItems.push_back(item);
                    }

                    bool TryTake(T& result) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_vItems.empty()) {
                            return false;
                        }

                        result = m_vItems.back();
                        m_vItems.pop_back();
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_vItems.empty()) {
                            return false;
                        }

                        result = m_vItems.back();
                        return true;
                    }

                    void Clear() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_vItems.clear();
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return (int)m_vItems.size();
                    }

                    bool IsEmpty() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_vItems.empty();
                    }

                    Array<T> ToArray() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        Array<T> arrResult((int)m_vItems.size());
                        for (size_t i = 0; i < m_vItems.size(); i++) {
                            arrResult[(int)i] = m_vItems[i];
                        }

                        return arrResult;
                    }
                };

            }
        }
    }
}

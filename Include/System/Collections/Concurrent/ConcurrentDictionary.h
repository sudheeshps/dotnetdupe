#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include <unordered_map>
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename TKey, typename TValue>
                class ConcurrentDictionary : public Object {
                private:
                    mutable std::mutex m_mtxLock;
                    std::unordered_map<TKey, TValue> m_uMap;

                public:
                    ConcurrentDictionary() = default;

                    bool TryAdd(const TKey& key, const TValue& value) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_uMap.find(key) != m_uMap.end()) {
                            return false;
                        }

                        m_uMap[key] = value;
                        return true;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        auto it = m_uMap.find(key);
                        if (it != m_uMap.end()) {
                            value = it->second;
                            return true;
                        }

                        return false;
                    }

                    bool TryRemove(const TKey& key, TValue& value) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        auto it = m_uMap.find(key);
                        if (it != m_uMap.end()) {
                            value = it->second;
                            m_uMap.erase(it);
                            return true;
                        }

                        return false;
                    }

                    bool ContainsKey(const TKey& key) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_uMap.find(key) != m_uMap.end();
                    }

                    void Clear() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_uMap.clear();
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return (int)m_uMap.size();
                    }

                    bool IsEmpty() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_uMap.empty();
                    }

                    TValue GetOrAdd(const TKey& key, const TValue& value) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        auto it = m_uMap.find(key);
                        if (it != m_uMap.end()) {
                            return it->second;
                        }

                        m_uMap[key] = value;
                        return value;
                    }

                    template <typename F>
                    TValue GetOrAdd(const TKey& key, F valueFactory) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        auto it = m_uMap.find(key);
                        if (it != m_uMap.end()) {
                            return it->second;
                        }

                        TValue val = valueFactory(key);
                        m_uMap[key] = val;
                        return val;
                    }

                    TValue AddOrUpdate(const TKey& key, const TValue& addValue, const TValue& updateValue) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        auto it = m_uMap.find(key);
                        if (it != m_uMap.end()) {
                            it->second = updateValue;
                            return updateValue;
                        }

                        m_uMap[key] = addValue;
                        return addValue;
                    }

                    TValue& operator[](const TKey& key) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_uMap[key];
                    }

                    Array<TKey> GetKeys() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        Array<TKey> arrKeys((int)m_uMap.size());
                        int iIndex = 0;
                        for (const auto& kvp : m_uMap) {
                            arrKeys[iIndex++] = kvp.first;
                        }

                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        Array<TValue> arrValues((int)m_uMap.size());
                        int iIndex = 0;
                        for (const auto& kvp : m_uMap) {
                            arrValues[iIndex++] = kvp.second;
                        }

                        return arrValues;
                    }
                };

            }
        }
    }
}

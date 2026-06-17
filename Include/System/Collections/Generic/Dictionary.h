#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include <unordered_map>
#include <stdexcept>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename TKey, typename TValue>
                struct KeyValuePair {
                    TKey Key;
                    TValue Value;
                    KeyValuePair(TKey k, TValue v) : Key(k), Value(v) {}
                };

                template <typename TKey, typename TValue>
                class Dictionary : public Object {
                public:
                    Dictionary() {}

                    int GetCount() const { return (int)m_uMap.size(); }

                    TValue& operator[](const TKey& key) {
                        return m_uMap[key];
                    }

                    const TValue& operator[](const TKey& key) const {
                        return m_uMap.at(key);
                    }

                    void Add(const TKey& key, const TValue& value) {
                        if (ContainsKey(key)) throw System::ArgumentException("An item with the same key has already been added.");
                        m_uMap[key] = value;
                    }

                    void Clear() {
                        m_uMap.clear();
                    }

                    bool ContainsKey(const TKey& key) const {
                        return m_uMap.find(key) != m_uMap.end();
                    }

                    bool Remove(const TKey& key) {
                        return m_uMap.erase(key) > 0;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        auto it = m_uMap.find(key);
                        if (it != m_uMap.end()) {
                            value = it->second;
                            return true;
                        }
                        return false;
                    }

                    Array<TKey> GetKeys() const {
                        Array<TKey> arrKeys(GetCount());
                        int iIdx = 0;
                        for (auto const& [key, val] : m_uMap) arrKeys[iIdx++] = key;
                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        Array<TValue> arrValues(GetCount());
                        int iIdx = 0;
                        for (auto const& [key, val] : m_uMap) arrValues[iIdx++] = val;
                        return arrValues;
                    }

                    // Iterator support for range-based for loops
                    auto begin() { return m_uMap.begin(); }
                    auto end() { return m_uMap.end(); }
                    auto begin() const { return m_uMap.begin(); }
                    auto end() const { return m_uMap.end(); }

                private:
                    std::unordered_map<TKey, TValue> m_uMap;
                };
            }
        }
    }
}

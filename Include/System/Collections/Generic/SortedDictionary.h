#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include <map>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename TKey, typename TValue>
                class SortedDictionary : public Object {
                private:
                    std::map<TKey, TValue> m_mMap;

                public:
                    SortedDictionary() = default;

                    int GetCount() const { return (int)m_mMap.size(); }

                    TValue& operator[](const TKey& key) {
                        return m_mMap[key];
                    }

                    const TValue& operator[](const TKey& key) const {
                        return m_mMap.at(key);
                    }

                    void Add(const TKey& key, const TValue& value) {
                        if (ContainsKey(key)) throw System::ArgumentException("An item with the same key has already been added.");
                        m_mMap[key] = value;
                    }

                    void Clear() {
                        m_mMap.clear();
                    }

                    bool ContainsKey(const TKey& key) const {
                        return m_mMap.find(key) != m_mMap.end();
                    }

                    bool Remove(const TKey& key) {
                        return m_mMap.erase(key) > 0;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        auto it = m_mMap.find(key);
                        if (it != m_mMap.end()) {
                            value = it->second;
                            return true;
                        }
                        return false;
                    }

                    Array<TKey> GetKeys() const {
                        Array<TKey> arrKeys((int)m_mMap.size());
                        int iIndex = 0;
                        for (const auto& kvp : m_mMap) {
                            arrKeys[iIndex++] = kvp.first;
                        }
                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        Array<TValue> arrValues((int)m_mMap.size());
                        int iIndex = 0;
                        for (const auto& kvp : m_mMap) {
                            arrValues[iIndex++] = kvp.second;
                        }
                        return arrValues;
                    }
                };

            }
        }
    }
}

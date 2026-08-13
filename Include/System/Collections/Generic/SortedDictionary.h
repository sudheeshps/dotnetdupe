#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename TKey, typename TValue>
                class SortedDictionary : public Object {
                private:
                    struct KeyValuePair {
                        TKey Key;
                        TValue Value;
                        
                        bool operator==(const KeyValuePair& other) const {
                            return Key == other.Key;
                        }
                        
                        bool operator<(const KeyValuePair& other) const {
                            return Key < other.Key;
                        }

                        bool operator>(const KeyValuePair& other) const {
                            return Key > other.Key;
                        }
                    };
                    
                    List<KeyValuePair> m_lstItems;

                public:
                    SortedDictionary() = default;

                    int GetCount() const { return m_lstItems.GetCount(); }

                    TValue& operator[](const TKey& key) {
                        int index = m_lstItems.BinarySearch(KeyValuePair{ key, TValue() });
                        if (index >= 0) {
                            return m_lstItems[index].Value;
                        }

                        m_lstItems.Insert(~index, KeyValuePair{ key, TValue() });
                        return m_lstItems[~index].Value;
                    }

                    const TValue& operator[](const TKey& key) const {
                        int index = m_lstItems.BinarySearch(KeyValuePair{ key, TValue() });
                        if (index >= 0) {
                            return m_lstItems[index].Value;
                        }
                        throw System::ArgumentException("Key not found.");
                    }

                    void Add(const TKey& key, const TValue& value) {
                        int index = m_lstItems.BinarySearch(KeyValuePair{ key, TValue() });
                        if (index >= 0) throw System::ArgumentException("An item with the same key has already been added.");

                        m_lstItems.Insert(~index, KeyValuePair{ key, value });
                    }

                    void Clear() {
                        m_lstItems.Clear();
                    }

                    bool ContainsKey(const TKey& key) const {
                        return m_lstItems.BinarySearch(KeyValuePair{ key, TValue() }) >= 0;
                    }

                    bool Remove(const TKey& key) {
                        int index = m_lstItems.BinarySearch(KeyValuePair{ key, TValue() });
                        if (index < 0) return false;

                        m_lstItems.RemoveAt(index);
                        return true;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        int index = m_lstItems.BinarySearch(KeyValuePair{ key, TValue() });
                        if (index >= 0) {
                            value = m_lstItems[index].Value;
                            return true;
                        }
                        return false;
                    }

                    Array<TKey> GetKeys() const {
                        int count = m_lstItems.GetCount();
                        Array<TKey> arrKeys(count);
                        for (int i = 0; i < count; ++i) {
                            arrKeys[i] = m_lstItems[i].Key;
                        }
                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        int count = m_lstItems.GetCount();
                        Array<TValue> arrValues(count);
                        for (int i = 0; i < count; ++i) {
                            arrValues[i] = m_lstItems[i].Value;
                        }
                        return arrValues;
                    }
                };

            }
        }
    }
}
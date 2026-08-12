#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
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
                    };
                    
                    KeyValuePair* m_pItems;
                    int m_iCount;
                    int m_iCapacity;

                    void EnsureCapacity(int required) {
                        if (required <= m_iCapacity) return;
                        int newCap = m_iCapacity == 0 ? 4 : m_iCapacity * 2;
                        if (newCap < required) newCap = required;
                        
                        KeyValuePair* newItems = (KeyValuePair*)System::AllocateCollectionBuffer(newCap * sizeof(KeyValuePair));
                        for (int i = 0; i < m_iCount; ++i) {
                            new (&newItems[i]) KeyValuePair(std::move(m_pItems[i]));
                            m_pItems[i].~KeyValuePair();
                        }
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                        }
                        m_pItems = newItems;
                        m_iCapacity = newCap;
                    }

                    int BinarySearch(const TKey& key, bool& found) const {
                        int low = 0;
                        int high = m_iCount - 1;
                        while (low <= high) {
                            int mid = low + (high - low) / 2;
                            if (m_pItems[mid].Key == key) {
                                found = true;
                                return mid;
                            }
                            if (m_pItems[mid].Key < key) {
                                low = mid + 1;
                            } else {
                                high = mid - 1;
                            }
                        }
                        found = false;
                        return low;
                    }

                public:
                    SortedDictionary() : m_pItems(nullptr), m_iCount(0), m_iCapacity(0) {}

                    ~SortedDictionary() override {
                        Clear();
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                            m_pItems = nullptr;
                        }
                    }

                    int GetCount() const { return m_iCount; }

                    TValue& operator[](const TKey& key) {
                        bool found = false;
                        int index = BinarySearch(key, found);
                        if (found) {
                            return m_pItems[index].Value;
                        }

                        EnsureCapacity(m_iCount + 1);
                        for (int i = m_iCount; i > index; --i) {
                            new (&m_pItems[i]) KeyValuePair(std::move(m_pItems[i - 1]));
                            m_pItems[i - 1].~KeyValuePair();
                        }
                        new (&m_pItems[index]) KeyValuePair{ key, TValue() };
                        m_iCount++;
                        return m_pItems[index].Value;
                    }

                    const TValue& operator[](const TKey& key) const {
                        bool found = false;
                        int index = BinarySearch(key, found);
                        if (found) {
                            return m_pItems[index].Value;
                        }
                        throw System::ArgumentException("Key not found.");
                    }

                    void Add(const TKey& key, const TValue& value) {
                        bool found = false;
                        int index = BinarySearch(key, found);
                        if (found) throw System::ArgumentException("An item with the same key has already been added.");

                        EnsureCapacity(m_iCount + 1);
                        for (int i = m_iCount; i > index; --i) {
                            new (&m_pItems[i]) KeyValuePair(std::move(m_pItems[i - 1]));
                            m_pItems[i - 1].~KeyValuePair();
                        }
                        new (&m_pItems[index]) KeyValuePair{ key, value };
                        m_iCount++;
                    }

                    void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pItems[i].~KeyValuePair();
                        }
                        m_iCount = 0;
                    }

                    bool ContainsKey(const TKey& key) const {
                        bool found = false;
                        BinarySearch(key, found);
                        return found;
                    }

                    bool Remove(const TKey& key) {
                        bool found = false;
                        int index = BinarySearch(key, found);
                        if (!found) return false;

                        m_pItems[index].~KeyValuePair();
                        for (int i = index; i < m_iCount - 1; ++i) {
                            new (&m_pItems[i]) KeyValuePair(std::move(m_pItems[i + 1]));
                            m_pItems[i + 1].~KeyValuePair();
                        }
                        m_iCount--;
                        return true;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        bool found = false;
                        int index = BinarySearch(key, found);
                        if (found) {
                            value = m_pItems[index].Value;
                            return true;
                        }
                        return false;
                    }

                    Array<TKey> GetKeys() const {
                        Array<TKey> arrKeys(m_iCount);
                        for (int i = 0; i < m_iCount; ++i) {
                            arrKeys[i] = m_pItems[i].Key;
                        }
                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        Array<TValue> arrValues(m_iCount);
                        for (int i = 0; i < m_iCount; ++i) {
                            arrValues[i] = m_pItems[i].Value;
                        }
                        return arrValues;
                    }
                };

            }
        }
    }
}
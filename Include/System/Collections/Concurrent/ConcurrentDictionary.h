#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include <mutex>
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename TKey, typename TValue>
                class ConcurrentDictionary : public Object {
                private:
                    struct KeyValuePair {
                        TKey Key;
                        TValue Value;
                    };
                    
                    mutable std::mutex m_mtxLock;
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

                    int FindIndex(const TKey& key) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pItems[i].Key == key) return i;
                        }
                        return -1;
                    }

                public:
                    ConcurrentDictionary() : m_pItems(nullptr), m_iCount(0), m_iCapacity(0) {}

                    ~ConcurrentDictionary() override {
                        Clear();
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                            m_pItems = nullptr;
                        }
                    }

                    bool TryAdd(const TKey& key, const TValue& value) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        if (FindIndex(key) != -1) return false;

                        EnsureCapacity(m_iCount + 1);
                        new (&m_pItems[m_iCount]) KeyValuePair{ key, value };
                        m_iCount++;
                        return true;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        int index = FindIndex(key);
                        if (index != -1) {
                            value = m_pItems[index].Value;
                            return true;
                        }
                        return false;
                    }

                    bool TryRemove(const TKey& key, TValue& value) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        int index = FindIndex(key);
                        if (index != -1) {
                            value = std::move(m_pItems[index].Value);
                            m_pItems[index].~KeyValuePair();
                            for (int i = index; i < m_iCount - 1; ++i) {
                                new (&m_pItems[i]) KeyValuePair(std::move(m_pItems[i + 1]));
                                m_pItems[i + 1].~KeyValuePair();
                            }
                            m_iCount--;
                            return true;
                        }
                        return false;
                    }

                    bool ContainsKey(const TKey& key) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return FindIndex(key) != -1;
                    }

                    void Clear() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pItems[i].~KeyValuePair();
                        }
                        m_iCount = 0;
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_iCount;
                    }

                    bool IsEmpty() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_iCount == 0;
                    }

                    TValue GetOrAdd(const TKey& key, const TValue& value) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        int index = FindIndex(key);
                        if (index != -1) {
                            return m_pItems[index].Value;
                        }

                        EnsureCapacity(m_iCount + 1);
                        new (&m_pItems[m_iCount]) KeyValuePair{ key, value };
                        m_iCount++;
                        return value;
                    }

                    template <typename F>
                    TValue GetOrAdd(const TKey& key, F valueFactory) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        int index = FindIndex(key);
                        if (index != -1) {
                            return m_pItems[index].Value;
                        }

                        TValue val = valueFactory(key);
                        EnsureCapacity(m_iCount + 1);
                        new (&m_pItems[m_iCount]) KeyValuePair{ key, val };
                        m_iCount++;
                        return val;
                    }

                    TValue AddOrUpdate(const TKey& key, const TValue& addValue, const TValue& updateValue) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        int index = FindIndex(key);
                        if (index != -1) {
                            m_pItems[index].Value = updateValue;
                            return updateValue;
                        }

                        EnsureCapacity(m_iCount + 1);
                        new (&m_pItems[m_iCount]) KeyValuePair{ key, addValue };
                        m_iCount++;
                        return addValue;
                    }

                    TValue& operator[](const TKey& key) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        int index = FindIndex(key);
                        if (index != -1) {
                            return m_pItems[index].Value;
                        }

                        EnsureCapacity(m_iCount + 1);
                        new (&m_pItems[m_iCount]) KeyValuePair{ key, TValue() };
                        int addedIndex = m_iCount;
                        m_iCount++;
                        return m_pItems[addedIndex].Value;
                    }

                    Array<TKey> GetKeys() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        Array<TKey> arrKeys(m_iCount);
                        for (int i = 0; i < m_iCount; ++i) {
                            arrKeys[i] = m_pItems[i].Key;
                        }
                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
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
#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/HashHelper.h"
#include <new>
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename TKey, typename TValue>
                struct KeyValuePair {
                    TKey Key;
                    TValue Value;
                    KeyValuePair() {}
                    KeyValuePair(TKey k, TValue v) : Key(k), Value(v) {}
                };

                struct HashHelpers {
                    inline static const int s_primes[] = {
                        3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
                        1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
                        17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
                        187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
                        1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
                    };
                    static int GetPrime(int min) {
                        for (int i = 0; i < (int)(sizeof(s_primes) / sizeof(s_primes[0])); ++i) {
                            if (s_primes[i] >= min) return s_primes[i];
                        }
                        return min;
                    }
                };

                template <typename TKey, typename TValue>
                class Dictionary : public Object {
                private:
                    struct Entry {
                        int hashCode;
                        int next;
                        TKey key;
                        TValue value;
                        
                        Entry() : hashCode(-1), next(-1) {}
                        Entry(int h, int n, const TKey& k, const TValue& v) : hashCode(h), next(n), key(k), value(v) {}
                        Entry(const Entry& other) : hashCode(other.hashCode), next(other.next), key(other.key), value(other.value) {}
                        Entry(Entry&& other) noexcept : hashCode(other.hashCode), next(other.next), key(std::move(other.key)), value(std::move(other.value)) {}
                    };

                    int* m_pBuckets;
                    Entry* m_pEntries;
                    int m_iCount;
                    int m_iFreeList;
                    int m_iFreeCount;
                    int m_iCapacity;

                    void Initialize(int capacity) {
                        m_iCapacity = HashHelpers::GetPrime(capacity);
                        m_pBuckets = (int*)AllocateCollectionBuffer(m_iCapacity * sizeof(int));
                        for (int i = 0; i < m_iCapacity; i++) m_pBuckets[i] = -1;
                        m_pEntries = (Entry*)AllocateCollectionBuffer(m_iCapacity * sizeof(Entry));
                        m_iFreeList = -1;
                        m_iFreeCount = 0;
                        m_iCount = 0;
                    }

                    void FreeBuffer() {
                        if (m_pBuckets) FreeCollectionBuffer(m_pBuckets);
                        if (m_pEntries) {
                            for (int i = 0; i < m_iCount; ++i) {
                                if (m_pEntries[i].hashCode >= 0) m_pEntries[i].~Entry();
                            }
                            FreeCollectionBuffer(m_pEntries);
                        }
                        m_pBuckets = nullptr;
                        m_pEntries = nullptr;
                    }

                    void Resize() {
                        Resize(m_iCount > 0 ? m_iCount * 2 : 4);
                    }

                    void Resize(int newSize) {
                        int newCapacity = HashHelpers::GetPrime(newSize);
                        int* pNewBuckets = (int*)AllocateCollectionBuffer(newCapacity * sizeof(int));
                        for (int i = 0; i < newCapacity; i++) pNewBuckets[i] = -1;
                        Entry* pNewEntries = (Entry*)AllocateCollectionBuffer(newCapacity * sizeof(Entry));
                        for (int i = 0; i < m_iCount; i++) {
                            if (m_pEntries[i].hashCode >= 0) {
                                ::new ((void*)&pNewEntries[i]) Entry(std::move(m_pEntries[i]));
                                m_pEntries[i].~Entry();
                            } else {
                                ::new ((void*)&pNewEntries[i]) Entry();
                            }
                        }
                        for (int i = 0; i < m_iCount; i++) {
                            if (pNewEntries[i].hashCode >= 0) {
                                int bucket = pNewEntries[i].hashCode % newCapacity;
                                pNewEntries[i].next = pNewBuckets[bucket];
                                pNewBuckets[bucket] = i;
                            }
                        }
                        if (m_pBuckets) FreeCollectionBuffer(m_pBuckets);
                        if (m_pEntries) FreeCollectionBuffer(m_pEntries);
                        m_pBuckets = pNewBuckets;
                        m_pEntries = pNewEntries;
                        m_iCapacity = newCapacity;
                    }

                    int FindEntry(const TKey& key) const {
                        if (m_pBuckets != nullptr) {
                            int hashCode = HashHelper<TKey>::GetHashCode(key) & 0x7FFFFFFF;
                            for (int i = m_pBuckets[hashCode % m_iCapacity]; i >= 0; i = m_pEntries[i].next) {
                                if (m_pEntries[i].hashCode == hashCode && m_pEntries[i].key == key) return i;
                            }
                        }
                        return -1;
                    }

                    bool Insert(const TKey& key, const TValue& value, bool add) {
                        if (m_pBuckets == nullptr) Initialize(0);
                        int hashCode = HashHelper<TKey>::GetHashCode(key) & 0x7FFFFFFF;
                        int targetBucket = hashCode % m_iCapacity;

                        for (int i = m_pBuckets[targetBucket]; i >= 0; i = m_pEntries[i].next) {
                            if (m_pEntries[i].hashCode == hashCode && m_pEntries[i].key == key) {
                                if (add) throw System::ArgumentException("An item with the same key has already been added.");
                                m_pEntries[i].value = value;
                                return false;
                            }
                        }

                        int index;
                        if (m_iFreeCount > 0) {
                            index = m_iFreeList;
                            m_iFreeList = m_pEntries[index].next;
                            m_iFreeCount--;
                        } else {
                            if (m_iCount == m_iCapacity) {
                                Resize();
                                targetBucket = hashCode % m_iCapacity;
                            }
                            index = m_iCount;
                            m_iCount++;
                        }
                        ::new ((void*)&m_pEntries[index]) Entry(hashCode, m_pBuckets[targetBucket], key, value);
                        m_pBuckets[targetBucket] = index;
                        return true;
                    }

                public:
                    Dictionary() : m_pBuckets(nullptr), m_pEntries(nullptr), m_iCount(0), m_iFreeList(-1), m_iFreeCount(0), m_iCapacity(0) {}
                    
                    ~Dictionary() override {
                        FreeBuffer();
                    }

                    Dictionary(const Dictionary& other) : Dictionary() {
                        if (other.m_iCount > 0) {
                            Initialize(other.m_iCount);
                            for (int i = 0; i < other.m_iCount; i++) {
                                if (other.m_pEntries[i].hashCode >= 0) {
                                    Add(other.m_pEntries[i].key, other.m_pEntries[i].value);
                                }
                            }
                        }
                    }

                    Dictionary& operator=(const Dictionary& other) {
                        if (this != &other) {
                            Clear();
                            if (other.m_iCount > 0) {
                                Initialize(other.m_iCount);
                                for (int i = 0; i < other.m_iCount; i++) {
                                    if (other.m_pEntries[i].hashCode >= 0) {
                                        Add(other.m_pEntries[i].key, other.m_pEntries[i].value);
                                    }
                                }
                            }
                        }
                        return *this;
                    }

                    Dictionary(Dictionary&& other) noexcept : m_pBuckets(other.m_pBuckets), m_pEntries(other.m_pEntries), m_iCount(other.m_iCount), m_iFreeList(other.m_iFreeList), m_iFreeCount(other.m_iFreeCount), m_iCapacity(other.m_iCapacity) {
                        other.m_pBuckets = nullptr;
                        other.m_pEntries = nullptr;
                        other.m_iCount = 0;
                        other.m_iFreeList = -1;
                        other.m_iFreeCount = 0;
                        other.m_iCapacity = 0;
                    }

                    Dictionary& operator=(Dictionary&& other) noexcept {
                        if (this != &other) {
                            FreeBuffer();
                            m_pBuckets = other.m_pBuckets;
                            m_pEntries = other.m_pEntries;
                            m_iCount = other.m_iCount;
                            m_iFreeList = other.m_iFreeList;
                            m_iFreeCount = other.m_iFreeCount;
                            m_iCapacity = other.m_iCapacity;
                            other.m_pBuckets = nullptr;
                            other.m_pEntries = nullptr;
                            other.m_iCount = 0;
                            other.m_iFreeList = -1;
                            other.m_iFreeCount = 0;
                            other.m_iCapacity = 0;
                        }
                        return *this;
                    }

                    int GetCount() const { return m_iCount - m_iFreeCount; }

                    void Add(const TKey& key, const TValue& value) {
                        Insert(key, value, true);
                    }

                    bool Remove(const TKey& key) {
                        if (m_pBuckets != nullptr) {
                            int hashCode = HashHelper<TKey>::GetHashCode(key) & 0x7FFFFFFF;
                            int bucket = hashCode % m_iCapacity;
                            int last = -1;
                            for (int i = m_pBuckets[bucket]; i >= 0; last = i, i = m_pEntries[i].next) {
                                if (m_pEntries[i].hashCode == hashCode && m_pEntries[i].key == key) {
                                    if (last < 0) m_pBuckets[bucket] = m_pEntries[i].next;
                                    else m_pEntries[last].next = m_pEntries[i].next;
                                    m_pEntries[i].~Entry();
                                    m_pEntries[i].hashCode = -1;
                                    m_pEntries[i].next = m_iFreeList;
                                    m_iFreeList = i;
                                    m_iFreeCount++;
                                    return true;
                                }
                            }
                        }
                        return false;
                    }

                    void Clear() {
                        if (m_iCount > 0) {
                            for (int i = 0; i < m_iCapacity; i++) m_pBuckets[i] = -1;
                            for (int i = 0; i < m_iCount; i++) {
                                if (m_pEntries[i].hashCode >= 0) m_pEntries[i].~Entry();
                            }
                            m_iFreeList = -1;
                            m_iCount = 0;
                            m_iFreeCount = 0;
                        }
                    }

                    bool ContainsKey(const TKey& key) const {
                        return FindEntry(key) >= 0;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        int i = FindEntry(key);
                        if (i >= 0) {
                            value = m_pEntries[i].value;
                            return true;
                        }
                        return false;
                    }

                    TValue& operator[](const TKey& key) {
                        int i = FindEntry(key);
                        if (i >= 0) return m_pEntries[i].value;
                        Insert(key, TValue(), false);
                        return m_pEntries[FindEntry(key)].value;
                    }

                    const TValue& operator[](const TKey& key) const {
                        int i = FindEntry(key);
                        if (i >= 0) return m_pEntries[i].value;
                        throw System::ArgumentException("Key not found.");
                    }

                    Array<TKey> GetKeys() const {
                        Array<TKey> arrKeys(GetCount());
                        int index = 0;
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pEntries[i].hashCode >= 0) arrKeys[index++] = m_pEntries[i].key;
                        }
                        return arrKeys;
                    }

                    Array<TValue> GetValues() const {
                        Array<TValue> arrValues(GetCount());
                        int index = 0;
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pEntries[i].hashCode >= 0) arrValues[index++] = m_pEntries[i].value;
                        }
                        return arrValues;
                    }

                    class Iterator {
                    private:
                        const Dictionary* m_pDict;
                        int m_iIndex;
                        void AdvanceToValid() {
                            while (m_pDict && m_iIndex < m_pDict->m_iCount && m_pDict->m_pEntries[m_iIndex].hashCode < 0) {
                                m_iIndex++;
                            }
                        }
                    public:
                        Iterator(const Dictionary* dict, int index) : m_pDict(dict), m_iIndex(index) {
                            AdvanceToValid();
                        }
                        bool operator!=(const Iterator& other) const { return m_iIndex != other.m_iIndex; }
                        Iterator& operator++() {
                            m_iIndex++;
                            AdvanceToValid();
                            return *this;
                        }
                        KeyValuePair<TKey, TValue> operator*() const {
                            return KeyValuePair<TKey, TValue>(m_pDict->m_pEntries[m_iIndex].key, m_pDict->m_pEntries[m_iIndex].value);
                        }
                    };

                    Iterator begin() const { return Iterator(this, 0); }
                    Iterator end() const { return Iterator(this, m_iCount); }
                };

            }
        }
    }
}

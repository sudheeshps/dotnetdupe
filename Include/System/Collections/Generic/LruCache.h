/// \file LruCache.h
/// \brief Thread-safe generic Least Recently Used (LRU) bounded cache collection.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Collections/Generic/List.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class LruCache
                /// \brief Thread-safe bounded Least Recently Used (LRU) cache backed by Dictionary and List.
                ///
                /// Maintains a bounded set of key-value pairs ordered by recency of access.
                /// When capacity is reached, the least recently accessed item is evicted automatically.
                /// All public operations are synchronized via an internal CriticalSection.
                ///
                /// \tparam TKey The type of keys in the cache.
                /// \tparam TValue The type of values in the cache.
                template <typename TKey, typename TValue>
                class LruCache : public Object {
                private:
                    size_t m_nCapacity;
                    Dictionary<TKey, TValue> m_mapEntries;
                    List<TKey> m_lstOrder;
                    mutable Threading::CriticalSection m_csLock;

                    /// \brief Moves an existing key to the most-recently-used position (front of list).
                    void TouchKey(const TKey& key) {
                        /// Remove existing position and re-insert at front.
                        m_lstOrder.Remove(key);
                        m_lstOrder.Insert(0, key);
                    }

                    /// \brief Evicts the least-recently-used entry from the back of the order list.
                    void EvictLeastRecentlyUsed() {
                        /// Guard: Verify order list is non-empty before eviction.
                        int iCount = m_lstOrder.GetCount();
                        if (iCount <= 0) {
                            return;
                        }

                        /// Remove oldest key from dictionary and order tracking list.
                        TKey oldestKey = m_lstOrder[iCount - 1];
                        m_lstOrder.RemoveAt(iCount - 1);
                        m_mapEntries.Remove(oldestKey);
                    }

                public:
                    /// \brief Initializes a new instance of the LruCache class with the specified capacity.
                    /// \param nCapacity Maximum number of entries to retain (clamped to at least 1).
                    explicit LruCache(size_t nCapacity = 10)
                        : m_nCapacity(nCapacity < 1 ? 1 : nCapacity) {}

                    /// \brief Virtual destructor.
                    virtual ~LruCache() = default;

                    LruCache(const LruCache&) = delete;
                    LruCache& operator=(const LruCache&) = delete;

                    /// \brief Attempts to retrieve the value associated with the specified key and marks it as recently used.
                    /// \param key The key of the value to get.
                    /// \param outValue When this method returns, contains the value if found.
                    /// \return True if the cache contains an element with the specified key; otherwise, false.
                    bool TryGet(const TKey& key, TValue& outValue) {
                        /// Synchronize cache lookup and update recency order on hit.
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        if (m_mapEntries.TryGetValue(key, outValue)) {
                            TouchKey(key);
                            return true;
                        }

                        return false;
                    }

                    /// \brief Inserts or updates a key-value pair in the cache and marks it as most recently used.
                    /// \param key The key of the element to add or update.
                    /// \param value The value of the element to add or update.
                    void Put(const TKey& key, const TValue& value) {
                        /// Synchronize insertion and update existing key if present.
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        if (m_mapEntries.ContainsKey(key)) {
                            m_mapEntries[key] = value;
                            TouchKey(key);
                            return;
                        }

                        /// Evict oldest entry when capacity is reached, then insert at front.
                        if (static_cast<size_t>(m_mapEntries.GetCount()) >= m_nCapacity) {
                            EvictLeastRecentlyUsed();
                        }
                        m_mapEntries.Add(key, value);
                        m_lstOrder.Insert(0, key);
                    }

                    /// \brief Removes the element with the specified key from the cache.
                    /// \param key The key of the element to remove.
                    /// \return True if the element is successfully found and removed; otherwise, false.
                    bool Remove(const TKey& key) {
                        /// Synchronize removal from both dictionary and recency list.
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        if (!m_mapEntries.ContainsKey(key)) {
                            return false;
                        }

                        m_mapEntries.Remove(key);
                        m_lstOrder.Remove(key);
                        return true;
                    }

                    /// \brief Removes all keys and values from the cache.
                    void Clear() {
                        /// Synchronize and clear internal storage containers.
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        m_mapEntries.Clear();
                        m_lstOrder.Clear();
                    }

                    /// \brief Gets the number of key-value pairs currently contained in the cache.
                    /// \return Current entry count.
                    size_t GetCount() const {
                        /// Synchronize and return current dictionary count.
                        Threading::Lock<Threading::CriticalSection> lock(m_csLock);
                        return static_cast<size_t>(m_mapEntries.GetCount());
                    }

                    /// \brief Gets the maximum capacity of the cache.
                    /// \return Maximum number of entries before eviction occurs.
                    size_t GetCapacity() const {
                        /// Return configured capacity.
                        return m_nCapacity;
                    }
                };

            }
        }
    }
}

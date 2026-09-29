/// \file ConcurrentDictionary.h
/// \brief Thread-safe dictionary mirroring .NET System.Collections.Concurrent.ConcurrentDictionary<TKey, TValue>.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Collections/Generic/IDictionary.h"
#include "System/Collections/Generic/IReadOnlyDictionary.h"
#include "System/Collections/Generic/KeyValuePair.h"
#include "System/Collections/Concurrent/ConcurrentEnumerator.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                /// \class ConcurrentDictionary
                /// \brief Represents a thread-safe collection of key/value pairs that can be accessed by multiple threads concurrently.
                ///
                /// \tparam TKey The type of the keys in the dictionary.
                /// \tparam TValue The type of the values in the dictionary.
                /// \note Conforms to ECMA-335 Partition IV and .NET Concurrent Collections architecture.
                ///       Synchronizes operations using CriticalSection mutual exclusion to guarantee atomic modifications.
                template <typename TKey, typename TValue>
                class ConcurrentDictionary : public virtual Generic::IDictionary<TKey, TValue>,
                                             public virtual Generic::IReadOnlyDictionary<TKey, TValue> {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::Dictionary<TKey, TValue> m_dict;

                public:
                    using typename Generic::IEnumerable<Generic::KeyValuePair<TKey, TValue>>::Iterator;
                    using Generic::IEnumerable<Generic::KeyValuePair<TKey, TValue>>::begin;
                    using Generic::IEnumerable<Generic::KeyValuePair<TKey, TValue>>::end;

                    /// \brief Initializes a new instance of the ConcurrentDictionary class that is empty.
                    ConcurrentDictionary() = default;

                    /// \brief Destructor. Clears dictionary contents.
                    ~ConcurrentDictionary() override {
                        Clear();
                    }

                    /// \brief Attempts to add the specified key and value to the ConcurrentDictionary.
                    /// \param key The key of the element to add.
                    /// \param value The value of the element to add.
                    /// \return True if the key/value pair was added successfully; false if the key already exists.
                    bool TryAdd(const TKey& key, const TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.ContainsKey(key)) {
                            return false;
                        }
                        m_dict.Add(key, value);
                        return true;
                    }

                    /// \brief Attempts to get the value associated with the specified key from the ConcurrentDictionary.
                    /// \param key The key of the value to get.
                    /// \param value Output parameter receiving the value associated with key, if found.
                    /// \return True if the key was found in the ConcurrentDictionary; otherwise, false.
                    bool TryGetValue(const TKey& key, TValue& value) const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.TryGetValue(key, value);
                    }

                    /// \brief Attempts to remove and return the value with the specified key.
                    /// \param key The key of the element to remove.
                    /// \param value Output parameter receiving the removed value.
                    /// \return True if the object was removed successfully; otherwise, false.
                    bool TryRemove(const TKey& key, TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.TryGetValue(key, value)) {
                            m_dict.Remove(key);
                            return true;
                        }
                        return false;
                    }

                    /// \brief Determines whether the dictionary contains the specified key.
                    /// \param key The key to locate.
                    /// \return True if the dictionary contains an element with the key; otherwise, false.
                    bool ContainsKey(const TKey& key) const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.ContainsKey(key);
                    }

                    /// \brief Removes all keys and values from the ConcurrentDictionary.
                    void Clear() override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_dict.Clear();
                    }

                    /// \brief Gets the number of key/value pairs contained in the ConcurrentDictionary.
                    /// \return Number of key/value pairs.
                    int GetCount() const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetCount();
                    }

                    /// \brief Gets a value indicating whether the collection is read-only.
                    /// \return False.
                    bool IsReadOnly() const override {
                        return false;
                    }

                    /// \brief Gets a value indicating whether the ConcurrentDictionary is empty.
                    /// \return True if empty; otherwise, false.
                    bool IsEmpty() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetCount() == 0;
                    }

                    /// \brief Adds a key/value pair to the dictionary if the key does not already exist.
                    /// \param key The key of the element to add.
                    /// \param value The value to be added.
                    /// \return The value for the key.
                    TValue GetOrAdd(const TKey& key, const TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        TValue existingVal;
                        if (m_dict.TryGetValue(key, existingVal)) {
                            return existingVal;
                        }
                        m_dict.Add(key, value);
                        return value;
                    }

                    /// \brief Adds a key/value pair using a factory generator function if the key does not exist.
                    /// \tparam F The factory function type.
                    /// \param key The key of the element to add.
                    /// \param valueFactory The function used to generate a value for the key.
                    /// \return The value for the key.
                    template <typename F>
                    TValue GetOrAdd(const TKey& key, F valueFactory) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        TValue existingVal;
                        if (m_dict.TryGetValue(key, existingVal)) {
                            return existingVal;
                        }
                        TValue val = valueFactory(key);
                        m_dict.Add(key, val);
                        return val;
                    }

                    /// \brief Adds a key/value pair if the key does not exist, or updates a key/value pair if it does.
                    /// \param key The key to add or update.
                    /// \param addValue The value to be added for an absent key.
                    /// \param updateValue The value to be set for an existing key.
                    /// \return The new value for the key.
                    TValue AddOrUpdate(const TKey& key, const TValue& addValue, const TValue& updateValue) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.ContainsKey(key)) {
                            m_dict[key] = updateValue;
                            return updateValue;
                        }
                        m_dict.Add(key, addValue);
                        return addValue;
                    }

                    /// \brief Gets or sets the value associated with the specified key.
                    /// \param key The key of the value to get or set.
                    /// \return Reference to the value associated with the key.
                    TValue& operator[](const TKey& key) override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict[key];
                    }

                    /// \brief Gets the value associated with the specified key.
                    /// \param key The key of the value to get.
                    /// \return Const reference to the value associated with the key.
                    const TValue& operator[](const TKey& key) const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict[key];
                    }

                    /// \brief Adds an element with the provided key and value to the dictionary.
                    /// \param key The key of the element to add.
                    /// \param value The value of the element to add.
                    void Add(const TKey& key, const TValue& value) override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_dict.Add(key, value);
                    }

                    /// \brief Removes the element with the specified key.
                    /// \param key The key of the element to remove.
                    /// \return True if the element is removed; otherwise, false.
                    bool Remove(const TKey& key) override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.Remove(key);
                    }

                    /// \brief Adds a KeyValuePair to the dictionary.
                    /// \param item The KeyValuePair to add.
                    void Add(const Generic::KeyValuePair<TKey, TValue>& item) override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_dict.Add(item);
                    }

                    /// \brief Determines whether the dictionary contains a specific KeyValuePair.
                    /// \param item The KeyValuePair to locate.
                    /// \return True if item is found; otherwise, false.
                    bool Contains(const Generic::KeyValuePair<TKey, TValue>& item) const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.Contains(item);
                    }

                    /// \brief Removes a specific KeyValuePair from the dictionary.
                    /// \param item The KeyValuePair to remove.
                    /// \return True if item was removed; otherwise, false.
                    bool Remove(const Generic::KeyValuePair<TKey, TValue>& item) override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.Remove(item);
                    }

                    /// \brief Copies key/value pairs to an Array starting at index.
                    /// \param array Destination Array.
                    /// \param arrayIndex Starting index.
                    void CopyTo(Array<Generic::KeyValuePair<TKey, TValue>>& array, int arrayIndex) const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_dict.CopyTo(array, arrayIndex);
                    }

                    /// \brief Copies all key/value pairs to a new Array snapshot.
                    /// \return An Array of KeyValuePair snapshots.
                    Array<Generic::KeyValuePair<TKey, TValue>> ToArray() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        Array<Generic::KeyValuePair<TKey, TValue>> arrResult(m_dict.GetCount());
                        m_dict.CopyTo(arrResult, 0);
                        return arrResult;
                    }

                    /// \brief Gets an array containing the keys of the dictionary.
                    /// \return An Array of keys.
                    Array<TKey> GetKeys() const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetKeys();
                    }

                    /// \brief Gets an array containing the values of the dictionary.
                    /// \return An Array of values.
                    Array<TValue> GetValues() const override {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetValues();
                    }

                    /// \brief Returns a snapshot enumerator that iterates through the dictionary.
                    /// \return A SmartPointer to an IEnumerator of KeyValuePair elements.
                    SmartPointer<Generic::IEnumerator<Generic::KeyValuePair<TKey, TValue>>> GetEnumerator() const override {
                        return SmartPointer<Generic::IEnumerator<Generic::KeyValuePair<TKey, TValue>>>(
                            new ConcurrentEnumerator<Generic::KeyValuePair<TKey, TValue>>(ToArray()), true);
                    }
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe
#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/KeyValuePair.h"
#include "System/Collections/Generic/IDictionary.h"
#include "System/Collections/Generic/IReadOnlyDictionary.h"
#include "System/Collections/Generic/List.h"
#include "System/EqualityHelper.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class SortedDictionary
                /// \brief Represents a collection of key/value pairs that are sorted on the key.
                ///
                /// \tparam TKey The type of the keys in the dictionary.
                /// \tparam TValue The type of the values in the dictionary.
                /// \note Conforms to ECMA-335 Partition IV Section 5.43 (System.Collections.Generic.SortedDictionary<TKey, TValue>).
                ///       Maintains key-sorted element ordering using binary search.
                template <typename TKey, typename TValue>
                class SortedDictionary : public virtual IDictionary<TKey, TValue>,
                                         public virtual IReadOnlyDictionary<TKey, TValue> {
                private:
                    List<KeyValuePair<TKey, TValue>> m_lstItems;

                    int FindKey(const TKey& key) const {
                        int low = 0;
                        int high = m_lstItems.GetCount() - 1;
                        while (low <= high) {
                            int mid = low + (high - low) / 2;
                            if (m_lstItems[mid].Key == key) return mid;
                            if (m_lstItems[mid].Key < key) {
                                low = mid + 1;
                            } else {
                                high = mid - 1;
                            }
                        }
                        return ~low;
                    }

                public:
                    using typename IEnumerable<KeyValuePair<TKey, TValue>>::Iterator;
                    using IEnumerable<KeyValuePair<TKey, TValue>>::begin;
                    using IEnumerable<KeyValuePair<TKey, TValue>>::end;

                    /// \brief Initializes a new instance of the SortedDictionary class that is empty.
                    SortedDictionary() = default;

                    /// \brief Gets the number of key/value pairs contained in the SortedDictionary.
                    /// \return The number of key/value pairs contained in the SortedDictionary.
                    int GetCount() const override { return m_lstItems.GetCount(); }

                    /// \brief Gets a value indicating whether the collection is read-only.
                    bool IsReadOnly() const override { return false; }

                    /// \brief Gets or sets the value associated with the specified key.
                    /// \param key The key of the value to get or set.
                    /// \return The value associated with the specified key.
                    TValue& operator[](const TKey& key) override {
                        int index = FindKey(key);
                        if (index >= 0) {
                            return m_lstItems[index].Value;
                        }

                        m_lstItems.Insert(~index, KeyValuePair<TKey, TValue>(key, TValue()));
                        return m_lstItems[~index].Value;
                    }

                    /// \brief Gets the value associated with the specified key.
                    const TValue& operator[](const TKey& key) const override {
                        int index = FindKey(key);
                        if (index >= 0) {
                            return m_lstItems[index].Value;
                        }
                        throw System::ArgumentException("Key not found.");
                    }

                    /// \brief Adds an element with the provided key and value.
                    void Add(const TKey& key, const TValue& value) override {
                        int index = FindKey(key);
                        if (index >= 0) throw System::ArgumentException("An item with the same key has already been added.");

                        m_lstItems.Insert(~index, KeyValuePair<TKey, TValue>(key, value));
                    }

                    /// \brief Adds a key/value pair.
                    void Add(const KeyValuePair<TKey, TValue>& item) override {
                        Add(item.Key, item.Value);
                    }

                    /// \brief Clears all items.
                    void Clear() override {
                        m_lstItems.Clear();
                    }

                    /// \brief Determines whether the SortedDictionary contains a specific key.
                    bool ContainsKey(const TKey& key) const override {
                        return FindKey(key) >= 0;
                    }

                    /// \brief Determines whether the SortedDictionary contains a specific key/value pair.
                    bool Contains(const KeyValuePair<TKey, TValue>& item) const override {
                        int index = FindKey(item.Key);
                        return (index >= 0 && EqualityHelper<TValue>::Equals(m_lstItems[index].Value, item.Value));
                    }

                    /// \brief Removes the element with the specified key.
                    bool Remove(const TKey& key) override {
                        int index = FindKey(key);
                        if (index < 0) return false;

                        m_lstItems.RemoveAt(index);
                        return true;
                    }

                    /// \brief Removes the specified key/value pair.
                    bool Remove(const KeyValuePair<TKey, TValue>& item) override {
                        int index = FindKey(item.Key);
                        if (index >= 0 && EqualityHelper<TValue>::Equals(m_lstItems[index].Value, item.Value)) {
                            m_lstItems.RemoveAt(index);
                            return true;
                        }
                        return false;
                    }

                    /// \brief Gets the value associated with the specified key.
                    bool TryGetValue(const TKey& key, TValue& value) const override {
                        int index = FindKey(key);
                        if (index >= 0) {
                            value = m_lstItems[index].Value;
                            return true;
                        }
                        return false;
                    }

                    /// \brief Gets an array containing the keys.
                    Array<TKey> GetKeys() const override {
                        int count = m_lstItems.GetCount();
                        Array<TKey> arrKeys(count);
                        for (int i = 0; i < count; ++i) {
                            arrKeys[i] = m_lstItems[i].Key;
                        }
                        return arrKeys;
                    }

                    /// \brief Gets an array containing the values.
                    Array<TValue> GetValues() const override {
                        int count = m_lstItems.GetCount();
                        Array<TValue> arrValues(count);
                        for (int i = 0; i < count; ++i) {
                            arrValues[i] = m_lstItems[i].Value;
                        }
                        return arrValues;
                    }

                    /// \brief Copies key/value pairs to an array.
                    void CopyTo(Array<KeyValuePair<TKey, TValue>>& array, int arrayIndex) const override {
                        m_lstItems.CopyTo(array, arrayIndex);
                    }

                    /// \brief Returns an enumerator that iterates through the collection.
                    IEnumeratorPtr<KeyValuePair<TKey, TValue>> GetEnumerator() const override {
                        return m_lstItems.GetEnumerator();
                    }
                };

            }
        }
    }
}
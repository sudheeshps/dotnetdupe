#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/ISet.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Generic/IEnumerator.h"
#include "System/Collections/Generic/Dictionary.h"
#include <new>
#include <initializer_list>
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class HashSetEnumerator
                /// \brief Enumerator for HashSet elements.
                /// \tparam T The element type.
                template <typename T>
                class HashSetEnumerator : public virtual IEnumerator<T> {
                private:
                    IEnumeratorPtr<KeyValuePair<T, bool>> m_spDictEnum;

                public:
                    /// \brief Constructs an enumerator wrapping the internal dictionary enumerator.
                    /// \param spDictEnum Pointer to the dictionary enumerator.
                    explicit HashSetEnumerator(IEnumeratorPtr<KeyValuePair<T, bool>> spDictEnum)
                        : m_spDictEnum(spDictEnum) {}

                    /// \brief Gets current element in the set.
                    const T& Current() const override { return GetCurrent(); }

                    /// \brief Gets current element in the set.
                    const T& GetCurrent() const override {
                        if (!m_spDictEnum) throw InvalidOperationException("Enumerator is invalid.");
                        return m_spDictEnum->GetCurrent().Key;
                    }

                    /// \brief Advances to next element.
                    bool MoveNext() override {
                        return m_spDictEnum && m_spDictEnum->MoveNext();
                    }

                    /// \brief Resets enumerator.
                    void Reset() override {
                        if (m_spDictEnum) m_spDictEnum->Reset();
                    }
                };

                /// \class HashSet
                /// \brief Represents a set of unique values.
                /// \tparam T The type of elements in the hash set.
                /// \note Conforms to ECMA-335 Partition IV Section 5.39 (System.Collections.Generic.HashSet<T>).
                template <typename T>
                class HashSet : public virtual ISet<T>,
                                public virtual IReadOnlyCollection<T> {
                private:
                    Dictionary<T, bool> m_dict;

                public:
                    using typename IEnumerable<T>::Iterator;
                    using IEnumerable<T>::begin;
                    using IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the HashSet class that is empty.
                    HashSet() {}

                    /// \brief Copy constructor.
                    HashSet(const HashSet& other) : m_dict(other.m_dict) {}

                    /// \brief Move constructor.
                    HashSet(HashSet&& other) noexcept : m_dict(std::move(other.m_dict)) {}

                    /// \brief Copy assignment operator.
                    HashSet& operator=(const HashSet& other) {
                        if (this != &other) {
                            m_dict = other.m_dict;
                        }
                        return *this;
                    }

                    /// \brief Move assignment operator.
                    HashSet& operator=(HashSet&& other) noexcept {
                        if (this != &other) {
                            m_dict = std::move(other.m_dict);
                        }
                        return *this;
                    }

                    /// \brief Destructor.
                    ~HashSet() override {}

                    /// \brief Gets the number of elements in the set.
                    int GetCount() const override { return m_dict.GetCount(); }

                    /// \brief Adds the specified element to the set.
                    bool Add(const T& item) override {
                        if (m_dict.ContainsKey(item)) return false;
                        m_dict.Add(item, true);
                        return true;
                    }

                    /// \brief Removes the specified element from the set.
                    bool Remove(const T& item) override {
                        return m_dict.Remove(item);
                    }

                    /// \brief Determines whether the set contains the specified element.
                    bool Contains(const T& item) const override {
                        return m_dict.ContainsKey(item);
                    }

                    /// \brief Removes all elements from the set.
                    void Clear() override {
                        m_dict.Clear();
                    }

                    /// \brief Copies elements to an array.
                    void CopyTo(Array<T>& array, int arrayIndex) const override {
                        Array<T> keys = m_dict.GetKeys();
                        if (arrayIndex < 0 || arrayIndex + keys.GetLength() > array.GetLength()) {
                            throw ArgumentException("Target array is too small or index is invalid.");
                        }
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            array[arrayIndex + i] = keys[i];
                        }
                    }

                    /// \brief Copies the elements of a HashSet object to an Array.
                    Array<T> ToArray() const {
                        return m_dict.GetKeys();
                    }

                    /// \brief Modifies current set to contain all elements present in itself or specified collection.
                    void UnionWith(const IEnumerable<T>& other) override {
                        for (const auto& item : other) {
                            Add(item);
                        }
                    }

                    /// \brief Overload for HashSet.
                    void UnionWith(const HashSet<T>& other) {
                        UnionWith(static_cast<const IEnumerable<T>&>(other));
                    }

                    /// \brief Modifies current set to contain only elements also present in specified collection.
                    void IntersectWith(const IEnumerable<T>& other) override {
                        HashSet<T> otherSet;
                        for (const auto& item : other) {
                            otherSet.Add(item);
                        }
                        Array<T> keys = ToArray();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            if (!otherSet.Contains(keys[i])) {
                                Remove(keys[i]);
                            }
                        }
                    }

                    /// \brief Overload for HashSet.
                    void IntersectWith(const HashSet<T>& other) {
                        IntersectWith(static_cast<const IEnumerable<T>&>(other));
                    }

                    /// \brief Removes all elements in the specified collection from the current set.
                    void ExceptWith(const IEnumerable<T>& other) override {
                        for (const auto& item : other) {
                            Remove(item);
                        }
                    }

                    /// \brief Overload for HashSet.
                    void ExceptWith(const HashSet<T>& other) {
                        ExceptWith(static_cast<const IEnumerable<T>&>(other));
                    }

                    /// \brief Determines whether current set is a subset of specified collection.
                    bool IsSubsetOf(const IEnumerable<T>& other) const override {
                        HashSet<T> otherSet;
                        for (const auto& item : other) {
                            otherSet.Add(item);
                        }
                        Array<T> keys = ToArray();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            if (!otherSet.Contains(keys[i])) return false;
                        }
                        return true;
                    }

                    /// \brief Determines whether current set is a superset of specified collection.
                    bool IsSupersetOf(const IEnumerable<T>& other) const override {
                        for (const auto& item : other) {
                            if (!Contains(item)) return false;
                        }
                        return true;
                    }

                    /// \brief Determines whether current set overlaps with specified collection.
                    bool Overlaps(const IEnumerable<T>& other) const override {
                        for (const auto& item : other) {
                            if (Contains(item)) return true;
                        }
                        return false;
                    }

                    /// \brief Returns an enumerator that iterates through the set.
                    IEnumeratorPtr<T> GetEnumerator() const override {
                        return IEnumeratorPtr<T>(new HashSetEnumerator<T>(m_dict.GetEnumerator()), true);
                    }
                };

            }
        }
    }
}

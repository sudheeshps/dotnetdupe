#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/Dictionary.h"
#include <new>
#include <initializer_list>
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class HashSet
                /// \brief Represents a set of unique values.
                ///
                /// \tparam T The type of elements in the hash set.
                /// \note Conforms to ECMA-335 Partition IV Section 5.39 (System.Collections.Generic.HashSet<T>).
                ///       Implements set operations (UnionWith, IntersectWith, ExceptWith) backed by a hash table dictionary.
                template <typename T>
                class HashSet : public Object {
                private:
                    Dictionary<T, bool> m_dict;

                public:
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

                    /// \brief Gets the number of elements that are contained in a set.
                    /// \return The number of elements contained in the set.
                    int GetCount() const { return m_dict.GetCount(); }

                    /// \brief Adds the specified element to a set.
                    /// \param item The element to add to the set.
                    /// \return true if the element is added to the HashSet object; false if the element is already present.
                    bool Add(const T& item) {
                        if (m_dict.ContainsKey(item)) return false;
                        m_dict.Add(item, true);
                        return true;
                    }

                    /// \brief Removes the specified element from a HashSet object.
                    /// \param item The element to remove.
                    /// \return true if the element is successfully found and removed; otherwise, false.
                    bool Remove(const T& item) {
                        return m_dict.Remove(item);
                    }

                    /// \brief Determines whether a HashSet object contains the specified element.
                    /// \param item The element to locate in the HashSet object.
                    /// \return true if the HashSet object contains the specified element; otherwise, false.
                    bool Contains(const T& item) const {
                        return m_dict.ContainsKey(item);
                    }

                    /// \brief Removes all elements from a HashSet object.
                    void Clear() {
                        m_dict.Clear();
                    }

                    /// \brief Modifies the current HashSet object to contain all elements that are present in itself, the specified collection, or both.
                    /// \param other The collection to compare to the current HashSet object.
                    void UnionWith(const HashSet<T>& other) {
                        Array<T> keys = other.m_dict.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            Add(keys[i]);
                        }
                    }

                    /// \brief Modifies the current HashSet object to contain only elements that are present in that object and in the specified collection.
                    /// \param other The collection to compare to the current HashSet object.
                    void IntersectWith(const HashSet<T>& other) {
                        Array<T> keys = m_dict.GetKeys();
                        for (int i = keys.GetLength() - 1; i >= 0; --i) {
                            if (!other.Contains(keys[i])) {
                                Remove(keys[i]);
                            }
                        }
                    }

                    /// \brief Removes all elements in the specified collection from the current HashSet object.
                    /// \param other The collection of items to remove from the HashSet object.
                    void ExceptWith(const HashSet<T>& other) {
                        Array<T> keys = other.m_dict.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            Remove(keys[i]);
                        }
                    }

                    /// \brief Copies the elements of a HashSet object to an Array.
                    /// \return An Array containing the unique elements of the set.
                    Array<T> ToArray() const {
                        return m_dict.GetKeys();
                    }
                };

            }
        }
    }
}

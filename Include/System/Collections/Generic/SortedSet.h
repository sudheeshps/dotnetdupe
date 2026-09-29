#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/ISet.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class SortedSet
                /// \brief Represents a collection of objects that is maintained in sorted order.
                ///
                /// \tparam T The type of elements in the set.
                /// \note Conforms to ECMA-335 Partition IV Section 5.44 (System.Collections.Generic.SortedSet<T>).
                ///       Maintains sorted unique elements using binary search insertion.
                template <typename T>
                class SortedSet : public virtual ISet<T>,
                                  public virtual IReadOnlyCollection<T> {
                private:
                    List<T> m_lstItems;

                public:
                    using typename IEnumerable<T>::Iterator;
                    using IEnumerable<T>::begin;
                    using IEnumerable<T>::end;

                    /// \brief Initializes a new instance of the SortedSet class.
                    SortedSet() = default;

                    /// \brief Gets the number of elements in the SortedSet.
                    /// \return The number of elements in the SortedSet.
                    int GetCount() const override { return m_lstItems.GetCount(); }

                    /// \brief Adds an element to the set and returns a value that indicates if it was successfully added.
                    /// \param item The element to add to the set.
                    /// \return true if item is added to the set; otherwise, false.
                    bool Add(const T& item) override {
                        int index = m_lstItems.BinarySearch(item);
                        if (index >= 0) return false;

                        m_lstItems.Insert(~index, item);
                        return true;
                    }

                    /// \brief Removes a specified item from the SortedSet.
                    /// \param item The element to remove.
                    /// \return true if the element is found and removed; otherwise, false.
                    bool Remove(const T& item) override {
                        int index = m_lstItems.BinarySearch(item);
                        if (index < 0) return false;

                        m_lstItems.RemoveAt(index);
                        return true;
                    }

                    /// \brief Determines whether the set contains a specific element.
                    /// \param item The element to locate in the set.
                    /// \return true if the set contains item; otherwise, false.
                    bool Contains(const T& item) const override {
                        return m_lstItems.BinarySearch(item) >= 0;
                    }

                    /// \brief Removes all elements from the set.
                    void Clear() override {
                        m_lstItems.Clear();
                    }

                    /// \brief Copies elements to an array.
                    void CopyTo(Array<T>& array, int arrayIndex) const override {
                        m_lstItems.CopyTo(array, arrayIndex);
                    }

                    /// \brief Modifies current set to contain all elements present in itself or specified collection.
                    void UnionWith(const IEnumerable<T>& other) override {
                        for (const auto& item : other) {
                            Add(item);
                        }
                    }

                    /// \brief Overload for SortedSet.
                    void UnionWith(const SortedSet<T>& other) {
                        UnionWith(static_cast<const IEnumerable<T>&>(other));
                    }

                    /// \brief Modifies current set to contain only elements also present in specified collection.
                    void IntersectWith(const IEnumerable<T>& other) override {
                        SortedSet<T> otherSet;
                        for (const auto& item : other) {
                            otherSet.Add(item);
                        }
                        for (int i = m_lstItems.GetCount() - 1; i >= 0; --i) {
                            if (!otherSet.Contains(m_lstItems[i])) {
                                Remove(m_lstItems[i]);
                            }
                        }
                    }

                    /// \brief Overload for SortedSet.
                    void IntersectWith(const SortedSet<T>& other) {
                        IntersectWith(static_cast<const IEnumerable<T>&>(other));
                    }

                    /// \brief Removes all elements in the specified collection from the current set.
                    void ExceptWith(const IEnumerable<T>& other) override {
                        for (const auto& item : other) {
                            Remove(item);
                        }
                    }

                    /// \brief Overload for SortedSet.
                    void ExceptWith(const SortedSet<T>& other) {
                        ExceptWith(static_cast<const IEnumerable<T>&>(other));
                    }

                    /// \brief Determines whether current set is a subset of specified collection.
                    bool IsSubsetOf(const IEnumerable<T>& other) const override {
                        SortedSet<T> otherSet;
                        for (const auto& item : other) {
                            otherSet.Add(item);
                        }
                        for (int i = 0; i < m_lstItems.GetCount(); ++i) {
                            if (!otherSet.Contains(m_lstItems[i])) return false;
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

                    /// \brief Copies the elements of the SortedSet to a new array.
                    /// \return An array containing copies of the elements of the SortedSet.
                    Array<T> ToArray() const {
                        return m_lstItems.ToArray();
                    }

                    /// \brief Returns an enumerator that iterates through the SortedSet.
                    /// \return An enumerator for the SortedSet.
                    IEnumeratorPtr<T> GetEnumerator() const override {
                        return m_lstItems.GetEnumerator();
                    }
                };

            }
        }
    }
}
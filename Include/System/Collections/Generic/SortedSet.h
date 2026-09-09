#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
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
                class SortedSet : public Object {
                private:
                    List<T> m_lstItems;

                public:
                    /// \brief Initializes a new instance of the SortedSet class.
                    SortedSet() = default;

                    /// \brief Gets the number of elements in the SortedSet.
                    /// \return The number of elements in the SortedSet.
                    int GetCount() const { return m_lstItems.GetCount(); }

                    /// \brief Adds an element to the set and returns a value that indicates if it was successfully added.
                    /// \param item The element to add to the set.
                    /// \return true if item is added to the set; otherwise, false.
                    bool Add(const T& item) {
                        int index = m_lstItems.BinarySearch(item);
                        if (index >= 0) return false;

                        m_lstItems.Insert(~index, item);
                        return true;
                    }

                    /// \brief Removes a specified item from the SortedSet.
                    /// \param item The element to remove.
                    /// \return true if the element is found and removed; otherwise, false.
                    bool Remove(const T& item) {
                        int index = m_lstItems.BinarySearch(item);
                        if (index < 0) return false;

                        m_lstItems.RemoveAt(index);
                        return true;
                    }

                    /// \brief Determines whether the set contains a specific element.
                    /// \param item The element to locate in the set.
                    /// \return true if the set contains item; otherwise, false.
                    bool Contains(const T& item) const {
                        return m_lstItems.BinarySearch(item) >= 0;
                    }

                    /// \brief Removes all elements from the set.
                    void Clear() {
                        m_lstItems.Clear();
                    }

                    /// \brief Modifies the current SortedSet object so that it contains all elements that are present in either the current object or the specified collection.
                    /// \param other The collection to compare to the current SortedSet object.
                    void UnionWith(const SortedSet<T>& other) {
                        for (int i = 0; i < other.m_lstItems.GetCount(); ++i) {
                            Add(other.m_lstItems[i]);
                        }
                    }

                    /// \brief Modifies the current SortedSet object so that it contains only elements that are also in a specified collection.
                    /// \param other The collection to compare to the current SortedSet object.
                    void IntersectWith(const SortedSet<T>& other) {
                        for (int i = m_lstItems.GetCount() - 1; i >= 0; --i) {
                            if (!other.Contains(m_lstItems[i])) {
                                Remove(m_lstItems[i]);
                            }
                        }
                    }

                    /// \brief Removes all elements that are in a specified collection from the current SortedSet object.
                    /// \param other The collection of items to remove from the current SortedSet object.
                    void ExceptWith(const SortedSet<T>& other) {
                        for (int i = 0; i < other.m_lstItems.GetCount(); ++i) {
                            Remove(other.m_lstItems[i]);
                        }
                    }

                    /// \brief Copies the elements of the SortedSet to a new array.
                    /// \return An array containing copies of the elements of the SortedSet.
                    Array<T> ToArray() const {
                        return m_lstItems.ToArray();
                    }
                };

            }
        }
    }
}
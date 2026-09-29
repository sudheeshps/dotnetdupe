/// \file ISet.h
/// \brief Provides the base interface for the abstraction of sets.
///
/// Modeled after .NET System.Collections.Generic.ISet<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"

namespace DotNetDupe {
    namespace System {

        template <class T>
        class Array;

        namespace Collections {
            namespace Generic {

                /// \class ISet
                /// \brief Provides the base interface for the abstraction of sets.
                /// \tparam T The type of elements in the set.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class ISet : public virtual IReadOnlyCollection<T> {
                public:
                    /// \brief Virtual destructor.
                    ~ISet() override = default;

                    /// \brief Adds an element to the current set and returns a value indicating if it was successfully added.
                    /// \param item The element to add to the set.
                    /// \return True if the element is added to the set; false if the element is already in the set.
                    virtual bool Add(const T& item) = 0;

                    /// \brief Removes the specified element from the set.
                    /// \param item The element to remove.
                    /// \return True if the element is successfully found and removed; otherwise, false.
                    virtual bool Remove(const T& item) = 0;

                    /// \brief Determines whether the set contains a specific element.
                    /// \param item The element to locate.
                    /// \return True if found; false otherwise.
                    virtual bool Contains(const T& item) const = 0;

                    /// \brief Removes all elements from the set.
                    virtual void Clear() = 0;

                    /// \brief Copies the elements of the set to an array starting at a specific index.
                    /// \param array The destination array.
                    /// \param arrayIndex The zero-based index in array at which copying begins.
                    virtual void CopyTo(Array<T>& array, int arrayIndex) const = 0;

                    /// \brief Modifies the current set so that it contains all elements that are present in both the current set and in the specified collection.
                    /// \param other The collection to compare to the current set.
                    virtual void IntersectWith(const IEnumerable<T>& other) = 0;

                    /// \brief Removes all elements in the specified collection from the current set.
                    /// \param other The collection of items to remove from the set.
                    virtual void ExceptWith(const IEnumerable<T>& other) = 0;

                    /// \brief Modifies the current set so that it contains all elements that are present in the current set, in the specified collection, or in both.
                    /// \param other The collection to compare to the current set.
                    virtual void UnionWith(const IEnumerable<T>& other) = 0;

                    /// \brief Determines whether a set is a subset of a specified collection.
                    /// \param other The collection to compare to the current set.
                    /// \return True if the current set is a subset of other; otherwise, false.
                    virtual bool IsSubsetOf(const IEnumerable<T>& other) const = 0;

                    /// \brief Determines whether a set is a superset of a specified collection.
                    /// \param other The collection to compare to the current set.
                    /// \return True if the current set is a superset of other; otherwise, false.
                    virtual bool IsSupersetOf(const IEnumerable<T>& other) const = 0;

                    /// \brief Determines whether the current set overlaps with the specified collection.
                    /// \param other The collection to compare to the current set.
                    /// \return True if the current set and other share at least one common element; otherwise, false.
                    virtual bool Overlaps(const IEnumerable<T>& other) const = 0;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

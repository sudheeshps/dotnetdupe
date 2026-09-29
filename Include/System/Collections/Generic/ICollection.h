/// \file ICollection.h
/// \brief Defines size, enumerators, and synchronization methods for all generic collections.
///
/// Modeled after .NET System.Collections.Generic.ICollection<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/IEnumerable.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"

namespace DotNetDupe {
    namespace System {

        template <class T>
        class Array;

        namespace Collections {
            namespace Generic {

                /// \class ICollection
                /// \brief Defines methods to manipulate generic collections.
                /// \tparam T The type of the elements in the collection.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class ICollection : public virtual IReadOnlyCollection<T> {
                public:
                    /// \brief Virtual destructor.
                    ~ICollection() override = default;

                    /// \brief Gets a value indicating whether the collection is read-only.
                    /// \return True if the collection is read-only; otherwise, false.
                    virtual bool IsReadOnly() const { return false; }

                    /// \brief Adds an item to the collection.
                    /// \param item The object to add to the collection.
                    virtual void Add(const T& item) = 0;

                    /// \brief Removes all items from the collection.
                    virtual void Clear() = 0;

                    /// \brief Determines whether the collection contains a specific value.
                    /// \param item The object to locate in the collection.
                    /// \return True if item is found in the collection; otherwise, false.
                    virtual bool Contains(const T& item) const = 0;

                    /// \brief Copies the elements of the collection to an Array, starting at a particular Array index.
                    /// \param array The one-dimensional Array that is the destination of the elements.
                    /// \param arrayIndex The zero-based index in array at which copying begins.
                    virtual void CopyTo(Array<T>& array, int arrayIndex) const = 0;

                    /// \brief Removes the first occurrence of a specific object from the collection.
                    /// \param item The object to remove from the collection.
                    /// \return True if item was successfully removed from the collection; otherwise, false.
                    virtual bool Remove(const T& item) = 0;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

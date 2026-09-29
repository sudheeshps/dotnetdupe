/// \file IList.h
/// \brief Represents a collection of objects that can be individually accessed by index.
///
/// Modeled after .NET System.Collections.Generic.IList<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/ICollection.h"
#include "System/Collections/Generic/IReadOnlyList.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class IList
                /// \brief Represents a collection of objects that can be individually accessed by index.
                /// \tparam T The type of elements in the list.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class IList : public virtual ICollection<T>, public virtual IReadOnlyList<T> {
                public:
                    /// \brief Virtual destructor.
                    ~IList() override = default;

                    /// \brief Gets or sets the element at the specified index.
                    /// \param index The zero-based index of the element to get or set.
                    /// \return The element at the specified index.
                    virtual T& operator[](int index) = 0;

                    /// \brief Gets the element at the specified index.
                    /// \param index The zero-based index of the element to get.
                    /// \return The element at the specified index.
                    virtual const T& operator[](int index) const override = 0;

                    /// \brief Determines the index of a specific item in the IList.
                    /// \param item The object to locate in the IList.
                    /// \return The index of item if found; otherwise, -1.
                    virtual int IndexOf(const T& item) const = 0;

                    /// \brief Inserts an item into the IList at the specified index.
                    /// \param index The zero-based index at which item should be inserted.
                    /// \param item The object to insert into the IList.
                    virtual void Insert(int index, const T& item) = 0;

                    /// \brief Removes the IList item at the specified index.
                    /// \param index The zero-based index of the item to remove.
                    virtual void RemoveAt(int index) = 0;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

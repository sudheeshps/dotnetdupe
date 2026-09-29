/// \file IReadOnlyList.h
/// \brief Represents a read-only collection of elements that can be accessed by index.
///
/// Modeled after .NET System.Collections.Generic.IReadOnlyList<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class IReadOnlyList
                /// \brief Represents a read-only collection of elements that can be accessed by index.
                /// \tparam T The type of elements in the read-only list.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class IReadOnlyList : public virtual IReadOnlyCollection<T> {
                public:
                    /// \brief Virtual destructor.
                    ~IReadOnlyList() override = default;

                    /// \brief Gets the element at the specified index in the read-only list.
                    /// \param index The zero-based index of the element to get.
                    /// \return The element at the specified index in the read-only list.
                    virtual const T& operator[](int index) const = 0;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

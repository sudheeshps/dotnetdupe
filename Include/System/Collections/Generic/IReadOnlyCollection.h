/// \file IReadOnlyCollection.h
/// \brief Represents a strongly-typed, read-only collection of elements.
///
/// Modeled after .NET System.Collections.Generic.IReadOnlyCollection<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/IEnumerable.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class IReadOnlyCollection
                /// \brief Represents a strongly-typed, read-only collection of elements.
                /// \tparam T The type of the elements in the collection.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class IReadOnlyCollection : public virtual IEnumerable<T> {
                public:
                    /// \brief Virtual destructor.
                    ~IReadOnlyCollection() override = default;

                    /// \brief Gets the number of elements in the collection.
                    /// \return The number of elements in the collection.
                    virtual int GetCount() const = 0;

                    /// \brief Gets the number of elements in the collection (alias for GetCount).
                    /// \return The number of elements in the collection.
                    int Count() const { return GetCount(); }
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

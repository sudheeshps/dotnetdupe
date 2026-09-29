/// \file IEnumerator.h
/// \brief Defines an enumerator for iterating through a generic collection.
///
/// Modeled after .NET System.Collections.Generic.IEnumerator<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class IEnumerator
                /// \brief Supports a simple iteration over a generic collection.
                /// \tparam T The type of objects to enumerate.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class IEnumerator : public virtual Object {
                public:
                    /// \brief Virtual destructor ensuring polymorphic cleanup.
                    ~IEnumerator() override = default;

                    /// \brief Advances the enumerator to the next element of the collection.
                    /// \return True if the enumerator was successfully advanced; false if at the end.
                    virtual bool MoveNext() = 0;

                    /// \brief Gets the element in the collection at the current position of the enumerator.
                    /// \return The element in the collection at the current position.
                    virtual const T& Current() const = 0;

                    /// \brief DotNetDupe naming convention alias for Current.
                    /// \return The element in the collection at the current position.
                    virtual const T& GetCurrent() const { return Current(); }

                    /// \brief Sets the enumerator to its initial position, which is before the first element.
                    virtual void Reset() = 0;
                };

                /// \brief SmartPointer alias for IEnumerator.
                template <typename T>
                using IEnumeratorPtr = SmartPointer<IEnumerator<T>>;

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

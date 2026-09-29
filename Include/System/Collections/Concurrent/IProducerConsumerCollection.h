/// \file IProducerConsumerCollection.h
/// \brief Defines methods to manipulate thread-safe collections intended for producer/consumer usage.
///
/// Modeled after .NET System.Collections.Concurrent.IProducerConsumerCollection<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Array.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                /// \class IProducerConsumerCollection
                /// \brief Defines methods to manipulate thread-safe collections intended for producer/consumer usage.
                /// \tparam T Specifies the type of elements in the collection.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class IProducerConsumerCollection : public virtual Generic::IReadOnlyCollection<T> {
                public:
                    /// \brief Virtual destructor.
                    ~IProducerConsumerCollection() override = default;

                    /// \brief Attempts to add an object to the collection.
                    /// \param item The object to add.
                    /// \return True if the object was added successfully; otherwise, false.
                    virtual bool TryAdd(const T& item) = 0;

                    /// \brief Attempts to remove and return an object from the collection.
                    /// \param item When this method returns, contains the object removed.
                    /// \return True if an object was removed and returned successfully; otherwise, false.
                    virtual bool TryTake(T& item) = 0;

                    /// \brief Copies the elements of the collection to a new array.
                    /// \return A new array containing a snapshot of elements copied from the collection.
                    virtual Array<T> ToArray() const = 0;

                    /// \brief Copies the elements of the collection to an Array, starting at a specified index.
                    /// \param array The one-dimensional Array that is the destination of the elements.
                    /// \param index The zero-based index in array at which copying begins.
                    virtual void CopyTo(Array<T>& array, int index) const = 0;
                };

            } // namespace Concurrent
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

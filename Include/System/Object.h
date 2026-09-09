/// \file Object.h
/// \brief Base object class for DotNetDupe mirroring .NET System.Object.

#pragma once
#include "Common.h"
#include <cstddef>

namespace DotNetDupe {
    namespace System {
        /// \brief Supports all classes in the DotNetDupe class hierarchy.
        ///
        /// Provides low-level identity, reference equality, hash code generation,
        /// and string representation services for derived types in the library.
        /// Thread-safe for const operations; derived classes must document their concurrency guarantees.
        ///
        /// \note Conforms to ECMA-335 Partition I Section 8.9.1 (System.Object).
        /// \see String, SmartPointer
        class Object {
        public:
            /// \brief Virtual destructor ensuring polymorphic cleanup.
            DOTNETDUPE_API virtual ~Object() = default;

            /// \brief Determines reference equality between two objects.
            /// \param obj Object to compare with current instance.
            /// \return True if objects share identity; otherwise, false.
            DOTNETDUPE_API bool operator == (const Object& obj) const;

            /// \brief Determines whether the specified Object is equal to the current Object.
            /// \param obj Object to compare with current instance.
            /// \return True if equal; otherwise, false.
            DOTNETDUPE_API virtual bool Equals(const Object& obj) const;

            /// \brief Static helper determining whether two object instances are equal.
            /// \param obj1 First object to compare.
            /// \param obj2 Second object to compare.
            /// \return True if equal; otherwise, false.
            DOTNETDUPE_API static bool Equals(const Object& obj1, const Object& obj2);

            /// \brief Serves as the default hash function.
            /// \return An integer hash code for the current object.
            DOTNETDUPE_API virtual int GetHashCode() const;

            /// \brief Returns wide-character string representation.
            /// \return Pointer to string representation or nullptr.
            DOTNETDUPE_API wchar_t* ToStringW() const;

            /// \brief Returns narrow-character string representation.
            /// \return Pointer to string representation or nullptr.
            DOTNETDUPE_API char* ToStringA() const;
        };

        /// \brief Internal memory allocator for generic collections.
        /// \param size Number of bytes to allocate.
        /// \return Allocated memory pointer.
        DOTNETDUPE_API void* AllocateCollectionBuffer(size_t size);

        /// \brief Frees collection buffer allocated with AllocateCollectionBuffer.
        /// \param p Pointer to memory block to release.
        DOTNETDUPE_API void FreeCollectionBuffer(void* p);
    }
}
/// \file Comparable.h
/// \brief Defines the non-generic IComparable interface for ordering objects.
///
/// Modeled after .NET System.IComparable (ECMA-335).

#pragma once
#include "System/Object.h"

namespace DotNetDupe {
    namespace System {
        /// \interface IComparable
        /// \brief Defines a generalized type-specific comparison method that a value type or class implements to order or sort its instances.
        ///
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        class IComparable {
        public:
            /// \brief Virtual destructor for polymorphic cleanup.
            virtual ~IComparable() = default;

            /// \brief Compares the current instance with another object and returns an integer that indicates whether the current instance precedes, follows, or occurs in the same position in the sort order as the other object.
            /// \param obj An object to compare with this instance.
            /// \return A value that indicates the relative order of the objects being compared. Less than zero: this instance precedes obj. Zero: this instance occurs in the same position. Greater than zero: this instance follows obj.
            virtual int CompareTo(const Object& obj) = 0;
        };
    }
}

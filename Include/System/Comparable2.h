/// \file Comparable2.h
/// \brief Defines the generic IComparable<T> interface for type-safe ordering.
///
/// Modeled after .NET System.IComparable<T> (ECMA-335).

#pragma once

namespace DotNetDupe {
    namespace System {
        /// \interface IComparable
        /// \brief Defines a generalized comparison method that a value type or class implements to create a type-safe comparison method for ordering or sorting its instances.
        ///
        /// \tparam T The type of object to compare.
        /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
        template <class T>
        class IComparable {
        public:
            /// \brief Virtual destructor for polymorphic cleanup.
            virtual ~IComparable() = default;

            /// \brief Compares the current instance with another object of the same type and returns an integer indicating relative order.
            /// \param other An object to compare with this instance.
            /// \return A value that indicates the relative order of the objects being compared. Less than zero: this precedes other. Zero: same position. Greater than zero: this follows other.
            virtual int CompareTo(const T& other) = 0;
        };
    }
}

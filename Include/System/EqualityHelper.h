/// \file EqualityHelper.h
/// \brief Provides default equality comparisons for generic types.
///
/// Modeled after .NET System.Collections.Generic.EqualityComparer<T> (ECMA-335).

#pragma once

#include "Common.h"
#include <type_traits>
#include <utility>

namespace DotNetDupe {
    namespace System {

        namespace Internal {

            template <typename T, typename = void>
            struct HasEqualityOperator : std::false_type {};

            template <typename T>
            struct HasEqualityOperator<T, std::void_t<decltype(std::declval<const T&>() == std::declval<const T&>())>> : std::true_type {};

        } // namespace Internal

        /// \struct EqualityHelper
        /// \brief Provides equality comparison between two objects of the same type.
        template <typename T>
        struct EqualityHelper {
            /// \brief Determines whether two objects of type T are equal.
            /// \param a The first object to compare.
            /// \param b The second object to compare.
            /// \return True if the specified objects are equal; otherwise, false.
            static bool Equals(const T& a, const T& b) {
                if constexpr (Internal::HasEqualityOperator<T>::value) {
                    return a == b;
                } else {
                    (void)a;
                    (void)b;
                    return false;
                }
            }
        };

    } // namespace System
} // namespace DotNetDupe

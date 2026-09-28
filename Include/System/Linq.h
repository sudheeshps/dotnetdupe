/// \file Linq.h
/// \brief Master umbrella include file and extension helpers for DotNetDupe LINQ.
///
/// Modeled after .NET System.Linq (ECMA-335).

#pragma once

#include "System/Linq/IGrouping.h"
#include "System/Linq/OrderedEnumerable.h"
#include "System/Linq/Enumerable.h"

namespace DotNetDupe {
    namespace System {
        namespace Linq {

            /// \brief Extension helper converting a List to an Enumerable.
            /// \tparam T Element type.
            /// \param list Const reference to input list.
            /// \return Enumerable wrapping the list.
            template <typename T>
            inline Enumerable<T> AsEnumerable(const Collections::Generic::List<T>& list) {
                return Enumerable<T>(list);
            }

            /// \brief Extension helper moving a List into an Enumerable.
            /// \tparam T Element type.
            /// \param list Rvalue reference to input list.
            /// \return Enumerable owning the moved list.
            template <typename T>
            inline Enumerable<T> AsEnumerable(Collections::Generic::List<T>&& list) {
                return Enumerable<T>(std::move(list));
            }

            /// \brief Extension helper converting an Array to an Enumerable.
            /// \tparam T Element type.
            /// \param arr Const reference to input array.
            /// \return Enumerable wrapping the array elements.
            template <typename T>
            inline Enumerable<T> AsEnumerable(const Array<T>& arr) {
                return Enumerable<T>(arr);
            }

            /// \brief Extension helper converting an initializer list to an Enumerable.
            /// \tparam T Element type.
            /// \param items Initializer list of elements.
            /// \return Enumerable wrapping the initializer list.
            template <typename T>
            inline Enumerable<T> AsEnumerable(const std::initializer_list<T>& items) {
                return Enumerable<T>(items);
            }

            /// \brief Extension helper converting an IGrouping to an Enumerable.
            /// \tparam TKey Group key type.
            /// \tparam TElement Group element type.
            /// \param group Const reference to input grouping.
            /// \return Enumerable wrapping the group elements.
            template <typename TKey, typename TElement>
            inline Enumerable<TElement> AsEnumerable(const IGrouping<TKey, TElement>& group) {
                return Enumerable<TElement>(group.Elements());
            }

            /// \brief Extension helper moving an IGrouping into an Enumerable.
            /// \tparam TKey Group key type.
            /// \tparam TElement Group element type.
            /// \param group Rvalue reference to input grouping.
            /// \return Enumerable wrapping the moved group elements.
            template <typename TKey, typename TElement>
            inline Enumerable<TElement> AsEnumerable(IGrouping<TKey, TElement>&& group) {
                return Enumerable<TElement>(group.ToList());
            }

        } // namespace Linq
    } // namespace System
} // namespace DotNetDupe

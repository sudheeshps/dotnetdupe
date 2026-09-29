/// \file IEnumerable.h
/// \brief Exposes an enumerator, which supports a simple iteration over a non-generic or generic collection.
///
/// Modeled after .NET System.Collections.Generic.IEnumerable<T> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/SmartPointer.h"
#include "System/Collections/Generic/IEnumerator.h"

namespace DotNetDupe {
    namespace System {

        template <class T>
        class Array;

        namespace Linq {
            template <typename T>
            class Enumerable;

            template <typename T>
            class OrderedEnumerable;
        }

        namespace Collections {
            namespace Generic {

                template <typename T>
                class List;

                template <typename T>
                class HashSet;

                template <typename TKey, typename TValue>
                class Dictionary;

                /// \class IEnumerable
                /// \brief Exposes the enumerator, which supports a simple iteration over a collection of a specified type.
                /// \tparam T The type of objects to enumerate.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename T>
                class IEnumerable : public virtual Object {
                public:
                    /// \brief Virtual destructor.
                    ~IEnumerable() override = default;

                    /// \brief Returns an enumerator that iterates through the collection.
                    /// \return An IEnumerator that can be used to iterate through the collection.
                    virtual SmartPointer<IEnumerator<T>> GetEnumerator() const = 0;

                    /// \class Iterator
                    /// \brief STL/C++ standard-compliant range-for loop iterator adapter.
                    class Iterator {
                    private:
                        SmartPointer<IEnumerator<T>> m_spEnum;
                        bool m_bValid;

                    public:
                        /// \brief Constructs an iterator adapter.
                        /// \param spEnum The enumerator pointer.
                        /// \param bStart True if this is the begin iterator, false for end.
                        Iterator(SmartPointer<IEnumerator<T>> spEnum, bool bStart)
                            : m_spEnum(std::move(spEnum)), m_bValid(false) {
                            if (bStart && m_spEnum) {
                                m_bValid = m_spEnum->MoveNext();
                            }
                        }

                        /// \brief Inequality comparison operator for range-for loop termination.
                        bool operator!=(const Iterator& other) const {
                            return m_bValid != other.m_bValid;
                        }

                        /// \brief Advances the enumerator to the next element.
                        Iterator& operator++() {
                            if (m_spEnum) {
                                m_bValid = m_spEnum->MoveNext();
                            }
                            return *this;
                        }

                        /// \brief Dereferences the current element.
                        const T& operator*() const {
                            return m_spEnum->Current();
                        }
                    };

                    /// \brief Returns an iterator to the first element for C++ range-for loops.
                    Iterator begin() const { return Iterator(GetEnumerator(), true); }

                    /// \brief Returns an iterator to the end for C++ range-for loops.
                    Iterator end() const { return Iterator(nullptr, false); }

                    // Fluent LINQ Operators (implemented in System/Linq/Enumerable.h):

                    /// \brief Filters a sequence of values based on a predicate.
                    template <typename F>
                    Linq::Enumerable<T> Where(F&& predicate) const;

                    /// \brief Projects each element of a sequence into a new form.
                    template <typename F>
                    auto Select(F&& selector) const;

                    /// \brief Flattens nested sequences into a single sequence.
                    template <typename F>
                    auto SelectMany(F&& selector) const;

                    /// \brief Sorts the elements of a sequence in ascending order.
                    template <typename FKey>
                    Linq::OrderedEnumerable<T> OrderBy(FKey&& keySelector) const;

                    /// \brief Sorts the elements of a sequence in descending order.
                    template <typename FKey>
                    Linq::OrderedEnumerable<T> OrderByDescending(FKey&& keySelector) const;

                    /// \brief Groups the elements of a sequence according to a specified key selector function.
                    template <typename FKey>
                    auto GroupBy(FKey&& keySelector) const;

                    /// \brief Groups the elements of a sequence according to a specified key selector and element selector.
                    template <typename FKey, typename FElement>
                    auto GroupBy(FKey&& keySelector, FElement&& elementSelector) const;

                    /// \brief Returns a specified number of contiguous elements from the start of a sequence.
                    Linq::Enumerable<T> Take(int count) const;

                    /// \brief Returns elements while a specified condition is true.
                    template <typename F>
                    Linq::Enumerable<T> TakeWhile(F&& predicate) const;

                    /// \brief Bypasses a specified number of elements in a sequence and returns the remaining elements.
                    Linq::Enumerable<T> Skip(int count) const;

                    /// \brief Bypasses elements while a specified condition is true and returns the remaining elements.
                    template <typename F>
                    Linq::Enumerable<T> SkipWhile(F&& predicate) const;

                    /// \brief Returns distinct elements from a sequence.
                    Linq::Enumerable<T> Distinct() const;

                    /// \brief Concatenates two sequences.
                    Linq::Enumerable<T> Concat(const IEnumerable<T>& other) const;

                    /// \brief Produces the set union of two sequences.
                    Linq::Enumerable<T> Union(const IEnumerable<T>& other) const;

                    /// \brief Produces the set intersection of two sequences.
                    Linq::Enumerable<T> Intersect(const IEnumerable<T>& other) const;

                    /// \brief Produces the set difference of two sequences.
                    Linq::Enumerable<T> Except(const IEnumerable<T>& other) const;

                    /// \brief Merges two sequences by using the specified predicate function.
                    template <typename TOther, typename FResult>
                    auto Zip(const IEnumerable<TOther>& other, FResult&& resultSelector) const;

                    /// \brief Inverts the order of the elements in a sequence.
                    Linq::Enumerable<T> Reverse() const;

                    /// \brief Returns the first element of a sequence.
                    T First() const;

                    /// \brief Returns the first element matching a predicate.
                    template <typename F>
                    T First(F&& predicate) const;

                    /// \brief Returns the first element of a sequence, or a default value if the sequence contains no elements.
                    T FirstOrDefault() const;

                    /// \brief Returns the first element matching a predicate or default.
                    template <typename F>
                    T FirstOrDefault(F&& predicate) const;

                    /// \brief Returns the last element of a sequence.
                    T Last() const;

                    /// \brief Returns the last element matching a predicate.
                    template <typename F>
                    T Last(F&& predicate) const;

                    /// \brief Returns the only element of a sequence, and throws an exception if there is not exactly one element.
                    T Single() const;

                    /// \brief Returns the only element of a sequence, or a default value if the sequence is empty.
                    T SingleOrDefault() const;

                    /// \brief Returns the element at a specified index in a sequence.
                    T ElementAt(int index) const;

                    /// \brief Returns the element at a specified index in a sequence or a default value if the index is out of range.
                    T ElementAtOrDefault(int index, const T& defaultValue = T()) const;

                    /// \brief Determines whether a sequence contains any elements.
                    bool Any() const;

                    /// \brief Determines whether any element of a sequence satisfies a condition.
                    template <typename F>
                    bool Any(F&& predicate) const;

                    /// \brief Determines whether all elements of a sequence satisfy a condition.
                    template <typename F>
                    bool All(F&& predicate) const;

                    /// \brief Returns the number of elements in a sequence.
                    int Count() const;

                    /// \brief Returns a number that represents how many elements in the specified sequence satisfy a condition.
                    template <typename F>
                    int Count(F&& predicate) const;

                    /// \brief Computes the sum of a sequence of numeric values.
                    T Sum() const;

                    /// \brief Computes the average of a sequence of numeric values.
                    double Average() const;

                    /// \brief Returns the minimum value in a generic sequence.
                    T Min() const;

                    /// \brief Returns the maximum value in a generic sequence.
                    T Max() const;

                    /// \brief Creates a List<T> from an IEnumerable<T>.
                    List<T> ToList() const;

                    /// \brief Creates an Array<T> from an IEnumerable<T>.
                    Array<T> ToArray() const;

                    /// \brief Creates a HashSet<T> from an IEnumerable<T>.
                    HashSet<T> ToHashSet() const;

                    /// \brief Creates a Dictionary from an IEnumerable<T>.
                    template <typename FKey, typename FValue>
                    auto ToDictionary(FKey&& keySelector, FValue&& valueSelector) const;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

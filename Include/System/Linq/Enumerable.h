/// \file Enumerable.h
/// \brief Provides a set of static and fluent methods for querying objects that implement IEnumerable or List<T>.
///
/// Modeled after .NET System.Linq.Enumerable (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Collections/Generic/HashSet.h"
#include "System/Collections/Generic/IEnumerable.h"
#include "System/InvalidOperationException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/Linq/IGrouping.h"
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <cstdint>

namespace DotNetDupe {
    namespace System {
        namespace Linq {

            template <typename T>
            class OrderedEnumerable;

            /// \class Enumerable
            /// \brief Provides fluent LINQ query operators over strongly typed element sequences.
            /// \tparam T The element type in the sequence.
            ///
            /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
            template <typename T>
            class Enumerable : public virtual Collections::Generic::IEnumerable<T> {
            protected:
                Collections::Generic::List<T> m_items;

            public:
                using ElementType = T;

                /// \brief Default constructor.
                Enumerable() = default;

                /// \brief Constructs an Enumerable from any IEnumerable sequence.
                explicit Enumerable(const Collections::Generic::IEnumerable<T>& seq) {
                    auto spEnum = seq.GetEnumerator();
                    while (spEnum && spEnum->MoveNext()) {
                        m_items.Add(spEnum->GetCurrent());
                    }
                }

                /// \brief Constructs an Enumerable copying from a List.
                explicit Enumerable(const Collections::Generic::List<T>& items) : m_items(items) {}

                /// \brief Constructs an Enumerable moving from a List.
                explicit Enumerable(Collections::Generic::List<T>&& items) : m_items(std::move(items)) {}

                /// \brief Constructs an Enumerable copying from an Array.
                explicit Enumerable(const Array<T>& items) {
                    /// Copy elements from Array to List buffer.
                    int count = items.GetLength();
                    m_items.SetCapacity(count);
                    for (int i = 0; i < count; ++i) {
                        m_items.Add(items[i]);
                    }
                }

                /// \brief Constructs an Enumerable from an initializer list.
                Enumerable(const std::initializer_list<T>& items) : m_items(items) {}

                /// \brief Copy constructor.
                Enumerable(const Enumerable& other) : m_items(other.m_items) {}

                /// \brief Move constructor.
                Enumerable(Enumerable&& other) noexcept : m_items(std::move(other.m_items)) {}

                /// \brief Copy assignment operator.
                Enumerable& operator=(const Enumerable& other) {
                    if (this != &other) {
                        m_items = other.m_items;
                    }
                    return *this;
                }

                /// \brief Move assignment operator.
                Enumerable& operator=(Enumerable&& other) noexcept {
                    if (this != &other) {
                        m_items = std::move(other.m_items);
                    }
                    return *this;
                }

                /// \brief Destructor.
                ~Enumerable() override = default;

                /// \brief Gets the number of elements contained in the sequence.
                int Count() const { return m_items.GetCount(); }

                /// \brief Gets the number of elements contained in the sequence.
                int GetCount() const { return m_items.GetCount(); }

                /// \brief Returns an enumerator that iterates through the sequence.
                Collections::Generic::IEnumeratorPtr<T> GetEnumerator() const override {
                    return m_items.GetEnumerator();
                }

                /// \brief Gets the element at the specified index.
                const T& operator[](int index) const { return m_items[index]; }

                /// \brief Gets the element at the specified index.
                T& operator[](int index) { return m_items[index]; }

                /// \brief Returns a pointer to the beginning of the elements buffer.
                const T* begin() const { return m_items.GetCount() > 0 ? &m_items[0] : nullptr; }

                /// \brief Returns a pointer to the end of the elements buffer.
                const T* end() const { return m_items.GetCount() > 0 ? &m_items[0] + m_items.GetCount() : nullptr; }

                /// \brief Returns a pointer to the beginning of the elements buffer.
                T* begin() { return m_items.GetCount() > 0 ? &m_items[0] : nullptr; }

                /// \brief Returns a pointer to the end of the elements buffer.
                T* end() { return m_items.GetCount() > 0 ? &m_items[0] + m_items.GetCount() : nullptr; }

                /// \brief Returns the count of elements satisfying a predicate.
                template <typename F>
                int Count(F&& predicate) const {
                    /// Count matching elements.
                    int count = 0;
                    int total = m_items.GetCount();
                    for (int i = 0; i < total; ++i) {
                        if (predicate(m_items[i])) count++;
                    }
                    return count;
                }

                /// \brief Materializes sequence into a List.
                Collections::Generic::List<T> ToList() const { return m_items; }

                /// \brief Materializes sequence into an Array.
                Array<T> ToArray() const {
                    /// Convert sequence to Array.
                    int count = m_items.GetCount();
                    Array<T> arr(count);
                    for (int i = 0; i < count; ++i) { arr[i] = m_items[i]; }
                    return arr;
                }

                /// \brief Materializes sequence into a Dictionary.
                template <typename FKey, typename FValue>
                auto ToDictionary(FKey&& keySelector, FValue&& valueSelector) const {
                    /// Build key-value dictionary.
                    using TKey = decltype(keySelector(std::declval<T>()));
                    using TValue = decltype(valueSelector(std::declval<T>()));
                    Collections::Generic::Dictionary<TKey, TValue> dict;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        dict.Add(keySelector(m_items[i]), valueSelector(m_items[i]));
                    }
                    return dict;
                }

                /// \brief Materializes sequence into a HashSet.
                Collections::Generic::HashSet<T> ToHashSet() const {
                    /// Build unique hash set.
                    Collections::Generic::HashSet<T> set;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) { set.Add(m_items[i]); }
                    return set;
                }

                /// \brief Filters elements based on a predicate.
                template <typename F>
                Enumerable<T> Where(F&& predicate) const {
                    /// Collect matching elements.
                    Collections::Generic::List<T> result;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (predicate(m_items[i])) {
                            result.Add(m_items[i]);
                        }
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Projects each element of a sequence into a new form.
                template <typename F>
                auto Select(F&& selector) const {
                    /// Transform each element.
                    using TResult = decltype(selector(std::declval<T>()));
                    Collections::Generic::List<TResult> result(m_items.GetCount());
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        result.Add(selector(m_items[i]));
                    }
                    return Enumerable<TResult>(std::move(result));
                }

                /// \brief Flattens nested sequences into a single sequence.
                template <typename F>
                auto SelectMany(F&& selector) const {
                    /// Flatten child sequences into single stream.
                    using TSeq = std::decay_t<decltype(selector(std::declval<T>()))>;
                    using TItem = std::decay_t<decltype(std::declval<TSeq>()[0])>;
                    Collections::Generic::List<TItem> result;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        auto seq = selector(m_items[i]);
                        int subCount = seq.GetCount();
                        for (int j = 0; j < subCount; ++j) {
                            result.Add(seq[j]);
                        }
                    }
                    return Enumerable<TItem>(std::move(result));
                }

                /// \brief Sorts elements in ascending order according to a key.
                template <typename FKey>
                OrderedEnumerable<T> OrderBy(FKey&& keySelector) const;

                /// \brief Sorts elements in descending order according to a key.
                template <typename FKey>
                OrderedEnumerable<T> OrderByDescending(FKey&& keySelector) const;

                /// \brief Returns a specified number of contiguous elements from the start.
                Enumerable<T> Take(int count) const {
                    /// Partition initial elements.
                    Collections::Generic::List<T> result;
                    int limit = (count < m_items.GetCount()) ? count : m_items.GetCount();
                    if (limit < 0) limit = 0;
                    for (int i = 0; i < limit; ++i) {
                        result.Add(m_items[i]);
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Returns elements while a specified condition is true.
                template <typename F>
                Enumerable<T> TakeWhile(F&& predicate) const {
                    /// Take while predicate evaluates true.
                    Collections::Generic::List<T> result;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (!predicate(m_items[i])) break;
                        result.Add(m_items[i]);
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Bypasses a specified number of elements and returns the rest.
                Enumerable<T> Skip(int count) const {
                    /// Skip prefix elements.
                    Collections::Generic::List<T> result;
                    int start = (count > 0) ? count : 0;
                    int total = m_items.GetCount();
                    for (int i = start; i < total; ++i) {
                        result.Add(m_items[i]);
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Bypasses elements as long as a condition is true, then returns the rest.
                template <typename F>
                Enumerable<T> SkipWhile(F&& predicate) const {
                    /// Skip until condition fails.
                    Collections::Generic::List<T> result;
                    int count = m_items.GetCount();
                    bool skipping = true;
                    for (int i = 0; i < count; ++i) {
                        if (skipping && !predicate(m_items[i])) skipping = false;
                        if (!skipping) result.Add(m_items[i]);
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Returns distinct elements from a sequence.
                Enumerable<T> Distinct() const {
                    /// Deduplicate sequence using HashSet.
                    Collections::Generic::HashSet<T> seen;
                    Collections::Generic::List<T> result;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (!seen.Contains(m_items[i])) {
                            seen.Add(m_items[i]);
                            result.Add(m_items[i]);
                        }
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Groups elements of a sequence according to a key selector.
                template <typename FKey>
                auto GroupBy(FKey&& keySelector) const {
                    /// Group elements into buckets preserving appearance order.
                    using TKey = decltype(keySelector(std::declval<T>()));
                    Collections::Generic::Dictionary<TKey, Collections::Generic::List<T>> map;
                    Collections::Generic::List<TKey> orderedKeys;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        TKey key = keySelector(m_items[i]);
                        if (!map.ContainsKey(key)) {
                            map.Add(key, Collections::Generic::List<T>());
                            orderedKeys.Add(key);
                        }
                        map[key].Add(m_items[i]);
                    }
                    Collections::Generic::List<IGrouping<TKey, T>> groups;
                    int groupCount = orderedKeys.GetCount();
                    for (int i = 0; i < groupCount; ++i) {
                        const TKey& k = orderedKeys[i];
                        groups.Add(IGrouping<TKey, T>(k, std::move(map[k])));
                    }
                    return Enumerable<IGrouping<TKey, T>>(std::move(groups));
                }

                /// \brief Groups elements according to a key selector and element selector.
                template <typename FKey, typename FElement>
                auto GroupBy(FKey&& keySelector, FElement&& elementSelector) const {
                    /// Group transformed elements into buckets.
                    using TKey = decltype(keySelector(std::declval<T>()));
                    using TElem = decltype(elementSelector(std::declval<T>()));
                    Collections::Generic::Dictionary<TKey, Collections::Generic::List<TElem>> map;
                    Collections::Generic::List<TKey> orderedKeys;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        TKey key = keySelector(m_items[i]);
                        if (!map.ContainsKey(key)) {
                            map.Add(key, Collections::Generic::List<TElem>());
                            orderedKeys.Add(key);
                        }
                        map[key].Add(elementSelector(m_items[i]));
                    }
                    Collections::Generic::List<IGrouping<TKey, TElem>> groups;
                    int groupCount = orderedKeys.GetCount();
                    for (int i = 0; i < groupCount; ++i) {
                        const TKey& k = orderedKeys[i];
                        groups.Add(IGrouping<TKey, TElem>(k, std::move(map[k])));
                    }
                    return Enumerable<IGrouping<TKey, TElem>>(std::move(groups));
                }

                /// \brief Concatenates two sequences.
                Enumerable<T> Concat(const Collections::Generic::IEnumerable<T>& other) const {
                    /// Append elements of other sequence.
                    Collections::Generic::List<T> result = m_items;
                    auto spEnum = other.GetEnumerator();
                    while (spEnum && spEnum->MoveNext()) {
                        result.Add(spEnum->GetCurrent());
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Produces the set union of two sequences.
                Enumerable<T> Union(const Collections::Generic::IEnumerable<T>& other) const {
                    return Concat(other).Distinct();
                }

                /// \brief Produces the set intersection of two sequences.
                Enumerable<T> Intersect(const Collections::Generic::IEnumerable<T>& other) const {
                    /// Intersect elements between two sequences.
                    Collections::Generic::HashSet<T> otherSet;
                    auto spEnum = other.GetEnumerator();
                    while (spEnum && spEnum->MoveNext()) {
                        otherSet.Add(spEnum->GetCurrent());
                    }
                    Collections::Generic::HashSet<T> added;
                    Collections::Generic::List<T> result;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (otherSet.Contains(m_items[i]) && !added.Contains(m_items[i])) {
                            added.Add(m_items[i]);
                            result.Add(m_items[i]);
                        }
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Produces the set difference of two sequences.
                Enumerable<T> Except(const Collections::Generic::IEnumerable<T>& other) const {
                    /// Exclude elements present in other sequence.
                    Collections::Generic::HashSet<T> otherSet;
                    auto spEnum = other.GetEnumerator();
                    while (spEnum && spEnum->MoveNext()) {
                        otherSet.Add(spEnum->GetCurrent());
                    }
                    Collections::Generic::HashSet<T> added;
                    Collections::Generic::List<T> result;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (!otherSet.Contains(m_items[i]) && !added.Contains(m_items[i])) {
                            added.Add(m_items[i]);
                            result.Add(m_items[i]);
                        }
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Merges two sequences by using the specified predicate function.
                template <typename TOther, typename FResult>
                auto Zip(const Collections::Generic::IEnumerable<TOther>& other, FResult&& resultSelector) const {
                    /// Pairwise zip two sequences.
                    using TResult = decltype(resultSelector(std::declval<T>(), std::declval<TOther>()));
                    Collections::Generic::List<TResult> result;
                    auto spEnum = other.GetEnumerator();
                    int i = 0;
                    int count = m_items.GetCount();
                    while (i < count && spEnum && spEnum->MoveNext()) {
                        result.Add(resultSelector(m_items[i], spEnum->GetCurrent()));
                        i++;
                    }
                    return Enumerable<TResult>(std::move(result));
                }

                /// \brief Inverts the order of the elements in a sequence.
                Enumerable<T> Reverse() const {
                    /// Reverse elements into new sequence.
                    Collections::Generic::List<T> result;
                    for (int i = m_items.GetCount() - 1; i >= 0; --i) {
                        result.Add(m_items[i]);
                    }
                    return Enumerable<T>(std::move(result));
                }

                /// \brief Determines whether a sequence contains any elements.
                bool Any() const { return m_items.GetCount() > 0; }

                /// \brief Determines whether any element of a sequence satisfies a condition.
                template <typename F>
                bool Any(F&& predicate) const {
                    /// Check if any item matches predicate.
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (predicate(m_items[i])) return true;
                    }
                    return false;
                }

                /// \brief Determines whether all elements of a sequence satisfy a condition.
                template <typename F>
                bool All(F&& predicate) const {
                    /// Check if all items match predicate.
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (!predicate(m_items[i])) return false;
                    }
                    return true;
                }

                /// \brief Determines whether a sequence contains a specified element.
                bool Contains(const T& value) const { return m_items.Contains(value); }

                /// \brief Returns the first element of a sequence.
                T First() const {
                    if (m_items.GetCount() == 0) {
                        throw InvalidOperationException("Sequence contains no elements.");
                    }
                    return m_items[0];
                }

                /// \brief Returns the first element in a sequence that satisfies a specified condition.
                template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, const T&>>>
                T First(F&& predicate) const {
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (predicate(m_items[i])) return m_items[i];
                    }
                    throw InvalidOperationException("Sequence contains no matching element.");
                }

                /// \brief Returns the first element, or a default value if the sequence is empty.
                T FirstOrDefault(const T& defaultValue = T()) const {
                    return (m_items.GetCount() > 0) ? m_items[0] : defaultValue;
                }

                /// \brief Returns the first element matching a predicate, or a default value.
                template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, const T&>>>
                T FirstOrDefault(F&& predicate, const T& defaultValue = T()) const {
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) {
                        if (predicate(m_items[i])) return m_items[i];
                    }
                    return defaultValue;
                }

                /// \brief Returns the last element of a sequence.
                T Last() const {
                    int count = m_items.GetCount();
                    if (count == 0) {
                        throw InvalidOperationException("Sequence contains no elements.");
                    }
                    return m_items[count - 1];
                }

                /// \brief Returns the last element in a sequence that satisfies a specified condition.
                template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, const T&>>>
                T Last(F&& predicate) const {
                    for (int i = m_items.GetCount() - 1; i >= 0; --i) {
                        if (predicate(m_items[i])) return m_items[i];
                    }
                    throw InvalidOperationException("Sequence contains no matching element.");
                }

                /// \brief Returns the last element, or a default value if the sequence is empty.
                T LastOrDefault(const T& defaultValue = T()) const {
                    int count = m_items.GetCount();
                    return (count > 0) ? m_items[count - 1] : defaultValue;
                }

                /// \brief Returns the last element matching a predicate, or a default value.
                template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, const T&>>>
                T LastOrDefault(F&& predicate, const T& defaultValue = T()) const {
                    for (int i = m_items.GetCount() - 1; i >= 0; --i) {
                        if (predicate(m_items[i])) return m_items[i];
                    }
                    return defaultValue;
                }

                /// \brief Returns the only element of a sequence, throwing if not exactly one element.
                T Single() const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    if (count > 1) throw InvalidOperationException("Sequence contains more than one element.");
                    return m_items[0];
                }

                /// \brief Returns the only element satisfying a condition, throwing if not exactly one.
                template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, const T&>>>
                T Single(F&& predicate) const {
                    int count = m_items.GetCount();
                    int matchIndex = -1;
                    for (int i = 0; i < count; ++i) {
                        if (predicate(m_items[i])) {
                            if (matchIndex >= 0) throw InvalidOperationException("Sequence contains more than one matching element.");
                            matchIndex = i;
                        }
                    }
                    if (matchIndex < 0) throw InvalidOperationException("Sequence contains no matching element.");
                    return m_items[matchIndex];
                }

                /// \brief Returns the only element, or a default value if the sequence is empty.
                T SingleOrDefault(const T& defaultValue = T()) const {
                    int count = m_items.GetCount();
                    if (count == 0) return defaultValue;
                    if (count > 1) throw InvalidOperationException("Sequence contains more than one element.");
                    return m_items[0];
                }

                /// \brief Returns the only element matching a predicate, or default if empty.
                template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, const T&>>>
                T SingleOrDefault(F&& predicate, const T& defaultValue = T()) const {
                    int count = m_items.GetCount();
                    int matchIndex = -1;
                    for (int i = 0; i < count; ++i) {
                        if (predicate(m_items[i])) {
                            if (matchIndex >= 0) throw InvalidOperationException("Sequence contains more than one matching element.");
                            matchIndex = i;
                        }
                    }
                    return (matchIndex >= 0) ? m_items[matchIndex] : defaultValue;
                }

                /// \brief Returns the element at a specified index in a sequence.
                T ElementAt(int index) const {
                    if (index < 0 || index >= m_items.GetCount()) {
                        throw ArgumentOutOfRangeException("Index was out of range.");
                    }
                    return m_items[index];
                }

                /// \brief Returns the element at a specified index or a default value.
                T ElementAtOrDefault(int index, const T& defaultValue = T()) const {
                    if (index < 0 || index >= m_items.GetCount()) return defaultValue;
                    return m_items[index];
                }

                /// \brief Computes the sum of a sequence of numeric values.
                T Sum() const {
                    T total{};
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) { total += m_items[i]; }
                    return total;
                }

                /// \brief Computes the sum of projected values.
                template <typename F>
                auto Sum(F&& selector) const {
                    using TResult = decltype(selector(std::declval<T>()));
                    TResult total{};
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) { total += selector(m_items[i]); }
                    return total;
                }

                /// \brief Returns the minimum value in a sequence of values.
                T Min() const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    T minVal = m_items[0];
                    for (int i = 1; i < count; ++i) {
                        if (m_items[i] < minVal) minVal = m_items[i];
                    }
                    return minVal;
                }

                /// \brief Invokes a transform function on each element and returns the minimum value.
                template <typename F>
                auto Min(F&& selector) const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    auto minVal = selector(m_items[0]);
                    for (int i = 1; i < count; ++i) {
                        auto val = selector(m_items[i]);
                        if (val < minVal) minVal = val;
                    }
                    return minVal;
                }

                /// \brief Returns the maximum value in a sequence of values.
                T Max() const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    T maxVal = m_items[0];
                    for (int i = 1; i < count; ++i) {
                        if (maxVal < m_items[i]) maxVal = m_items[i];
                    }
                    return maxVal;
                }

                /// \brief Invokes a transform function on each element and returns the maximum value.
                template <typename F>
                auto Max(F&& selector) const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    auto maxVal = selector(m_items[0]);
                    for (int i = 1; i < count; ++i) {
                        auto val = selector(m_items[i]);
                        if (maxVal < val) maxVal = val;
                    }
                    return maxVal;
                }

                /// \brief Computes the average of a sequence of numeric values.
                double Average() const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    double sum = 0.0;
                    for (int i = 0; i < count; ++i) { sum += static_cast<double>(m_items[i]); }
                    return sum / count;
                }

                /// \brief Computes the average of projected numeric values.
                template <typename F>
                double Average(F&& selector) const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    double sum = 0.0;
                    for (int i = 0; i < count; ++i) { sum += static_cast<double>(selector(m_items[i])); }
                    return sum / count;
                }

                /// \brief Applies an accumulator function over a sequence.
                template <typename F>
                T Aggregate(F&& func) const {
                    int count = m_items.GetCount();
                    if (count == 0) throw InvalidOperationException("Sequence contains no elements.");
                    T accum = m_items[0];
                    for (int i = 1; i < count; ++i) { accum = func(accum, m_items[i]); }
                    return accum;
                }

                /// \brief Applies an accumulator function over a sequence with seed value.
                template <typename TAccum, typename F>
                TAccum Aggregate(TAccum seed, F&& func) const {
                    TAccum accum = seed;
                    int count = m_items.GetCount();
                    for (int i = 0; i < count; ++i) { accum = func(accum, m_items[i]); }
                    return accum;
                }

                /// \brief Generates a sequence of integral numbers within a specified range.
                static Enumerable<int> Range(int start, int count) {
                    if (count < 0) throw ArgumentOutOfRangeException("Count cannot be negative.");
                    Collections::Generic::List<int> list(count);
                    for (int i = 0; i < count; ++i) { list.Add(start + i); }
                    return Enumerable<int>(std::move(list));
                }

                /// \brief Generates a sequence that contains one repeated value.
                static Enumerable<T> Repeat(const T& element, int count) {
                    if (count < 0) throw ArgumentOutOfRangeException("Count cannot be negative.");
                    Collections::Generic::List<T> list(count);
                    for (int i = 0; i < count; ++i) { list.Add(element); }
                    return Enumerable<T>(std::move(list));
                }

                /// \brief Returns an empty sequence.
                static Enumerable<T> Empty() {
                    return Enumerable<T>();
                }
            };

        } // namespace Linq
    } // namespace System
} // namespace DotNetDupe

#include "System/Linq/OrderedEnumerable.h"

namespace DotNetDupe {
    namespace System {
        namespace Linq {

            template <typename T>
            template <typename FKey>
            OrderedEnumerable<T> Enumerable<T>::OrderBy(FKey&& keySelector) const {
                Internal::KeyComparer<T, std::decay_t<FKey>> comp(std::forward<FKey>(keySelector), false);
                return OrderedEnumerable<T>(m_items, &comp);
            }

            template <typename T>
            template <typename FKey>
            OrderedEnumerable<T> Enumerable<T>::OrderByDescending(FKey&& keySelector) const {
                Internal::KeyComparer<T, std::decay_t<FKey>> comp(std::forward<FKey>(keySelector), true);
                return OrderedEnumerable<T>(m_items, &comp);
            }

        } // namespace Linq

        namespace Collections {
            namespace Generic {

                template <typename T>
                template <typename F>
                Linq::Enumerable<T> IEnumerable<T>::Where(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).Where(std::forward<F>(predicate));
                }

                template <typename T>
                template <typename F>
                auto IEnumerable<T>::Select(F&& selector) const {
                    return Linq::Enumerable<T>(*this).Select(std::forward<F>(selector));
                }

                template <typename T>
                template <typename FKey>
                Linq::OrderedEnumerable<T> IEnumerable<T>::OrderBy(FKey&& keySelector) const {
                    return Linq::Enumerable<T>(*this).OrderBy(std::forward<FKey>(keySelector));
                }

                template <typename T>
                template <typename FKey>
                Linq::OrderedEnumerable<T> IEnumerable<T>::OrderByDescending(FKey&& keySelector) const {
                    return Linq::Enumerable<T>(*this).OrderByDescending(std::forward<FKey>(keySelector));
                }

                template <typename T>
                template <typename FKey>
                auto IEnumerable<T>::GroupBy(FKey&& keySelector) const {
                    return Linq::Enumerable<T>(*this).GroupBy(std::forward<FKey>(keySelector));
                }

                template <typename T>
                template <typename FKey, typename FElement>
                auto IEnumerable<T>::GroupBy(FKey&& keySelector, FElement&& elementSelector) const {
                    return Linq::Enumerable<T>(*this).GroupBy(std::forward<FKey>(keySelector), std::forward<FElement>(elementSelector));
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Take(int count) const {
                    return Linq::Enumerable<T>(*this).Take(count);
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Skip(int count) const {
                    return Linq::Enumerable<T>(*this).Skip(count);
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Distinct() const {
                    return Linq::Enumerable<T>(*this).Distinct();
                }

                template <typename T>
                T IEnumerable<T>::First() const {
                    return Linq::Enumerable<T>(*this).First();
                }

                template <typename T>
                T IEnumerable<T>::FirstOrDefault() const {
                    return Linq::Enumerable<T>(*this).FirstOrDefault();
                }

                template <typename T>
                T IEnumerable<T>::Single() const {
                    return Linq::Enumerable<T>(*this).Single();
                }

                template <typename T>
                T IEnumerable<T>::SingleOrDefault() const {
                    return Linq::Enumerable<T>(*this).SingleOrDefault();
                }

                template <typename T>
                T IEnumerable<T>::ElementAt(int index) const {
                    return Linq::Enumerable<T>(*this).ElementAt(index);
                }

                template <typename T>
                T IEnumerable<T>::ElementAtOrDefault(int index, const T& defaultValue) const {
                    return Linq::Enumerable<T>(*this).ElementAtOrDefault(index, defaultValue);
                }

                template <typename T>
                bool IEnumerable<T>::Any() const {
                    return Linq::Enumerable<T>(*this).Any();
                }

                template <typename T>
                template <typename F>
                bool IEnumerable<T>::Any(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).Any(std::forward<F>(predicate));
                }

                template <typename T>
                template <typename F>
                bool IEnumerable<T>::All(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).All(std::forward<F>(predicate));
                }

                template <typename T>
                int IEnumerable<T>::Count() const {
                    return Linq::Enumerable<T>(*this).Count();
                }

                template <typename T>
                template <typename F>
                int IEnumerable<T>::Count(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).Count(std::forward<F>(predicate));
                }

                template <typename T>
                T IEnumerable<T>::Sum() const {
                    return Linq::Enumerable<T>(*this).Sum();
                }

                template <typename T>
                double IEnumerable<T>::Average() const {
                    return Linq::Enumerable<T>(*this).Average();
                }

                template <typename T>
                T IEnumerable<T>::Min() const {
                    return Linq::Enumerable<T>(*this).Min();
                }

                template <typename T>
                T IEnumerable<T>::Max() const {
                    return Linq::Enumerable<T>(*this).Max();
                }

                template <typename T>
                template <typename F>
                auto IEnumerable<T>::SelectMany(F&& selector) const {
                    return Linq::Enumerable<T>(*this).SelectMany(std::forward<F>(selector));
                }

                template <typename T>
                template <typename F>
                Linq::Enumerable<T> IEnumerable<T>::TakeWhile(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).TakeWhile(std::forward<F>(predicate));
                }

                template <typename T>
                template <typename F>
                Linq::Enumerable<T> IEnumerable<T>::SkipWhile(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).SkipWhile(std::forward<F>(predicate));
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Concat(const IEnumerable<T>& other) const {
                    return Linq::Enumerable<T>(*this).Concat(other);
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Union(const IEnumerable<T>& other) const {
                    return Linq::Enumerable<T>(*this).Union(other);
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Intersect(const IEnumerable<T>& other) const {
                    return Linq::Enumerable<T>(*this).Intersect(other);
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Except(const IEnumerable<T>& other) const {
                    return Linq::Enumerable<T>(*this).Except(other);
                }

                template <typename T>
                template <typename TOther, typename FResult>
                auto IEnumerable<T>::Zip(const IEnumerable<TOther>& other, FResult&& resultSelector) const {
                    return Linq::Enumerable<T>(*this).Zip(other, std::forward<FResult>(resultSelector));
                }

                template <typename T>
                Linq::Enumerable<T> IEnumerable<T>::Reverse() const {
                    return Linq::Enumerable<T>(*this).Reverse();
                }

                template <typename T>
                template <typename F>
                T IEnumerable<T>::First(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).First(std::forward<F>(predicate));
                }

                template <typename T>
                template <typename F>
                T IEnumerable<T>::FirstOrDefault(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).FirstOrDefault(std::forward<F>(predicate));
                }

                template <typename T>
                T IEnumerable<T>::Last() const {
                    return Linq::Enumerable<T>(*this).Last();
                }

                template <typename T>
                template <typename F>
                T IEnumerable<T>::Last(F&& predicate) const {
                    return Linq::Enumerable<T>(*this).Last(std::forward<F>(predicate));
                }

                template <typename T>
                List<T> IEnumerable<T>::ToList() const {
                    return Linq::Enumerable<T>(*this).ToList();
                }

                template <typename T>
                Array<T> IEnumerable<T>::ToArray() const {
                    return Linq::Enumerable<T>(*this).ToArray();
                }

                template <typename T>
                HashSet<T> IEnumerable<T>::ToHashSet() const {
                    return Linq::Enumerable<T>(*this).ToHashSet();
                }

                template <typename T>
                template <typename FKey, typename FValue>
                auto IEnumerable<T>::ToDictionary(FKey&& keySelector, FValue&& valueSelector) const {
                    return Linq::Enumerable<T>(*this).ToDictionary(std::forward<FKey>(keySelector), std::forward<FValue>(valueSelector));
                }

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

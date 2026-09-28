/// \file OrderedEnumerable.h
/// \brief Represents a sorted sequence supporting secondary and tertiary key ordering.
///
/// Modeled after .NET System.Linq.IOrderedEnumerable<TElement> (ECMA-335).

#pragma once

#include "System/Collections/Generic/List.h"
#include <utility>
#include <type_traits>

namespace DotNetDupe {
    namespace System {
        namespace Linq {

            template <typename T>
            class Enumerable;

            namespace Internal {

                /// \interface IElementComparer
                /// \brief Internal comparison contract for multi-key sorting chains.
                template <typename T>
                struct IElementComparer {
                    virtual ~IElementComparer() = default;
                    virtual int Compare(const T& a, const T& b) const = 0;
                    virtual IElementComparer* Clone() const = 0;
                };

                /// \struct KeyComparer
                /// \brief Primary key extractor comparison node.
                template <typename T, typename FKey>
                struct KeyComparer : IElementComparer<T> {
                    FKey m_keySelector;
                    bool m_bDescending;

                    KeyComparer(const FKey& keySelector, bool bDesc)
                        : m_keySelector(keySelector), m_bDescending(bDesc) {}

                    int Compare(const T& a, const T& b) const override {
                        auto keyA = m_keySelector(a);
                        auto keyB = m_keySelector(b);
                        int res = (keyA < keyB) ? -1 : ((keyB < keyA) ? 1 : 0);
                        return m_bDescending ? -res : res;
                    }

                    IElementComparer<T>* Clone() const override {
                        return new KeyComparer<T, FKey>(m_keySelector, m_bDescending);
                    }
                };

                /// \struct ChainedComparer
                /// \brief Secondary or tertiary key comparison node evaluating parent first.
                template <typename T, typename FKey>
                struct ChainedComparer : IElementComparer<T> {
                    IElementComparer<T>* m_pParent;
                    FKey m_keySelector;
                    bool m_bDescending;

                    ChainedComparer(IElementComparer<T>* pParent, const FKey& keySelector, bool bDesc)
                        : m_pParent(pParent ? pParent->Clone() : nullptr),
                          m_keySelector(keySelector),
                          m_bDescending(bDesc) {}

                    ~ChainedComparer() override {
                        delete m_pParent;
                    }

                    int Compare(const T& a, const T& b) const override {
                        if (m_pParent) {
                            int parentRes = m_pParent->Compare(a, b);
                            if (parentRes != 0) return parentRes;
                        }
                        auto keyA = m_keySelector(a);
                        auto keyB = m_keySelector(b);
                        int res = (keyA < keyB) ? -1 : ((keyB < keyA) ? 1 : 0);
                        return m_bDescending ? -res : res;
                    }

                    IElementComparer<T>* Clone() const override {
                        return new ChainedComparer<T, FKey>(m_pParent, m_keySelector, m_bDescending);
                    }
                };

                /// \class LinqSortHelper
                /// \brief In-place stable quicksort implementation for List without STL algorithms.
                template <typename T>
                class LinqSortHelper {
                public:
                    /// \brief Swaps two integer indices in place.
                    static void SwapInt(int& a, int& b) {
                        int tmp = a;
                        a = b;
                        b = tmp;
                    }

                    /// \brief Partitions an index list around a median-of-three pivot.
                    static int Partition(Collections::Generic::List<int>& indices, const Collections::Generic::List<T>& items, int low, int high, const IElementComparer<T>& comp) {
                        int mid = low + (high - low) / 2;
                        SwapInt(indices[mid], indices[high]);
                        int pivotIdx = indices[high];
                        int i = low - 1;
                        for (int j = low; j < high; ++j) {
                            int curIdx = indices[j];
                            int c = comp.Compare(items[curIdx], items[pivotIdx]);
                            if (c < 0 || (c == 0 && curIdx < pivotIdx)) {
                                i++;
                                SwapInt(indices[i], indices[j]);
                            }
                        }
                        SwapInt(indices[i + 1], indices[high]);
                        return i + 1;
                    }

                    /// \brief Recursively sorts index list using tail-call optimization.
                    static void QuickSort(Collections::Generic::List<int>& indices, const Collections::Generic::List<T>& items, int low, int high, const IElementComparer<T>& comp) {
                        while (low < high) {
                            int pi = Partition(indices, items, low, high, comp);
                            if (pi - low < high - pi) {
                                QuickSort(indices, items, low, pi - 1, comp);
                                low = pi + 1;
                            } else {
                                QuickSort(indices, items, pi + 1, high, comp);
                                high = pi - 1;
                            }
                        }
                    }

                    /// \brief Applies stable sorting to a list and returns ordered result.
                    static Collections::Generic::List<T> Sort(const Collections::Generic::List<T>& items, const IElementComparer<T>& comp) {
                        int count = items.GetCount();
                        Collections::Generic::List<int> indices(count);
                        for (int i = 0; i < count; ++i) { indices.Add(i); }
                        if (count > 1) { QuickSort(indices, items, 0, count - 1, comp); }
                        Collections::Generic::List<T> result(count);
                        for (int i = 0; i < count; ++i) { result.Add(items[indices[i]]); }
                        return result;
                    }
                };

            } // namespace Internal

            /// \class OrderedEnumerable
            /// \brief Represents an ordered sequence supporting secondary sorting keys.
            /// \tparam T The type of elements in the sequence.
            ///
            /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
            template <typename T>
            class OrderedEnumerable : public Enumerable<T> {
            private:
                Collections::Generic::List<T> m_sourceItems;
                Internal::IElementComparer<T>* m_pComparer;

            public:
                /// \brief Default constructor.
                OrderedEnumerable() : m_pComparer(nullptr) {}

                /// \brief Constructs an ordered enumerable applying the specified comparison chain.
                OrderedEnumerable(const Collections::Generic::List<T>& sourceItems, Internal::IElementComparer<T>* pComp)
                    : Enumerable<T>(pComp ? Internal::LinqSortHelper<T>::Sort(sourceItems, *pComp) : sourceItems),
                      m_sourceItems(sourceItems),
                      m_pComparer(pComp ? pComp->Clone() : nullptr) {}

                /// \brief Copy constructor.
                OrderedEnumerable(const OrderedEnumerable& other)
                    : Enumerable<T>(other),
                      m_sourceItems(other.m_sourceItems),
                      m_pComparer(other.m_pComparer ? other.m_pComparer->Clone() : nullptr) {}

                /// \brief Move constructor.
                OrderedEnumerable(OrderedEnumerable&& other) noexcept
                    : Enumerable<T>(std::move(other)),
                      m_sourceItems(std::move(other.m_sourceItems)),
                      m_pComparer(other.m_pComparer) {
                    other.m_pComparer = nullptr;
                }

                /// \brief Copy assignment operator.
                OrderedEnumerable& operator=(const OrderedEnumerable& other) {
                    if (this != &other) {
                        Enumerable<T>::operator=(other);
                        m_sourceItems = other.m_sourceItems;
                        delete m_pComparer;
                        m_pComparer = other.m_pComparer ? other.m_pComparer->Clone() : nullptr;
                    }
                    return *this;
                }

                /// \brief Move assignment operator.
                OrderedEnumerable& operator=(OrderedEnumerable&& other) noexcept {
                    if (this != &other) {
                        Enumerable<T>::operator=(std::move(other));
                        m_sourceItems = std::move(other.m_sourceItems);
                        delete m_pComparer;
                        m_pComparer = other.m_pComparer;
                        other.m_pComparer = nullptr;
                    }
                    return *this;
                }

                /// \brief Destructor releasing comparer chain.
                ~OrderedEnumerable() override {
                    delete m_pComparer;
                }

                /// \brief Performs a subsequent ordering in ascending order.
                /// \tparam FKey Key selector invocable type.
                /// \param keySelector Functor extracting the secondary key.
                /// \return An OrderedEnumerable whose elements are sorted according to both keys.
                template <typename FKey>
                OrderedEnumerable<T> ThenBy(FKey&& keySelector) const {
                    Internal::ChainedComparer<T, std::decay_t<FKey>> chained(m_pComparer, std::forward<FKey>(keySelector), false);
                    return OrderedEnumerable<T>(m_sourceItems, &chained);
                }

                /// \brief Performs a subsequent ordering in descending order.
                /// \tparam FKey Key selector invocable type.
                /// \param keySelector Functor extracting the secondary key.
                /// \return An OrderedEnumerable whose elements are sorted according to both keys descending.
                template <typename FKey>
                OrderedEnumerable<T> ThenByDescending(FKey&& keySelector) const {
                    Internal::ChainedComparer<T, std::decay_t<FKey>> chained(m_pComparer, std::forward<FKey>(keySelector), true);
                    return OrderedEnumerable<T>(m_sourceItems, &chained);
                }
            };

        } // namespace Linq
    } // namespace System
} // namespace DotNetDupe

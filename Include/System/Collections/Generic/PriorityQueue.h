/// \file PriorityQueue.h
/// \brief Represents a collection of items that have a value and a priority mirroring .NET PriorityQueue<TElement, TPriority>.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"
#include <functional>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \class PriorityQueue
                /// \brief Represents a collection of items with prioritized ordering backed by a binary min-heap.
                /// \tparam TElement Specifies the type of elements in the queue.
                /// \tparam TPriority Specifies the type of the priority associated with elements.
                /// 
                /// Enqueues items with associated priorities and dequeues elements in ascending priority order (lowest value first).
                /// \note Thread Safety: Public static members of this type are thread safe. Instance members are not guaranteed to be thread safe.
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename TElement, typename TPriority>
                class PriorityQueue : public Object {
                private:
                    struct ElementPriorityPair {
                        TElement Element;
                        TPriority Priority;
                        
                        bool operator>(const ElementPriorityPair& other) const {
                            return Priority > other.Priority;
                        }
                        
                        bool operator==(const ElementPriorityPair& other) const {
                            return false; // required for List<T> Contains
                        }
                    };

                    List<ElementPriorityPair> m_lstItems;

                    /// \brief Algorithm: Binary Min-Heap Sift-Up (O(log N))
                    /// Traverses upward from leaf to root, swapping the item with its parent
                    /// at (index - 1) / 2 while child priority < parent priority.
                    void SiftUp(int index) {
                        while (index > 0) {
                            int parent = (index - 1) / 2;
                            if (m_lstItems[parent] > m_lstItems[index]) {
                                std::swap(m_lstItems[parent], m_lstItems[index]);
                                index = parent;
                            } else {
                                break;
                            }
                        }
                    }

                    /// \brief Algorithm: Binary Min-Heap Sift-Down (O(log N))
                    /// Moves the top element down by iteratively comparing against the smaller
                    /// of its left (2*i + 1) and right (2*i + 2) children until heap order is restored.
                    void SiftDown(int index) {
                        int count = m_lstItems.GetCount();
                        while (index * 2 + 1 < count) {
                            int smallest = index * 2 + 1;
                            int right = index * 2 + 2;
                            if (right < count && m_lstItems[smallest] > m_lstItems[right]) {
                                smallest = right;
                            }
                            if (m_lstItems[index] > m_lstItems[smallest]) {
                                std::swap(m_lstItems[index], m_lstItems[smallest]);
                                index = smallest;
                            } else {
                                break;
                            }
                        }
                    }

                public:
                    /// \brief Initializes a new instance of the PriorityQueue class.
                    PriorityQueue() = default;

                    /// \brief Gets the number of elements contained in the PriorityQueue.
                    /// \return The number of elements contained in the PriorityQueue.
                    int GetCount() const { return m_lstItems.GetCount(); }

                    /// \brief Adds the specified element with associated priority to the PriorityQueue.
                    /// \param element The element to add to the PriorityQueue.
                    /// \param priority The priority with which to associate the element.
                    void Enqueue(const TElement& element, const TPriority& priority) {
                        m_lstItems.Add(ElementPriorityPair{ element, priority });
                        SiftUp(m_lstItems.GetCount() - 1);
                    }

                    /// \brief Removes and returns the minimal element from the PriorityQueue.
                    /// \return The minimal element that is removed from the PriorityQueue.
                    /// \throws System::InvalidOperationException The PriorityQueue is empty.
                    TElement Dequeue() {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        TElement item = std::move(m_lstItems[0].Element);
                        std::swap(m_lstItems[0], m_lstItems[m_lstItems.GetCount() - 1]);
                        m_lstItems.RemoveAt(m_lstItems.GetCount() - 1);
                        SiftDown(0);
                        return item;
                    }

                    /// \brief Returns the minimal element from the PriorityQueue without removing it.
                    /// \return The minimal element in the PriorityQueue.
                    /// \throws System::InvalidOperationException The PriorityQueue is empty.
                    TElement Peek() const {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        return m_lstItems[0].Element;
                    }

                    /// \brief Removes the minimal element and copies it and its priority to the specified out parameters.
                    /// \param element The removed element.
                    /// \param priority The priority of the removed element.
                    /// \return True if an element was removed; false if the PriorityQueue is empty.
                    bool TryDequeue(TElement& element, TPriority& priority) {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        element = std::move(m_lstItems[0].Element);
                        priority = std::move(m_lstItems[0].Priority);
                        std::swap(m_lstItems[0], m_lstItems[m_lstItems.GetCount() - 1]);
                        m_lstItems.RemoveAt(m_lstItems.GetCount() - 1);
                        SiftDown(0);
                        return true;
                    }

                    /// \brief Removes all items from the PriorityQueue.
                    void Clear() {
                        m_lstItems.Clear();
                    }
                };

            }
        }
    }
}
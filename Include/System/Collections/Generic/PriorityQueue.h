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
                    PriorityQueue() = default;

                    int GetCount() const { return m_lstItems.GetCount(); }

                    void Enqueue(const TElement& element, const TPriority& priority) {
                        m_lstItems.Add(ElementPriorityPair{ element, priority });
                        SiftUp(m_lstItems.GetCount() - 1);
                    }

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

                    TElement Peek() const {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        return m_lstItems[0].Element;
                    }

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

                    void Clear() {
                        m_lstItems.Clear();
                    }
                };

            }
        }
    }
}
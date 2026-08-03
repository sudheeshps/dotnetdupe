#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include <queue>
#include <vector>
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
                    };

                    std::priority_queue<ElementPriorityPair, std::vector<ElementPriorityPair>, std::greater<ElementPriorityPair>> m_pqMinHeap;

                public:
                    PriorityQueue() = default;

                    int GetCount() const { return (int)m_pqMinHeap.size(); }

                    void Enqueue(const TElement& element, const TPriority& priority) {
                        m_pqMinHeap.push({ element, priority });
                    }

                    TElement Dequeue() {
                        if (m_pqMinHeap.empty()) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        TElement item = m_pqMinHeap.top().Element;
                        m_pqMinHeap.pop();
                        return item;
                    }

                    TElement Peek() const {
                        if (m_pqMinHeap.empty()) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        return m_pqMinHeap.top().Element;
                    }

                    bool TryDequeue(TElement& element, TPriority& priority) {
                        if (m_pqMinHeap.empty()) {
                            return false;
                        }
                        element = m_pqMinHeap.top().Element;
                        priority = m_pqMinHeap.top().Priority;
                        m_pqMinHeap.pop();
                        return true;
                    }

                    void Clear() {
                        std::priority_queue<ElementPriorityPair, std::vector<ElementPriorityPair>, std::greater<ElementPriorityPair>> emptyPq;
                        std::swap(m_pqMinHeap, emptyPq);
                    }
                };

            }
        }
    }
}

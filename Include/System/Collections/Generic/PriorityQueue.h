#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
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

                    ElementPriorityPair* m_pItems;
                    int m_iCount;
                    int m_iCapacity;

                    void EnsureCapacity(int required) {
                        if (required <= m_iCapacity) return;
                        int newCap = m_iCapacity == 0 ? 4 : m_iCapacity * 2;
                        if (newCap < required) newCap = required;
                        
                        ElementPriorityPair* newItems = (ElementPriorityPair*)System::AllocateCollectionBuffer(newCap * sizeof(ElementPriorityPair));
                        for (int i = 0; i < m_iCount; ++i) {
                            new (&newItems[i]) ElementPriorityPair(std::move(m_pItems[i]));
                            m_pItems[i].~ElementPriorityPair();
                        }
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                        }
                        m_pItems = newItems;
                        m_iCapacity = newCap;
                    }

                    void SiftUp(int index) {
                        while (index > 0) {
                            int parent = (index - 1) / 2;
                            if (m_pItems[parent] > m_pItems[index]) {
                                std::swap(m_pItems[parent], m_pItems[index]);
                                index = parent;
                            } else {
                                break;
                            }
                        }
                    }

                    void SiftDown(int index) {
                        while (index * 2 + 1 < m_iCount) {
                            int smallest = index * 2 + 1;
                            int right = index * 2 + 2;
                            if (right < m_iCount && m_pItems[smallest] > m_pItems[right]) {
                                smallest = right;
                            }
                            if (m_pItems[index] > m_pItems[smallest]) {
                                std::swap(m_pItems[index], m_pItems[smallest]);
                                index = smallest;
                            } else {
                                break;
                            }
                        }
                    }

                public:
                    PriorityQueue() : m_pItems(nullptr), m_iCount(0), m_iCapacity(0) {}

                    ~PriorityQueue() override {
                        Clear();
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                            m_pItems = nullptr;
                        }
                    }

                    int GetCount() const { return m_iCount; }

                    void Enqueue(const TElement& element, const TPriority& priority) {
                        EnsureCapacity(m_iCount + 1);
                        new (&m_pItems[m_iCount]) ElementPriorityPair{ element, priority };
                        SiftUp(m_iCount);
                        m_iCount++;
                    }

                    TElement Dequeue() {
                        if (m_iCount == 0) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        TElement item = std::move(m_pItems[0].Element);
                        std::swap(m_pItems[0], m_pItems[m_iCount - 1]);
                        m_pItems[m_iCount - 1].~ElementPriorityPair();
                        m_iCount--;
                        SiftDown(0);
                        return item;
                    }

                    TElement Peek() const {
                        if (m_iCount == 0) {
                            throw System::InvalidOperationException("PriorityQueue is empty.");
                        }
                        return m_pItems[0].Element;
                    }

                    bool TryDequeue(TElement& element, TPriority& priority) {
                        if (m_iCount == 0) {
                            return false;
                        }
                        element = std::move(m_pItems[0].Element);
                        priority = std::move(m_pItems[0].Priority);
                        std::swap(m_pItems[0], m_pItems[m_iCount - 1]);
                        m_pItems[m_iCount - 1].~ElementPriorityPair();
                        m_iCount--;
                        SiftDown(0);
                        return true;
                    }

                    void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pItems[i].~ElementPriorityPair();
                        }
                        m_iCount = 0;
                    }
                };

            }
        }
    }
}
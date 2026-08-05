#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include <queue>
#include <stdexcept>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class Queue : public Object {
                private:
                    std::queue<T> m_qItems;

                public:
                    Queue() = default;

                    int GetCount() const { return (int)m_qItems.size(); }

                    void Enqueue(const T& item) {
                        m_qItems.push(item);
                    }

                    T Dequeue() {
                        if (m_qItems.empty()) {
                            throw System::InvalidOperationException("Queue is empty.");
                        }
                        T item = m_qItems.front();
                        m_qItems.pop();
                        return item;
                    }

                    T Peek() const {
                        if (m_qItems.empty()) {
                            throw System::InvalidOperationException("Queue is empty.");
                        }
                        return m_qItems.front();
                    }

                    bool TryDequeue(T& result) {
                        if (m_qItems.empty()) {
                            return false;
                        }
                        result = m_qItems.front();
                        m_qItems.pop();
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        if (m_qItems.empty()) {
                            return false;
                        }
                        result = m_qItems.front();
                        return true;
                    }

                    void Clear() {
                        std::queue<T> emptyQueue;
                        std::swap(m_qItems, emptyQueue);
                    }

                    bool Contains(const T& item) const {
                        std::queue<T> qCopy = m_qItems;
                        while (!qCopy.empty()) {
                            if (qCopy.front() == item) return true;
                            qCopy.pop();
                        }
                        return false;
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult((int)m_qItems.size());
                        std::queue<T> qCopy = m_qItems;
                        int iIndex = 0;
                        while (!qCopy.empty()) {
                            arrResult[iIndex++] = qCopy.front();
                            qCopy.pop();
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <queue>
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename T>
                class ConcurrentQueue : public Object {
                private:
                    mutable std::mutex m_mtxLock;
                    std::queue<T> m_qQueue;

                public:
                    ConcurrentQueue() = default;

                    void Enqueue(const T& item) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_qQueue.push(item);
                    }

                    bool TryDequeue(T& result) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_qQueue.empty()) {
                            return false;
                        }

                        result = m_qQueue.front();
                        m_qQueue.pop();
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_qQueue.empty()) {
                            return false;
                        }

                        result = m_qQueue.front();
                        return true;
                    }

                    void Clear() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        std::queue<T> emptyQueue;
                        std::swap(m_qQueue, emptyQueue);
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return (int)m_qQueue.size();
                    }

                    bool IsEmpty() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_qQueue.empty();
                    }

                    Array<T> ToArray() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        Array<T> arrResult((int)m_qQueue.size());
                        std::queue<T> qTemp = m_qQueue;
                        int iIndex = 0;

                        while (!qTemp.empty()) {
                            arrResult[iIndex++] = qTemp.front();
                            qTemp.pop();
                        }

                        return arrResult;
                    }
                };

            }
        }
    }
}

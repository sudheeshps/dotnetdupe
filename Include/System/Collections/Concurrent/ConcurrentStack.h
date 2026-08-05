#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <stack>
#include <mutex>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename T>
                class ConcurrentStack : public Object {
                private:
                    mutable std::mutex m_mtxLock;
                    std::stack<T> m_stkStack;

                public:
                    ConcurrentStack() = default;

                    void Push(const T& item) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        m_stkStack.push(item);
                    }

                    bool TryPop(T& result) {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_stkStack.empty()) {
                            return false;
                        }

                        result = m_stkStack.top();
                        m_stkStack.pop();
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        if (m_stkStack.empty()) {
                            return false;
                        }

                        result = m_stkStack.top();
                        return true;
                    }

                    void Clear() {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        std::stack<T> emptyStack;
                        std::swap(m_stkStack, emptyStack);
                    }

                    int GetCount() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return (int)m_stkStack.size();
                    }

                    bool IsEmpty() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        return m_stkStack.empty();
                    }

                    Array<T> ToArray() const {
                        std::lock_guard<std::mutex> lock(m_mtxLock);
                        
                        Array<T> arrResult((int)m_stkStack.size());
                        std::stack<T> stkTemp = m_stkStack;
                        int iIndex = 0;

                        while (!stkTemp.empty()) {
                            arrResult[iIndex++] = stkTemp.top();
                            stkTemp.pop();
                        }

                        return arrResult;
                    }
                };

            }
        }
    }
}

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include <stack>
#include <stdexcept>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class Stack : public Object {
                private:
                    std::stack<T> m_stkItems;

                public:
                    Stack() = default;

                    int GetCount() const { return (int)m_stkItems.size(); }

                    void Push(const T& item) {
                        m_stkItems.push(item);
                    }

                    T Pop() {
                        if (m_stkItems.empty()) {
                            throw System::InvalidOperationException("Stack is empty.");
                        }
                        T item = m_stkItems.top();
                        m_stkItems.pop();
                        return item;
                    }

                    T Peek() const {
                        if (m_stkItems.empty()) {
                            throw System::InvalidOperationException("Stack is empty.");
                        }
                        return m_stkItems.top();
                    }

                    bool TryPop(T& result) {
                        if (m_stkItems.empty()) {
                            return false;
                        }
                        result = m_stkItems.top();
                        m_stkItems.pop();
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        if (m_stkItems.empty()) {
                            return false;
                        }
                        result = m_stkItems.top();
                        return true;
                    }

                    void Clear() {
                        std::stack<T> emptyStack;
                        std::swap(m_stkItems, emptyStack);
                    }

                    bool Contains(const T& item) const {
                        std::stack<T> stkCopy = m_stkItems;
                        while (!stkCopy.empty()) {
                            if (stkCopy.top() == item) return true;
                            stkCopy.pop();
                        }
                        return false;
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult((int)m_stkItems.size());
                        std::stack<T> stkCopy = m_stkItems;
                        int iIndex = 0;
                        while (!stkCopy.empty()) {
                            arrResult[iIndex++] = stkCopy.top();
                            stkCopy.pop();
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/InvalidOperationException.h"
#include "System/Collections/Generic/List.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class Stack : public Object {
                private:
                    List<T> m_lstItems;

                public:
                    Stack() = default;

                    int GetCount() const { return m_lstItems.GetCount(); }

                    void Push(const T& item) {
                        m_lstItems.Add(item);
                    }

                    T Pop() {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Stack is empty.");
                        }
                        int lastIndex = m_lstItems.GetCount() - 1;
                        T item = m_lstItems[lastIndex];
                        m_lstItems.RemoveAt(lastIndex);
                        return item;
                    }

                    T Peek() const {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Stack is empty.");
                        }
                        return m_lstItems[m_lstItems.GetCount() - 1];
                    }

                    bool TryPop(T& result) {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        int lastIndex = m_lstItems.GetCount() - 1;
                        result = m_lstItems[lastIndex];
                        m_lstItems.RemoveAt(lastIndex);
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        result = m_lstItems[m_lstItems.GetCount() - 1];
                        return true;
                    }

                    void Clear() {
                        m_lstItems.Clear();
                    }

                    bool Contains(const T& item) const {
                        return m_lstItems.Contains(item);
                    }

                    Array<T> ToArray() const {
                        int count = m_lstItems.GetCount();
                        Array<T> arrResult(count);
                        for (int i = 0; i < count; ++i) {
                            arrResult[i] = m_lstItems[count - 1 - i];
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}

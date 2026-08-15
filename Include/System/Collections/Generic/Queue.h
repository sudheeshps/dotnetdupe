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
                class Queue : public Object {
                private:
                    List<T> m_lstItems;

                public:
                    Queue() = default;

                    int GetCount() const { return m_lstItems.GetCount(); }

                    void Enqueue(const T& item) {
                        m_lstItems.Add(item);
                    }

                    T Dequeue() {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Queue is empty.");
                        }
                        T item = m_lstItems[0];
                        m_lstItems.RemoveAt(0);
                        return item;
                    }

                    T Peek() const {
                        if (m_lstItems.GetCount() == 0) {
                            throw System::InvalidOperationException("Queue is empty.");
                        }
                        return m_lstItems[0];
                    }

                    bool TryDequeue(T& result) {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        result = m_lstItems[0];
                        m_lstItems.RemoveAt(0);
                        return true;
                    }

                    bool TryPeek(T& result) const {
                        if (m_lstItems.GetCount() == 0) {
                            return false;
                        }
                        result = m_lstItems[0];
                        return true;
                    }

                    void Clear() {
                        m_lstItems.Clear();
                    }

                    bool Contains(const T& item) const {
                        return m_lstItems.Contains(item);
                    }

                    Array<T> ToArray() const {
                        return m_lstItems.ToArray();
                    }
                };

            }
        }
    }
}

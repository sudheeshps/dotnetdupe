#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/List.h"
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class SortedSet : public Object {
                private:
                    List<T> m_lstItems;

                public:
                    SortedSet() = default;

                    int GetCount() const { return m_lstItems.GetCount(); }

                    bool Add(const T& item) {
                        int index = m_lstItems.BinarySearch(item);
                        if (index >= 0) return false;

                        m_lstItems.Insert(~index, item);
                        return true;
                    }

                    bool Remove(const T& item) {
                        int index = m_lstItems.BinarySearch(item);
                        if (index < 0) return false;

                        m_lstItems.RemoveAt(index);
                        return true;
                    }

                    bool Contains(const T& item) const {
                        return m_lstItems.BinarySearch(item) >= 0;
                    }

                    void Clear() {
                        m_lstItems.Clear();
                    }

                    void UnionWith(const SortedSet<T>& other) {
                        for (int i = 0; i < other.m_lstItems.GetCount(); ++i) {
                            Add(other.m_lstItems[i]);
                        }
                    }

                    void IntersectWith(const SortedSet<T>& other) {
                        for (int i = m_lstItems.GetCount() - 1; i >= 0; --i) {
                            if (!other.Contains(m_lstItems[i])) {
                                Remove(m_lstItems[i]);
                            }
                        }
                    }

                    void ExceptWith(const SortedSet<T>& other) {
                        for (int i = 0; i < other.m_lstItems.GetCount(); ++i) {
                            Remove(other.m_lstItems[i]);
                        }
                    }

                    Array<T> ToArray() const {
                        return m_lstItems.ToArray();
                    }
                };

            }
        }
    }
}
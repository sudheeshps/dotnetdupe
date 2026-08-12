#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class SortedSet : public Object {
                private:
                    T* m_pItems;
                    int m_iCount;
                    int m_iCapacity;

                    void EnsureCapacity(int required) {
                        if (required <= m_iCapacity) return;
                        int newCap = m_iCapacity == 0 ? 4 : m_iCapacity * 2;
                        if (newCap < required) newCap = required;
                        
                        T* newItems = (T*)System::AllocateCollectionBuffer(newCap * sizeof(T));
                        for (int i = 0; i < m_iCount; ++i) {
                            new (&newItems[i]) T(std::move(m_pItems[i]));
                            m_pItems[i].~T();
                        }
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                        }
                        m_pItems = newItems;
                        m_iCapacity = newCap;
                    }

                    int BinarySearch(const T& item, bool& found) const {
                        int low = 0;
                        int high = m_iCount - 1;
                        while (low <= high) {
                            int mid = low + (high - low) / 2;
                            if (m_pItems[mid] == item) {
                                found = true;
                                return mid;
                            }
                            if (m_pItems[mid] < item) {
                                low = mid + 1;
                            } else {
                                high = mid - 1;
                            }
                        }
                        found = false;
                        return low;
                    }

                public:
                    SortedSet() : m_pItems(nullptr), m_iCount(0), m_iCapacity(0) {}

                    ~SortedSet() override {
                        Clear();
                        if (m_pItems) {
                            System::FreeCollectionBuffer(m_pItems);
                            m_pItems = nullptr;
                        }
                    }

                    int GetCount() const { return m_iCount; }

                    bool Add(const T& item) {
                        bool found = false;
                        int index = BinarySearch(item, found);
                        if (found) return false;

                        EnsureCapacity(m_iCount + 1);
                        for (int i = m_iCount; i > index; --i) {
                            new (&m_pItems[i]) T(std::move(m_pItems[i - 1]));
                            m_pItems[i - 1].~T();
                        }
                        new (&m_pItems[index]) T(item);
                        m_iCount++;
                        return true;
                    }

                    bool Remove(const T& item) {
                        bool found = false;
                        int index = BinarySearch(item, found);
                        if (!found) return false;

                        m_pItems[index].~T();
                        for (int i = index; i < m_iCount - 1; ++i) {
                            new (&m_pItems[i]) T(std::move(m_pItems[i + 1]));
                            m_pItems[i + 1].~T();
                        }
                        m_iCount--;
                        return true;
                    }

                    bool Contains(const T& item) const {
                        bool found = false;
                        BinarySearch(item, found);
                        return found;
                    }

                    void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pItems[i].~T();
                        }
                        m_iCount = 0;
                    }

                    void UnionWith(const SortedSet<T>& other) {
                        for (int i = 0; i < other.m_iCount; ++i) {
                            Add(other.m_pItems[i]);
                        }
                    }

                    void IntersectWith(const SortedSet<T>& other) {
                        for (int i = m_iCount - 1; i >= 0; --i) {
                            if (!other.Contains(m_pItems[i])) {
                                Remove(m_pItems[i]);
                            }
                        }
                    }

                    void ExceptWith(const SortedSet<T>& other) {
                        for (int i = 0; i < other.m_iCount; ++i) {
                            Remove(other.m_pItems[i]);
                        }
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult(m_iCount);
                        for (int i = 0; i < m_iCount; ++i) {
                            arrResult[i] = m_pItems[i];
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}
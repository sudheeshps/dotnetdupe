#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <new>
#include <initializer_list>
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class HashSet : public Object {
                private:
                    T* m_pData;
                    int m_iCount;
                    int m_iCapacity;

                    void EnsureCapacity(int minCapacity) {
                        if (m_iCapacity >= minCapacity) return;
                        int newCapacity = m_iCapacity == 0 ? 4 : m_iCapacity * 2;
                        if (newCapacity < minCapacity) newCapacity = minCapacity;

                        T* pNewData = (T*)AllocateCollectionBuffer(newCapacity * sizeof(T));
                        for (int i = 0; i < m_iCount; ++i) {
                            ::new ((void*)&pNewData[i]) T(std::move(m_pData[i]));
                            m_pData[i].~T();
                        }
                        if (m_pData) {
                            FreeCollectionBuffer(m_pData);
                        }
                        m_pData = pNewData;
                        m_iCapacity = newCapacity;
                    }

                public:
                    HashSet() : m_pData(nullptr), m_iCount(0), m_iCapacity(0) {}

                    HashSet(const HashSet& other) : m_pData(nullptr), m_iCount(0), m_iCapacity(0) {
                        EnsureCapacity(other.m_iCount);
                        for (int i = 0; i < other.m_iCount; ++i) {
                            new (&m_pData[i]) T(other.m_pData[i]);
                            m_iCount++;
                        }
                    }

                    HashSet(HashSet&& other) noexcept : m_pData(other.m_pData), m_iCount(other.m_iCount), m_iCapacity(other.m_iCapacity) {
                        other.m_pData = nullptr;
                        other.m_iCount = 0;
                        other.m_iCapacity = 0;
                    }

                    HashSet& operator=(const HashSet& other) {
                        if (this != &other) {
                            Clear();
                            EnsureCapacity(other.m_iCount);
                            for (int i = 0; i < other.m_iCount; ++i) {
                                new (&m_pData[i]) T(other.m_pData[i]);
                                m_iCount++;
                            }
                        }
                        return *this;
                    }

                    HashSet& operator=(HashSet&& other) noexcept {
                        if (this != &other) {
                            Clear();
                            if (m_pData) FreeCollectionBuffer(m_pData);
                            m_pData = other.m_pData;
                            m_iCount = other.m_iCount;
                            m_iCapacity = other.m_iCapacity;
                            other.m_pData = nullptr;
                            other.m_iCount = 0;
                            other.m_iCapacity = 0;
                        }
                        return *this;
                    }

                    ~HashSet() {
                        Clear();
                        if (m_pData) {
                            FreeCollectionBuffer(m_pData);
                            m_pData = nullptr;
                        }
                    }

                    int GetCount() const { return m_iCount; }

                    bool Add(const T& item) {
                        if (Contains(item)) return false;
                        EnsureCapacity(m_iCount + 1);
                        ::new ((void*)&m_pData[m_iCount]) T(item);
                        m_iCount++;
                        return true;
                    }

                    bool Remove(const T& item) {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pData[i] == item) {
                                m_pData[i].~T();
                                for (int j = i; j < m_iCount - 1; ++j) {
                                    ::new ((void*)&m_pData[j]) T(std::move(m_pData[j + 1]));
                                    m_pData[j + 1].~T();
                                }
                                m_iCount--;
                                return true;
                            }
                        }
                        return false;
                    }

                    bool Contains(const T& item) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pData[i] == item) return true;
                        }
                        return false;
                    }

                    void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pData[i].~T();
                        }
                        m_iCount = 0;
                    }

                    void UnionWith(const HashSet<T>& other) {
                        for (int i = 0; i < other.m_iCount; ++i) {
                            Add(other.m_pData[i]);
                        }
                    }

                    void IntersectWith(const HashSet<T>& other) {
                        for (int i = m_iCount - 1; i >= 0; --i) {
                            if (!other.Contains(m_pData[i])) {
                                Remove(m_pData[i]);
                            }
                        }
                    }

                    void ExceptWith(const HashSet<T>& other) {
                        for (int i = 0; i < other.m_iCount; ++i) {
                            Remove(other.m_pData[i]);
                        }
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult(m_iCount);
                        for (int i = 0; i < m_iCount; ++i) {
                            arrResult[i] = m_pData[i];
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}

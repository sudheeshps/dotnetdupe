#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class List : public Object {
                public:
                    List() {}
                    List(int iCapacity) { m_vItems.reserve(iCapacity); }
                    List(const std::initializer_list<T>& vCollection) : m_vItems(vCollection) {}

                    // Copy semantics
                    List(const List& lstOther) = default;
                    List& operator=(const List& lstOther) = default;

                    // Move semantics
                    List(List&& lstOther) noexcept : m_vItems(std::move(lstOther.m_vItems)) {}
                    List& operator=(List&& lstOther) noexcept {
                        if (this != &lstOther) {
                            m_vItems = std::move(lstOther.m_vItems);
                        }
                        return *this;
                    }

                    int GetCount() const { return (int)m_vItems.size(); }
                    int GetCapacity() const { return (int)m_vItems.capacity(); }
                    void SetCapacity(int iValue) { m_vItems.reserve(iValue); }

                    T& operator[](int iIndex) {
                        return m_vItems.at(iIndex);
                    }

                    const T& operator[](int iIndex) const {
                        return m_vItems.at(iIndex);
                    }

                    void Add(const T& item) {
                        m_vItems.push_back(item);
                    }

                    void AddRange(const Array<T>& arrCollection) {
                        for (int iIdx = 0; iIdx < arrCollection.GetLength(); iIdx++) {
                            m_vItems.push_back(arrCollection[iIdx]);
                        }
                    }

                    void Clear() {
                        m_vItems.clear();
                    }

                    bool Contains(const T& item) const {
                        return std::find(m_vItems.begin(), m_vItems.end(), item) != m_vItems.end();
                    }

                    int IndexOf(const T& item) const {
                        auto it = std::find(m_vItems.begin(), m_vItems.end(), item);
                        if (it == m_vItems.end()) return -1;
                        return (int)std::distance(m_vItems.begin(), it);
                    }

                    void Insert(int iIndex, const T& item) {
                        m_vItems.insert(m_vItems.begin() + iIndex, item);
                    }

                    bool Remove(const T& item) {
                        auto it = std::find(m_vItems.begin(), m_vItems.end(), item);
                        if (it != m_vItems.end()) {
                            m_vItems.erase(it);
                            return true;
                        }
                        return false;
                    }

                    void RemoveAt(int iIndex) {
                        m_vItems.erase(m_vItems.begin() + iIndex);
                    }

                    void Sort() {
                        std::sort(m_vItems.begin(), m_vItems.end());
                    }

                    template <typename Predicate>
                    bool Exists(Predicate fnMatch) const {
                        return std::find_if(m_vItems.begin(), m_vItems.end(), fnMatch) != m_vItems.end();
                    }

                    template <typename Predicate>
                    T Find(Predicate fnMatch) const {
                        auto it = std::find_if(m_vItems.begin(), m_vItems.end(), fnMatch);
                        if (it == m_vItems.end()) return T();
                        return *it;
                    }

                    template <typename Predicate>
                    List<T> FindAll(Predicate fnMatch) const {
                        List<T> lstResult;
                        for (const auto& item : m_vItems) {
                            if (fnMatch(item)) lstResult.Add(item);
                        }
                        return lstResult;
                    }

                    template <typename Predicate>
                    bool TrueForAll(Predicate fnMatch) const {
                        return std::all_of(m_vItems.begin(), m_vItems.end(), fnMatch);
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult((int)m_vItems.size());
                        for (int iIdx = 0; iIdx < (int)m_vItems.size(); iIdx++) {
                            arrResult[iIdx] = m_vItems[iIdx];
                        }
                        return arrResult;
                    }

                    // Iterator support for range-based for loops
                    auto begin() { return m_vItems.begin(); }
                    auto end() { return m_vItems.end(); }
                    auto begin() const { return m_vItems.begin(); }
                    auto end() const { return m_vItems.end(); }

                private:
                    std::vector<T> m_vItems;
                };
            }
        }
    }
}

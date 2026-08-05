#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <set>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class SortedSet : public Object {
                private:
                    std::set<T> m_sSet;

                public:
                    SortedSet() = default;

                    int GetCount() const { return (int)m_sSet.size(); }

                    bool Add(const T& item) {
                        return m_sSet.insert(item).second;
                    }

                    bool Remove(const T& item) {
                        return m_sSet.erase(item) > 0;
                    }

                    bool Contains(const T& item) const {
                        return m_sSet.find(item) != m_sSet.end();
                    }

                    void Clear() {
                        m_sSet.clear();
                    }

                    void UnionWith(const SortedSet<T>& other) {
                        for (const auto& item : other.m_sSet) {
                            m_sSet.insert(item);
                        }
                    }

                    void IntersectWith(const SortedSet<T>& other) {
                        std::set<T> sNewSet;
                        for (const auto& item : m_sSet) {
                            if (other.m_sSet.find(item) != other.m_sSet.end()) {
                                sNewSet.insert(item);
                            }
                        }
                        m_sSet = std::move(sNewSet);
                    }

                    void ExceptWith(const SortedSet<T>& other) {
                        for (const auto& item : other.m_sSet) {
                            m_sSet.erase(item);
                        }
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult((int)m_sSet.size());
                        int iIndex = 0;
                        for (const auto& item : m_sSet) {
                            arrResult[iIndex++] = item;
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}

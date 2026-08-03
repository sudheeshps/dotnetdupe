#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <unordered_set>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class HashSet : public Object {
                private:
                    std::unordered_set<T> m_uSet;

                public:
                    HashSet() = default;

                    int GetCount() const { return (int)m_uSet.size(); }

                    bool Add(const T& item) {
                        return m_uSet.insert(item).second;
                    }

                    bool Remove(const T& item) {
                        return m_uSet.erase(item) > 0;
                    }

                    bool Contains(const T& item) const {
                        return m_uSet.find(item) != m_uSet.end();
                    }

                    void Clear() {
                        m_uSet.clear();
                    }

                    void UnionWith(const HashSet<T>& other) {
                        for (const auto& item : other.m_uSet) {
                            m_uSet.insert(item);
                        }
                    }

                    void IntersectWith(const HashSet<T>& other) {
                        std::unordered_set<T> uNewSet;
                        for (const auto& item : m_uSet) {
                            if (other.m_uSet.find(item) != other.m_uSet.end()) {
                                uNewSet.insert(item);
                            }
                        }
                        m_uSet = std::move(uNewSet);
                    }

                    void ExceptWith(const HashSet<T>& other) {
                        for (const auto& item : other.m_uSet) {
                            m_uSet.erase(item);
                        }
                    }

                    Array<T> ToArray() const {
                        Array<T> arrResult((int)m_uSet.size());
                        int iIndex = 0;
                        for (const auto& item : m_uSet) {
                            arrResult[iIndex++] = item;
                        }
                        return arrResult;
                    }
                };

            }
        }
    }
}

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/Collections/Generic/Dictionary.h"
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
                    Dictionary<T, bool> m_dict;

                public:
                    HashSet() {}

                    HashSet(const HashSet& other) : m_dict(other.m_dict) {}

                    HashSet(HashSet&& other) noexcept : m_dict(std::move(other.m_dict)) {}

                    HashSet& operator=(const HashSet& other) {
                        if (this != &other) {
                            m_dict = other.m_dict;
                        }
                        return *this;
                    }

                    HashSet& operator=(HashSet&& other) noexcept {
                        if (this != &other) {
                            m_dict = std::move(other.m_dict);
                        }
                        return *this;
                    }

                    ~HashSet() override {}

                    int GetCount() const { return m_dict.GetCount(); }

                    bool Add(const T& item) {
                        if (m_dict.ContainsKey(item)) return false;
                        m_dict.Add(item, true);
                        return true;
                    }

                    bool Remove(const T& item) {
                        return m_dict.Remove(item);
                    }

                    bool Contains(const T& item) const {
                        return m_dict.ContainsKey(item);
                    }

                    void Clear() {
                        m_dict.Clear();
                    }

                    void UnionWith(const HashSet<T>& other) {
                        Array<T> keys = other.m_dict.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            Add(keys[i]);
                        }
                    }

                    void IntersectWith(const HashSet<T>& other) {
                        Array<T> keys = m_dict.GetKeys();
                        for (int i = keys.GetLength() - 1; i >= 0; --i) {
                            if (!other.Contains(keys[i])) {
                                Remove(keys[i]);
                            }
                        }
                    }

                    void ExceptWith(const HashSet<T>& other) {
                        Array<T> keys = other.m_dict.GetKeys();
                        for (int i = 0; i < keys.GetLength(); ++i) {
                            Remove(keys[i]);
                        }
                    }

                    Array<T> ToArray() const {
                        return m_dict.GetKeys();
                    }
                };

            }
        }
    }
}

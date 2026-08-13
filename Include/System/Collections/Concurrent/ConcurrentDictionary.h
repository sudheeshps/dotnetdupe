#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include "System/Collections/Generic/Dictionary.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Lock.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Concurrent {

                template <typename TKey, typename TValue>
                class ConcurrentDictionary : public Object {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::Dictionary<TKey, TValue> m_dict;

                public:
                    ConcurrentDictionary() = default;

                    ~ConcurrentDictionary() override {
                        Clear();
                    }

                    bool TryAdd(const TKey& key, const TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.ContainsKey(key)) {
                            return false;
                        }
                        m_dict.Add(key, value);
                        return true;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.TryGetValue(key, value);
                    }

                    bool TryRemove(const TKey& key, TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.TryGetValue(key, value)) {
                            m_dict.Remove(key);
                            return true;
                        }
                        return false;
                    }

                    bool ContainsKey(const TKey& key) const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.ContainsKey(key);
                    }

                    void Clear() {
                        Threading::CriticalSectionLock lock(m_csLock);
                        m_dict.Clear();
                    }

                    int GetCount() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetCount();
                    }

                    bool IsEmpty() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetCount() == 0;
                    }

                    TValue GetOrAdd(const TKey& key, const TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        TValue existingVal;
                        if (m_dict.TryGetValue(key, existingVal)) {
                            return existingVal;
                        }
                        m_dict.Add(key, value);
                        return value;
                    }

                    template <typename F>
                    TValue GetOrAdd(const TKey& key, F valueFactory) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        TValue existingVal;
                        if (m_dict.TryGetValue(key, existingVal)) {
                            return existingVal;
                        }
                        TValue val = valueFactory(key);
                        m_dict.Add(key, val);
                        return val;
                    }

                    TValue AddOrUpdate(const TKey& key, const TValue& addValue, const TValue& updateValue) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.ContainsKey(key)) {
                            m_dict[key] = updateValue;
                            return updateValue;
                        }
                        m_dict.Add(key, addValue);
                        return addValue;
                    }

                    TValue& operator[](const TKey& key) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        // Be careful returning a reference while lock is released.
                        // The user must synchronize external accesses to this reference.
                        return m_dict[key];
                    }

                    Array<TKey> GetKeys() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetKeys();
                    }

                    Array<TValue> GetValues() const {
                        Threading::CriticalSectionLock lock(m_csLock);
                        return m_dict.GetValues();
                    }
                };

            }
        }
    }
}
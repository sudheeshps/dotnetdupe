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

                /// \class ConcurrentDictionary
                /// \brief Represents a thread-safe collection of key/value pairs that can be accessed by multiple threads concurrently.
                ///
                /// \tparam TKey The type of the keys in the dictionary.
                /// \tparam TValue The type of the values in the dictionary.
                /// \note Conforms to ECMA-335 Partition IV and .NET Concurrent Collections architecture.
                ///       Synchronizes operations using CriticalSection mutual exclusion to guarantee atomic modifications.
                template <typename TKey, typename TValue>
                class ConcurrentDictionary : public Object {
                private:
                    mutable Threading::CriticalSection m_csLock;
                    Generic::Dictionary<TKey, TValue> m_dict;

                public:
                    /// \brief Initializes a new instance of the ConcurrentDictionary class that is empty.
                    ConcurrentDictionary() = default;

                    /// \brief Destructor. Clears dictionary contents.
                    ~ConcurrentDictionary() override {
                        Clear();
                    }

                    /// \brief Attempts to add the specified key and value to the ConcurrentDictionary.
                    /// \param key The key of the element to add.
                    /// \param value The value of the element to add.
                    /// \return true if the key/value pair was added to the ConcurrentDictionary successfully; false if the key already exists.
                    bool TryAdd(const TKey& key, const TValue& value) {
                        Threading::CriticalSectionLock lock(m_csLock);
                        if (m_dict.ContainsKey(key)) {
                            return false;
                        }
                        m_dict.Add(key, value);
                        return true;
                    }

                    /// \brief Attempts to get the value associated with the specified key from the ConcurrentDictionary.
                    /// \param key The key of the value to get.
                    /// \param value Output parameter receiving the value associated with key, if found.
                    /// \return true if the key was found in the ConcurrentDictionary; otherwise, false.
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
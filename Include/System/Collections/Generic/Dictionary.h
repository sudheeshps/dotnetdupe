#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include "System/ArgumentException.h"
#include <new>
#include <utility>

namespace DotNetDupe {
	namespace System {
		namespace Collections {
			namespace Generic {

				template <typename TKey, typename TValue>
				struct KeyValuePair {
					TKey Key;
					TValue Value;
					KeyValuePair(TKey k, TValue v) : Key(k), Value(v) { }
				};

				template <typename TKey, typename TValue>
				class Dictionary : public Object {
				public:
					Dictionary() { }
                    ~Dictionary() override {
                        FreeBuffer();
                    }

                    // Copy semantics
                    Dictionary(const Dictionary& other) {
                        SetCapacity(other.m_iCapacity);
                        for (int i = 0; i < other.m_iCount; ++i) {
                            ::new ((void*)&m_pData[i]) KeyValuePair<TKey, TValue>(other.m_pData[i]);
                        }
                        m_iCount = other.m_iCount;
                    }
                    Dictionary& operator=(const Dictionary& other) {
                        if (this != &other) {
                            Clear();
                            SetCapacity(other.m_iCapacity);
                            for (int i = 0; i < other.m_iCount; ++i) {
                                ::new ((void*)&m_pData[i]) KeyValuePair<TKey, TValue>(other.m_pData[i]);
                            }
                            m_iCount = other.m_iCount;
                        }
                        return *this;
                    }

                    // Move semantics
                    Dictionary(Dictionary&& other) noexcept : m_pData(other.m_pData), m_iCount(other.m_iCount), m_iCapacity(other.m_iCapacity) {
                        other.m_pData = nullptr;
                        other.m_iCount = 0;
                        other.m_iCapacity = 0;
                    }
                    Dictionary& operator=(Dictionary&& other) noexcept {
                        if (this != &other) {
                            FreeBuffer();
                            m_pData = other.m_pData;
                            m_iCount = other.m_iCount;
                            m_iCapacity = other.m_iCapacity;
                            other.m_pData = nullptr;
                            other.m_iCount = 0;
                            other.m_iCapacity = 0;
                        }
                        return *this;
                    }

					int GetCount() const { return m_iCount; }

					TValue& operator[](const TKey& key) {
                        int idx = IndexOfKey(key);
                        if (idx != -1) {
                            return m_pData[idx].Value;
                        }
                        
                        // Add new
                        if (m_iCount == m_iCapacity) {
                            SetCapacity(m_iCapacity == 0 ? 4 : m_iCapacity * 2);
                        }
                        ::new ((void*)&m_pData[m_iCount]) KeyValuePair<TKey, TValue>(key, TValue());
                        return m_pData[m_iCount++].Value;
					}

					const TValue& operator[](const TKey& key) const {
                        int idx = IndexOfKey(key);
                        if (idx != -1) {
                            return m_pData[idx].Value;
                        }
                        throw System::ArgumentException("Key not found.");
					}

					void Add(const TKey& key, const TValue& value) {
						if (ContainsKey(key)) throw System::ArgumentException("An item with the same key has already been added.");
						if (m_iCount == m_iCapacity) {
                            SetCapacity(m_iCapacity == 0 ? 4 : m_iCapacity * 2);
                        }
                        ::new ((void*)&m_pData[m_iCount]) KeyValuePair<TKey, TValue>(key, value);
                        m_iCount++;
					}

					void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pData[i].~KeyValuePair<TKey, TValue>();
                        }
                        m_iCount = 0;
					}

					bool ContainsKey(const TKey& key) const {
						return IndexOfKey(key) != -1;
					}

					bool Remove(const TKey& key) {
                        int idx = IndexOfKey(key);
                        if (idx != -1) {
                            for (int i = idx; i < m_iCount - 1; ++i) {
                                m_pData[i] = std::move(m_pData[i + 1]);
                            }
                            m_pData[m_iCount - 1].~KeyValuePair<TKey, TValue>();
                            m_iCount--;
                            return true;
                        }
						return false;
					}

					bool TryGetValue(const TKey& key, TValue& value) const {
                        int idx = IndexOfKey(key);
                        if (idx != -1) {
                            value = m_pData[idx].Value;
                            return true;
                        }
						return false;
					}

					Array<TKey> GetKeys() const {
						Array<TKey> arrKeys(GetCount());
						for (int i = 0; i < m_iCount; ++i) {
                            arrKeys[i] = m_pData[i].Key;
                        }
						return arrKeys;
					}

					Array<TValue> GetValues() const {
						Array<TValue> arrValues(GetCount());
						for (int i = 0; i < m_iCount; ++i) {
                            arrValues[i] = m_pData[i].Value;
                        }
						return arrValues;
					}

					// Iterator support for range-based for loops
					KeyValuePair<TKey, TValue>* begin() { return m_pData; }
					KeyValuePair<TKey, TValue>* end() { return m_pData + m_iCount; }
					const KeyValuePair<TKey, TValue>* begin() const { return m_pData; }
					const KeyValuePair<TKey, TValue>* end() const { return m_pData + m_iCount; }

				private:
                    KeyValuePair<TKey, TValue>* m_pData = nullptr;
                    int m_iCount = 0;
                    int m_iCapacity = 0;

                    int IndexOfKey(const TKey& key) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pData[i].Key == key) return i;
                        }
                        return -1;
                    }

                    void SetCapacity(int iCapacity) {
                        if (iCapacity > m_iCapacity) {
                            KeyValuePair<TKey, TValue>* pNewData = static_cast<KeyValuePair<TKey, TValue>*>(AllocateCollectionBuffer(sizeof(KeyValuePair<TKey, TValue>) * iCapacity));
                            for (int i = 0; i < m_iCount; ++i) {
                                ::new ((void*)&pNewData[i]) KeyValuePair<TKey, TValue>(std::move(m_pData[i]));
                            }
                            FreeBuffer();
                            m_pData = pNewData;
                            m_iCapacity = iCapacity;
                        }
                    }

                    void FreeBuffer() {
                        if (m_pData) {
                            for (int i = 0; i < m_iCount; ++i) {
                                m_pData[i].~KeyValuePair<TKey, TValue>();
                            }
                            FreeCollectionBuffer(m_pData);
                            m_pData = nullptr;
                        }
                    }
				};
			}
		}
	}
}

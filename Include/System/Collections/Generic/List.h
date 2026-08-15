#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <new>
#include <utility>
#include <initializer_list>

namespace DotNetDupe {
	namespace System {
		namespace Collections {
			namespace Generic {

				template <typename T>
				class List : public Object {
				public:
					List() { }
					List(int iCapacity) { SetCapacity(iCapacity); }
					List(const std::initializer_list<T>& vCollection) {
                        SetCapacity((int)vCollection.size());
                        for (const auto& item : vCollection) {
                            Add(item);
                        }
                    }

					// Copy semantics
					List(const List& lstOther) {
                        SetCapacity(lstOther.m_iCapacity);
                        for (int i = 0; i < lstOther.m_iCount; ++i) {
                            Add(lstOther.m_pData[i]);
                        }
                    }
					List& operator=(const List& lstOther) {
                        if (this != &lstOther) {
                            Clear();
                            SetCapacity(lstOther.m_iCapacity);
                            for (int i = 0; i < lstOther.m_iCount; ++i) {
                                Add(lstOther.m_pData[i]);
                            }
                        }
                        return *this;
                    }

					// Move semantics
					List(List&& lstOther) noexcept : m_pData(lstOther.m_pData), m_iCount(lstOther.m_iCount), m_iCapacity(lstOther.m_iCapacity) {
                        lstOther.m_pData = nullptr;
                        lstOther.m_iCount = 0;
                        lstOther.m_iCapacity = 0;
                    }
					List& operator=(List&& lstOther) noexcept {
						if (this != &lstOther) {
                            FreeBuffer();
							m_pData = lstOther.m_pData;
                            m_iCount = lstOther.m_iCount;
                            m_iCapacity = lstOther.m_iCapacity;
                            lstOther.m_pData = nullptr;
                            lstOther.m_iCount = 0;
                            lstOther.m_iCapacity = 0;
						}
						return *this;
					}

                    ~List() override {
                        FreeBuffer();
                    }

					int GetCount() const { return m_iCount; }
					int GetCapacity() const { return m_iCapacity; }
					
                    void SetCapacity(int iValue) {
                        if (iValue > m_iCapacity) {
                            T* pNewData = static_cast<T*>(AllocateCollectionBuffer(sizeof(T) * iValue));
                            for (int i = 0; i < m_iCount; ++i) {
                                ::new ((void*)&pNewData[i]) T(std::move(m_pData[i]));
                            }
                            FreeBuffer();
                            m_pData = pNewData;
                            m_iCapacity = iValue;
                        }
                    }

					T& operator[](int iIndex) {
						return m_pData[iIndex];
					}

					const T& operator[](int iIndex) const {
						return m_pData[iIndex];
					}

					void Add(const T& item) {
                        if (m_iCount == m_iCapacity) {
                            SetCapacity(m_iCapacity == 0 ? 4 : m_iCapacity * 2);
                        }
                        ::new ((void*)&m_pData[m_iCount]) T(item);
                        m_iCount++;
					}

					void AddRange(const Array<T>& arrCollection) {
                        int iNewCount = m_iCount + arrCollection.GetLength();
                        if (iNewCount > m_iCapacity) {
                            SetCapacity(iNewCount);
                        }
						for (int iIdx = 0; iIdx < arrCollection.GetLength(); iIdx++) {
                            ::new ((void*)&m_pData[m_iCount]) T(arrCollection[iIdx]);
                            m_iCount++;
						}
					}

					void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pData[i].~T();
                        }
                        m_iCount = 0;
					}

					bool Contains(const T& item) const {
						return IndexOf(item) != -1;
					}

					int IndexOf(const T& item) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pData[i] == item) return i;
                        }
                        return -1;
					}

					int BinarySearch(const T& item) const {
                        int low = 0;
                        int high = m_iCount - 1;
                        while (low <= high) {
                            int mid = low + (high - low) / 2;
                            if (m_pData[mid] == item) return mid;
                            if (m_pData[mid] < item) {
                                low = mid + 1;
                            } else {
                                high = mid - 1;
                            }
                        }
                        return ~low;
					}

					void Insert(int iIndex, const T& item) {
                        if (m_iCount == m_iCapacity) {
                            SetCapacity(m_iCapacity == 0 ? 4 : m_iCapacity * 2);
                        }
                        if (iIndex < m_iCount) {
                            ::new ((void*)&m_pData[m_iCount]) T(std::move(m_pData[m_iCount - 1]));
                            for (int i = m_iCount - 1; i > iIndex; --i) {
                                m_pData[i] = std::move(m_pData[i - 1]);
                            }
                            m_pData[iIndex] = item;
                        } else {
                            ::new ((void*)&m_pData[m_iCount]) T(item);
                        }
                        m_iCount++;
					}

					bool Remove(const T& item) {
                        int idx = IndexOf(item);
                        if (idx != -1) {
                            RemoveAt(idx);
                            return true;
                        }
                        return false;
					}

					void RemoveAt(int iIndex) {
                        if (iIndex >= 0 && iIndex < m_iCount) {
                            for (int i = iIndex; i < m_iCount - 1; ++i) {
                                m_pData[i] = std::move(m_pData[i + 1]);
                            }
                            m_pData[m_iCount - 1].~T();
                            m_iCount--;
                        }
					}

                    void SwapElements(T& a, T& b) {
                        T temp = std::move(a);
                        a = std::move(b);
                        b = std::move(temp);
                    }

					void Sort() {
                        for (int i = 0; i < m_iCount - 1; ++i) {
                            for (int j = 0; j < m_iCount - i - 1; ++j) {
                                if (m_pData[j] > m_pData[j + 1]) {
                                    SwapElements(m_pData[j], m_pData[j + 1]);
                                }
                            }
                        }
					}

					template <typename Predicate>
					bool Exists(Predicate fnMatch) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (fnMatch(m_pData[i])) return true;
                        }
                        return false;
					}

					template <typename Predicate>
					T Find(Predicate fnMatch) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (fnMatch(m_pData[i])) return m_pData[i];
                        }
                        return T();
					}

					template <typename Predicate>
					List<T> FindAll(Predicate fnMatch) const {
						List<T> lstResult;
						for (int i = 0; i < m_iCount; ++i) {
							if (fnMatch(m_pData[i])) lstResult.Add(m_pData[i]);
						}
						return lstResult;
					}

					template <typename Predicate>
					bool TrueForAll(Predicate fnMatch) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (!fnMatch(m_pData[i])) return false;
                        }
                        return true;
					}

					Array<T> ToArray() const {
						Array<T> arrResult(m_iCount);
						for (int iIdx = 0; iIdx < m_iCount; iIdx++) {
							arrResult[iIdx] = m_pData[iIdx];
						}
						return arrResult;
					}

					// Iterator support for range-based for loops
					T* begin() { return m_pData; }
					T* end() { return m_pData + m_iCount; }
					const T* begin() const { return m_pData; }
					const T* end() const { return m_pData + m_iCount; }

				private:
					T* m_pData = nullptr;
                    int m_iCount = 0;
                    int m_iCapacity = 0;

                    void FreeBuffer() {
                        if (m_pData) {
                            for (int i = 0; i < m_iCount; ++i) {
                                m_pData[i].~T();
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

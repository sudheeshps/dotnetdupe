/// \file List.h
/// \brief Represents a strongly typed list of objects that can be accessed by index mirroring .NET List<T>.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

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

				/// \class List
				/// \brief Represents a strongly typed list of objects accessible by index.
				/// \tparam T The type of elements in the list.
				/// 
				/// Implements dynamic array resizing with amortized O(1) additions, binary search,
				/// sorting, and placement new allocation without STL container leakage.
				/// \note Thread Safety: Public static members of this type are thread safe. Instance members are not guaranteed to be thread safe.
				/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
				template <typename T>
				class List : public Object {
				public:
					/// \brief Initializes a new instance of the List class that is empty.
					List() { }

					/// \brief Initializes a new instance of the List class with specified initial capacity.
					/// \param iCapacity Initial buffer capacity.
					List(int iCapacity) { SetCapacity(iCapacity); }

					/// \brief Initializes a new instance of the List class containing elements from an initializer list.
					/// \param vCollection The initializer list whose elements are copied.
					List(const std::initializer_list<T>& vCollection) {
                        SetCapacity((int)vCollection.size());
                        for (const auto& item : vCollection) {
                            Add(item);
                        }
                    }

					/// \brief Copy constructor.
					/// \param lstOther The list to copy.
					List(const List& lstOther) {
                        SetCapacity(lstOther.m_iCapacity);
                        for (int i = 0; i < lstOther.m_iCount; ++i) {
                            Add(lstOther.m_pData[i]);
                        }
                    }
					/// \brief Copy assignment operator.
					/// \param lstOther The list to copy.
					/// \return Reference to this list.
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

					/// \brief Move constructor.
					/// \param lstOther The list to move from.
					List(List&& lstOther) noexcept : m_pData(lstOther.m_pData), m_iCount(lstOther.m_iCount), m_iCapacity(lstOther.m_iCapacity) {
                        lstOther.m_pData = nullptr;
                        lstOther.m_iCount = 0;
                        lstOther.m_iCapacity = 0;
                    }

					/// \brief Move assignment operator.
					/// \param lstOther The list to move from.
					/// \return Reference to this list.
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

					/// \brief Destructor releasing internal buffer resources.
                    ~List() override {
                        FreeBuffer();
                    }

					/// \brief Gets the number of elements contained in the List.
					/// \return The number of elements contained in the List.
					int GetCount() const { return m_iCount; }

					/// \brief Gets the total number of elements the internal data structure can hold without resizing.
					/// \return The capacity of the List.
					int GetCapacity() const { return m_iCapacity; }
					
					/// \brief Sets the capacity of the internal buffer to a specified value.
					/// \param iValue The new capacity.
					/// \note Algorithm: Geometric Capacity Growth & Placement Move transfers elements via placement move constructor.
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

					/// \brief Gets a reference to the element at the specified index.
					/// \param iIndex The zero-based index of the element to get.
					/// \return Reference to the element at index.
					T& operator[](int iIndex) {
						return m_pData[iIndex];
					}

					/// \brief Gets a const reference to the element at the specified index.
					/// \param iIndex The zero-based index of the element to get.
					/// \return Const reference to the element at index.
					const T& operator[](int iIndex) const {
						return m_pData[iIndex];
					}

					/// \brief Adds an object to the end of the List.
					/// \param item The object to be added to the end of the List.
					/// \note Amortized O(1) insertion: doubles capacity when current storage is exhausted.
					void Add(const T& item) {
                        if (m_iCount == m_iCapacity) {
                            SetCapacity(m_iCapacity == 0 ? 4 : m_iCapacity * 2);
                        }
                        ::new ((void*)&m_pData[m_iCount]) T(item);
                        m_iCount++;
					}

					/// \brief Adds the elements of the specified array to the end of the List.
					/// \param arrCollection The array whose elements should be added.
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

					/// \brief Removes all elements from the List.
					void Clear() {
                        for (int i = 0; i < m_iCount; ++i) {
                            m_pData[i].~T();
                        }
                        m_iCount = 0;
					}

					/// \brief Determines whether an element is in the List.
					/// \param item The object to locate in the List.
					/// \return True if item is found; otherwise, false.
					bool Contains(const T& item) const {
						return IndexOf(item) != -1;
					}

					/// \brief Searches for the specified object and returns the zero-based index of the first occurrence within the entire List.
					/// \param item The object to locate in the List.
					/// \return The zero-based index of the first occurrence if found; otherwise, -1.
					int IndexOf(const T& item) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (m_pData[i] == item) return i;
                        }
                        return -1;
					}

					/// \brief Searches the entire sorted List for an element using the default comparer.
					/// \param item The object to locate.
					/// \return The zero-based index of item in the sorted List, if item is found; otherwise, a negative number that is the bitwise complement of the index of the next element.
					/// \note Performs logarithmic O(log N) binary search lookup.
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

					/// \brief Inserts an element into the List at the specified index.
					/// \param iIndex The zero-based index at which item should be inserted.
					/// \param item The object to insert.
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

					/// \brief Removes the first occurrence of a specific object from the List.
					/// \param item The object to remove from the List.
					/// \return True if item is successfully removed; otherwise, false.
					bool Remove(const T& item) {
                        int idx = IndexOf(item);
                        if (idx != -1) {
                            RemoveAt(idx);
                            return true;
                        }
                        return false;
					}

					/// \brief Removes the element at the specified index of the List.
					/// \param iIndex The zero-based index of the element to remove.
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

					/// \brief Sorts the elements in the entire List using the default comparer.
					void Sort() {
                        for (int i = 0; i < m_iCount - 1; ++i) {
                            for (int j = 0; j < m_iCount - i - 1; ++j) {
                                if (m_pData[j] > m_pData[j + 1]) {
                                    SwapElements(m_pData[j], m_pData[j + 1]);
                                }
                            }
                        }
					}

					/// \brief Determines whether the List contains elements that match the conditions defined by the specified predicate.
					/// \tparam Predicate The predicate callable type.
					/// \param fnMatch The delegate defining the conditions of the elements to search for.
					/// \return True if the List contains one or more elements that match the conditions; otherwise, false.
					template <typename Predicate>
					bool Exists(Predicate fnMatch) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (fnMatch(m_pData[i])) return true;
                        }
                        return false;
					}

					/// \brief Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence.
					/// \tparam Predicate The predicate callable type.
					/// \param fnMatch The delegate defining the conditions of the element to search for.
					/// \return The first element that matches the conditions, or default T if not found.
					template <typename Predicate>
					T Find(Predicate fnMatch) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (fnMatch(m_pData[i])) return m_pData[i];
                        }
                        return T();
					}

					/// \brief Retrieves all the elements that match the conditions defined by the specified predicate.
					/// \tparam Predicate The predicate callable type.
					/// \param fnMatch The delegate defining the conditions of the elements to search for.
					/// \return A List containing all elements that match the conditions.
					template <typename Predicate>
					List<T> FindAll(Predicate fnMatch) const {
						List<T> lstResult;
						for (int i = 0; i < m_iCount; ++i) {
							if (fnMatch(m_pData[i])) lstResult.Add(m_pData[i]);
						}
						return lstResult;
					}

					/// \brief Determines whether every element in the List matches the conditions defined by the specified predicate.
					/// \tparam Predicate The predicate callable type.
					/// \param fnMatch The delegate defining the conditions to check against the elements.
					/// \return True if every element in the List matches the conditions; otherwise, false.
					template <typename Predicate>
					bool TrueForAll(Predicate fnMatch) const {
                        for (int i = 0; i < m_iCount; ++i) {
                            if (!fnMatch(m_pData[i])) return false;
                        }
                        return true;
					}

					/// \brief Copies the elements of the List to a new Array.
					/// \return An Array containing copies of the elements of the List.
					Array<T> ToArray() const {
						Array<T> arrResult(m_iCount);
						for (int iIdx = 0; iIdx < m_iCount; iIdx++) {
							arrResult[iIdx] = m_pData[iIdx];
						}
						return arrResult;
					}

					/// \brief Returns a pointer to the first element for range-based for loops.
					T* begin() { return m_pData; }

					/// \brief Returns a pointer to one past the last element for range-based for loops.
					T* end() { return m_pData + m_iCount; }

					/// \brief Returns a const pointer to the first element for range-based for loops.
					const T* begin() const { return m_pData; }

					/// \brief Returns a const pointer to one past the last element for range-based for loops.
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

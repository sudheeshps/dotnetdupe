/// \file Array.h
/// \brief Provides methods for creating, manipulating, searching, and sorting arrays.
///
/// Standard Citation: ECMA-335 CLI Common Language Infrastructure.

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/Predicate.h"
#include "System/Action.h"
#include "System/String.h"
#include <new>
#include <initializer_list>
#include <utility>
#include <cstddef>

namespace DotNetDupe {
	namespace System {

		/// \class Array
		/// \brief Provides methods for creating, manipulating, searching, and sorting arrays, thereby serving as the base class for all arrays in the common language runtime.
		///
		/// \tparam T The element type contained in the array.
		/// \note Conforms to ECMA-335 Partition IV Section 5.20 (System.Array).
		template <class T>
		class Array : public Object {
		private:
			T* m_pData = nullptr;
			int m_iLength = 0;

			void Allocate(int iLength) {
				if (iLength > 0) {
					m_pData = static_cast<T*>(AllocateCollectionBuffer(sizeof(T) * iLength));
					m_iLength = iLength;
				}
			}

			void Free() {
				if (m_pData) {
					for (int i = 0; i < m_iLength; ++i) {
						m_pData[i].~T();
					}
					FreeCollectionBuffer(m_pData);
					m_pData = nullptr;
					m_iLength = 0;
				}
			}

			void SwapElements(T& a, T& b) {
				T temp = std::move(a);
				a = std::move(b);
				b = std::move(temp);
			}

		public:
			/// \brief Initializes an empty Array instance.
			Array() = default;

			/// \brief Initializes an Array of the specified length with default-constructed elements.
			/// \param iLength The number of elements in the array.
			Array(int iLength) {
				Allocate(iLength);
				for (int i = 0; i < m_iLength; ++i) {
					::new ((void*)&m_pData[i]) T();
				}
			}

			/// \brief Initializes an Array by copying elements from a raw pointer buffer.
			/// \param pData Pointer to the source elements buffer.
			/// \param iLength The number of elements to copy.
			Array(const T* pData, int iLength) {
				Allocate(iLength);
				if (pData) {
					for (int i = 0; i < m_iLength; ++i) {
						::new ((void*)&m_pData[i]) T(pData[i]);
					}
				} else {
					for (int i = 0; i < m_iLength; ++i) {
						::new ((void*)&m_pData[i]) T();
					}
				}
			}

			/// \brief Initializes an Array from an initializer list of elements.
			/// \param vItems Initializer list containing array elements.
			Array(const std::initializer_list<T>& vItems) {
				Allocate(static_cast<int>(vItems.size()));
				int iIdx = 0;
				for (const auto& item : vItems) {
					::new ((void*)&m_pData[iIdx++]) T(item);
				}
			}

			/// \brief Copy constructor. Performs a deep copy of elements.
			Array(const Array& other) {
				Allocate(other.m_iLength);
				for (int i = 0; i < m_iLength; ++i) {
					::new ((void*)&m_pData[i]) T(other.m_pData[i]);
				}
			}

			/// \brief Copy assignment operator.
			Array& operator=(const Array& other) {
				if (this != &other) {
					Free();
					Allocate(other.m_iLength);
					for (int i = 0; i < m_iLength; ++i) {
						::new ((void*)&m_pData[i]) T(other.m_pData[i]);
					}
				}
				return *this;
			}

			/// \brief Move constructor.
			Array(Array&& other) noexcept : m_pData(other.m_pData), m_iLength(other.m_iLength) {
				other.m_pData = nullptr;
				other.m_iLength = 0;
			}

			/// \brief Move assignment operator.
			Array& operator=(Array&& other) noexcept {
				if (this != &other) {
					Free();
					m_pData = other.m_pData;
					m_iLength = other.m_iLength;
					other.m_pData = nullptr;
					other.m_iLength = 0;
				}
				return *this;
			}

			/// \brief Destroys array elements and releases allocated storage.
			~Array() override {
				Free();
			}

			/// \brief Gets the total number of elements in all dimensions of the Array.
			/// \return The total number of elements.
			int GetLength() const { return m_iLength; }

			/// \brief Gets a pointer to the contiguous internal element buffer.
			/// \return A pointer to the element array.
			T* GetData() { return m_pData; }

			/// \brief Gets a const pointer to the contiguous internal element buffer.
			/// \return A const pointer to the element array.
			const T* GetData() const { return m_pData; }

			/// \brief Returns an iterator to the first element of the array.
			T* begin() { return m_pData; }

			/// \brief Returns an iterator to the element following the last element of the array.
			T* end() { return m_pData + m_iLength; }

			/// \brief Returns a const iterator to the first element of the array.
			const T* begin() const { return m_pData; }

			/// \brief Returns a const iterator to the element following the last element of the array.
			const T* end() const { return m_pData + m_iLength; }

			/// \brief Checks whether the array has zero length.
			/// \return true if length is 0; otherwise, false.
			bool IsNull() const { return m_iLength == 0; }

			/// \brief Accesses the element at the specified index.
			T& operator[](int iIndex) { return m_pData[iIndex]; }

			/// \brief Accesses the const element at the specified index.
			const T& operator[](int iIndex) const { return m_pData[iIndex]; }

			/// \brief Searches for the specified object and returns the index of its first occurrence.
			/// \param value The object to locate in the Array.
			/// \return The zero-based index of the first occurrence of value, if found; otherwise, -1.
			int IndexOf(const T& value) const {
				for (int iIdx = 0; iIdx < m_iLength; ++iIdx) {
					if (m_pData[iIdx] == value) return iIdx;
				}
				return -1;
			}

			/// \brief Searches for the specified object and returns the index of the last occurrence.
			/// \param value The object to locate in the Array.
			/// \return The zero-based index of the last occurrence of value, if found; otherwise, -1.
			int LastIndexOf(const T& value) const {
				for (int iIdx = m_iLength - 1; iIdx >= 0; --iIdx) {
					if (m_pData[iIdx] == value) return iIdx;
				}
				return -1;
			}

			/// \brief Sorts the elements in an entire Array using the default comparison.
			void Sort() {
				for (int i = 0; i < m_iLength - 1; ++i) {
					for (int j = 0; j < m_iLength - i - 1; ++j) {
						if (m_pData[j] > m_pData[j + 1]) {
							SwapElements(m_pData[j], m_pData[j + 1]);
						}
					}
				}
			}

			/// \brief Reverses the sequence of the elements in the entire Array.
			void Reverse() {
				int left = 0;
				int right = m_iLength - 1;
				while (left < right) {
					SwapElements(m_pData[left], m_pData[right]);
					left++;
					right--;
				}
			}

			/// \brief Sets a range of elements in the Array to the default value of each element type.
			void Clear() {
				for (int i = 0; i < m_iLength; ++i) {
					m_pData[i] = T();
				}
			}

			/// \brief Copies all elements of the current Array to the specified destination Array starting at the specified destination index.
			void CopyTo(Array<T>& arrTarget, int iIndex);

			/// \brief Copies a range of elements from an Array starting at the first element and pastes them to another Array starting at the first element.
			static void Copy(Array<T>& arrSource, Array<T>& arrDestination, int iLength);

			/// \brief Determines whether the specified array contains elements that match the conditions defined by the specified predicate.
			bool Exists(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) return true;
				}
				return false;
			}

			/// \brief Searches for an element that matches the conditions defined by the specified predicate, and returns the first occurrence.
			T Find(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) return m_pData[i];
				}
				return T();
			}

			/// \brief Retrieves all the elements that match the conditions defined by the specified predicate.
			Array<T> FindAll(const Predicate<T>& fnPredicate) const {
				int count = 0;
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) count++;
				}
				Array<T> arrNew(count);
				int idx = 0;
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) {
						arrNew[idx++] = m_pData[i];
					}
				}
				return arrNew;
			}

			/// \brief Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the first occurrence.
			int FindIndex(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) return i;
				}
				return -1;
			}

			/// \brief Searches for an element that matches the conditions defined by the specified predicate, and returns the last occurrence.
			T FindLast(const Predicate<T>& fnPredicate) const {
				for (int i = m_iLength - 1; i >= 0; --i) {
					if (fnPredicate(m_pData[i])) return m_pData[i];
				}
				return T();
			}

			/// \brief Searches for an element that matches the conditions defined by the specified predicate, and returns the zero-based index of the last occurrence.
			int FindLastIndex(const Predicate<T>& fnPredicate) const {
				for (int i = m_iLength - 1; i >= 0; --i) {
					if (fnPredicate(m_pData[i])) return i;
				}
				return -1;
			}

			/// \brief Performs the specified action on each element of the specified array.
			void ForEach(const Action<T>& fnAction) {
				for (int i = 0; i < m_iLength; ++i) {
					fnAction(m_pData[i]);
				}
			}

			/// \brief Determines whether every element in the array matches the conditions defined by the specified predicate.
			bool TrueForAll(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (!fnPredicate(m_pData[i])) return false;
				}
				return true;
			}
		};
	}
}

#include "System/String.h"

namespace DotNetDupe {
	namespace System {
		template <class T>
		inline void Array<T>::CopyTo(Array<T>& arrTarget, int iIndex) {
			/// Guard: Validate target index boundary.
			if (iIndex < 0) throw ArgumentOutOfRangeException("iIndex");
			if (iIndex + GetLength() > arrTarget.GetLength()) throw ArgumentException("Destination array was not long enough.");

			/// Copy elements sequentially into target buffer.
			for (int iIdx = 0; iIdx < GetLength(); ++iIdx) {
				arrTarget[iIndex + iIdx] = m_pData[iIdx];
			}
		}

		template <class T>
		inline void Array<T>::Copy(Array<T>& arrSource, Array<T>& arrDestination, int iLength) {
			/// Guard: Validate length boundary against source and destination capacities.
			if (iLength < 0) throw ArgumentOutOfRangeException("iLength");
			if (arrSource.GetLength() < iLength) throw ArgumentException("Source array was not long enough.");
			if (arrDestination.GetLength() < iLength) throw ArgumentException("Destination array was not long enough.");

			/// Copy elements across array bounds.
			for (int iIdx = 0; iIdx < iLength; ++iIdx) {
				arrDestination[iIdx] = arrSource[iIdx];
			}
		}
	}
}

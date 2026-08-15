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
			Array() = default;
			Array(int iLength) {
				Allocate(iLength);
				for (int i = 0; i < m_iLength; ++i) {
					::new ((void*)&m_pData[i]) T();
				}
			}
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
			Array(const std::initializer_list<T>& vItems) {
				Allocate(static_cast<int>(vItems.size()));
				int iIdx = 0;
				for (const auto& item : vItems) {
					::new ((void*)&m_pData[iIdx++]) T(item);
				}
			}
			Array(const Array& other) {
				Allocate(other.m_iLength);
				for (int i = 0; i < m_iLength; ++i) {
					::new ((void*)&m_pData[i]) T(other.m_pData[i]);
				}
			}
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
			Array(Array&& other) noexcept : m_pData(other.m_pData), m_iLength(other.m_iLength) {
				other.m_pData = nullptr;
				other.m_iLength = 0;
			}
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
			~Array() override {
				Free();
			}

			int GetLength() const { return m_iLength; }
			T* GetData() { return m_pData; }
			const T* GetData() const { return m_pData; }

			T* begin() { return m_pData; }
			T* end() { return m_pData + m_iLength; }
			const T* begin() const { return m_pData; }
			const T* end() const { return m_pData + m_iLength; }

			bool IsNull() const { return m_iLength == 0; }

			T& operator[](int iIndex) { return m_pData[iIndex]; }
			const T& operator[](int iIndex) const { return m_pData[iIndex]; }

			int IndexOf(const T& value) const {
				for (int iIdx = 0; iIdx < m_iLength; ++iIdx) {
					if (m_pData[iIdx] == value) return iIdx;
				}
				return -1;
			}

			int LastIndexOf(const T& value) const {
				for (int iIdx = m_iLength - 1; iIdx >= 0; --iIdx) {
					if (m_pData[iIdx] == value) return iIdx;
				}
				return -1;
			}

			void Sort() {
				for (int i = 0; i < m_iLength - 1; ++i) {
					for (int j = 0; j < m_iLength - i - 1; ++j) {
						if (m_pData[j] > m_pData[j + 1]) {
							SwapElements(m_pData[j], m_pData[j + 1]);
						}
					}
				}
			}

			void Reverse() {
				int left = 0;
				int right = m_iLength - 1;
				while (left < right) {
					SwapElements(m_pData[left], m_pData[right]);
					left++;
					right--;
				}
			}

			void Clear() {
				for (int i = 0; i < m_iLength; ++i) {
					m_pData[i] = T();
				}
			}

			void CopyTo(Array<T>& arrTarget, int iIndex);
			static void Copy(Array<T>& arrSource, Array<T>& arrDestination, int iLength);

			bool Exists(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) return true;
				}
				return false;
			}

			T Find(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) return m_pData[i];
				}
				return T();
			}

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

			int FindIndex(const Predicate<T>& fnPredicate) const {
				for (int i = 0; i < m_iLength; ++i) {
					if (fnPredicate(m_pData[i])) return i;
				}
				return -1;
			}

			T FindLast(const Predicate<T>& fnPredicate) const {
				for (int i = m_iLength - 1; i >= 0; --i) {
					if (fnPredicate(m_pData[i])) return m_pData[i];
				}
				return T();
			}

			int FindLastIndex(const Predicate<T>& fnPredicate) const {
				for (int i = m_iLength - 1; i >= 0; --i) {
					if (fnPredicate(m_pData[i])) return i;
				}
				return -1;
			}

			void ForEach(const Action<T>& fnAction) {
				for (int i = 0; i < m_iLength; ++i) {
					fnAction(m_pData[i]);
				}
			}

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
			if (iIndex < 0) throw ArgumentOutOfRangeException("iIndex");
			if (iIndex + GetLength() > arrTarget.GetLength()) throw ArgumentException("Destination array was not long enough.");

			for (int iIdx = 0; iIdx < GetLength(); ++iIdx) {
				arrTarget[iIndex + iIdx] = m_pData[iIdx];
			}
		}

		template <class T>
		inline void Array<T>::Copy(Array<T>& arrSource, Array<T>& arrDestination, int iLength) {
			if (iLength < 0) throw ArgumentOutOfRangeException("iLength");
			if (arrSource.GetLength() < iLength) throw ArgumentException("Source array was not long enough.");
			if (arrDestination.GetLength() < iLength) throw ArgumentException("Destination array was not long enough.");

			for (int iIdx = 0; iIdx < iLength; ++iIdx) {
				arrDestination[iIdx] = arrSource[iIdx];
			}
		}
	}
}

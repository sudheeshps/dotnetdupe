#pragma once

#include "System/Char.h"

#include "Common.h"
#include "System/Object.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"

#include "System/Predicate.h"
#include "System/Action.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

namespace DotNetDupe {
    namespace System {
        template <class T>
        class Array : public Object {
        private:
            std::vector<T> m_vItems;

        public:
            // Constructors
            Array(int iLength) : m_vItems(iLength) { }
            Array(const std::initializer_list<T>& vItems) : m_vItems(vItems) { }

            // Properties
            int GetLength() const {
                return static_cast<int>(m_vItems.size());
            }

            T* GetData() {
                return m_vItems.data();
            }

            const T* GetData() const {
                return m_vItems.data();
            }

            // Iterator support for range-based for loops
            auto begin() { return m_vItems.begin(); }
            auto end() { return m_vItems.end(); }
            auto begin() const { return m_vItems.begin(); }
            auto end() const { return m_vItems.end(); }

            bool IsNull() const {
                return m_vItems.empty();
            }

            // Indexer
            T& operator[](int iIndex) {
                return m_vItems [iIndex];
            }

            const T& operator[](int iIndex) const {
                return m_vItems [iIndex];
            }

            int IndexOf(const T& value) const {
                for (int iIdx = 0; iIdx < static_cast<int>(m_vItems.size()); ++iIdx) {
                    if (m_vItems [iIdx] == value) {
                        return iIdx;
                    }
                }
                return -1;
            }

            int LastIndexOf(const T& value) const {
                for (int iIdx = static_cast<int>(m_vItems.size()) - 1; iIdx >= 0; --iIdx) {
                    if (m_vItems [iIdx] == value) {
                        return iIdx;
                    }
                }
                return -1;
            }

            void Sort() {
                std::sort(m_vItems.begin(), m_vItems.end());
            }

            void Reverse() {
                std::reverse(m_vItems.begin(), m_vItems.end());
            }

            void Clear() {
                std::fill(m_vItems.begin(), m_vItems.end(), T());
            }

            void CopyTo(Array<T>& arrTarget, int iIndex) {
                if (iIndex < 0)
                    throw ArgumentOutOfRangeException("iIndex");
                if (iIndex + GetLength() > arrTarget.GetLength())
                    throw ArgumentException("Destination array was not long enough.");

                for (int iIdx = 0; iIdx < GetLength(); ++iIdx) {
                    arrTarget [iIndex + iIdx] = m_vItems [iIdx];
                }
            }

            static void Copy(Array<T>& arrSource, Array<T>& arrDestination, int iLength) {
                if (iLength < 0)
                    throw ArgumentOutOfRangeException("iLength");
                if (arrSource.GetLength() < iLength)
                    throw ArgumentException("Source array was not long enough.");
                if (arrDestination.GetLength() < iLength)
                    throw ArgumentException("Destination array was not long enough.");

                for (int iIdx = 0; iIdx < iLength; ++iIdx) {
                    arrDestination [iIdx] = arrSource [iIdx];
                }
            }

            bool Exists(const Predicate<T>& fnPredicate) const {
                for (const auto& item : m_vItems) {
                    if (fnPredicate(item)) {
                        return true;
                    }
                }
                return false;
            }

            T Find(const Predicate<T>& fnPredicate) const {
                for (const auto& item : m_vItems) {
                    if (fnPredicate(item)) {
                        return item;
                    }
                }
                return T();
            }

            Array<T> FindAll(const Predicate<T>& fnPredicate) const {
                std::vector<T> vFoundItems;
                for (const auto& item : m_vItems) {
                    if (fnPredicate(item)) {
                        vFoundItems.push_back(item);
                    }
                }
                Array<T> arrNew(0);
                arrNew.m_vItems = vFoundItems;
                return arrNew;
            }

            int FindIndex(const Predicate<T>& fnPredicate) const {
                for (int iIdx = 0; iIdx < static_cast<int>(m_vItems.size()); ++iIdx) {
                    if (fnPredicate(m_vItems [iIdx])) {
                        return iIdx;
                    }
                }
                return -1;
            }

            T FindLast(const Predicate<T>& fnPredicate) const {
                for (int iIdx = static_cast<int>(m_vItems.size()) - 1; iIdx >= 0; --iIdx) {
                    if (fnPredicate(m_vItems [iIdx])) {
                        return m_vItems [iIdx];
                    }
                }
                return T();
            }

            int FindLastIndex(const Predicate<T>& fnPredicate) const {
                for (int iIdx = static_cast<int>(m_vItems.size()) - 1; iIdx >= 0; --iIdx) {
                    if (fnPredicate(m_vItems [iIdx])) {
                        return iIdx;
                    }
                }
                return -1;
            }

            void ForEach(const Action<T>& fnAction) {
                for (const auto& item : m_vItems) {
                    fnAction(item);
                }
            }

            bool TrueForAll(const Predicate<T>& fnPredicate) const {
                for (const auto& item : m_vItems) {
                    if (!fnPredicate(item)) {
                        return false;
                    }
                }
                return true;
            }
        };
    }
}

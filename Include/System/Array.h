#pragma once

#include <tchar.h>

#include "Common.h"
#include "System/Object.h"
#include "System/ArgumentException.h"
#include "System/ArgumentOutOfRangeException.h"

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace DotNetDupe {
    namespace System {
        template <class T>
        class Array : public Object {
        private:
            std::vector<T> _items;

        public:
            // Constructors
            Array(int length) : _items(length) { }
            Array(const std::initializer_list<T>& items) : _items(items) { }

            // Properties
            int GetLength() const {
                return static_cast<int>(_items.size());
            }

            T* GetData() {
                return _items.data();
            }

            bool IsNull() const {
                return _items.empty();
            }

            // Indexer
            T& operator[](int index) {
                return _items [index];
            }

            const T& operator[](int index) const {
                return _items [index];
            }

            int IndexOf(const T& value) const {
                for (int i = 0; i < static_cast<int>(_items.size()); ++i) {
                    if (_items [i] == value) {
                        return i;
                    }
                }
                return -1;
            }

            int LastIndexOf(const T& value) const {
                for (int i = static_cast<int>(_items.size()) - 1; i >= 0; --i) {
                    if (_items [i] == value) {
                        return i;
                    }
                }
                return -1;
            }

            void Sort() {
                std::sort(_items.begin(), _items.end());
            }

            void Reverse() {
                std::reverse(_items.begin(), _items.end());
            }

            void Clear() {
                std::fill(_items.begin(), _items.end(), T());
            }

            void CopyTo(Array<T>& array, int index) {
                if (index < 0)
                    throw ArgumentOutOfRangeException(_T("index"));
                if (index + GetLength() > array.GetLength())
                    throw ArgumentException(_T("Destination array was not long enough."));

                for (int i = 0; i < GetLength(); ++i) {
                    array [index + i] = _items [i];
                }
            }

            static void Copy(Array<T>& sourceArray, Array<T>& destinationArray, int length) {
                if (length < 0)
                    throw ArgumentOutOfRangeException(_T("length"));
                if (sourceArray.GetLength() < length)
                    throw ArgumentException(_T("Source array was not long enough."));
                if (destinationArray.GetLength() < length)
                    throw ArgumentException(_T("Destination array was not long enough."));

                for (int i = 0; i < length; ++i) {
                    destinationArray [i] = sourceArray [i];
                }
            }

            bool Exists(const std::function<bool(T)>& predicate) const {
                for (const auto& item : _items) {
                    if (predicate(item)) {
                        return true;
                    }
                }
                return false;
            }

            T Find(const std::function<bool(T)>& predicate) const {
                for (const auto& item : _items) {
                    if (predicate(item)) {
                        return item;
                    }
                }
                return T();
            }

            Array<T> FindAll(const std::function<bool(T)>& predicate) const {
                std::vector<T> foundItems;
                for (const auto& item : _items) {
                    if (predicate(item)) {
                        foundItems.push_back(item);
                    }
                }
                Array<T> newArr(0);
                newArr._items = foundItems;
                return newArr;
            }

            int FindIndex(const std::function<bool(T)>& predicate) const {
                for (int i = 0; i < static_cast<int>(_items.size()); ++i) {
                    if (predicate(_items [i])) {
                        return i;
                    }
                }
                return -1;
            }

            T FindLast(const std::function<bool(T)>& predicate) const {
                for (int i = static_cast<int>(_items.size()) - 1; i >= 0; --i) {
                    if (predicate(_items [i])) {
                        return _items [i];
                    }
                }
                return T();
            }

            int FindLastIndex(const std::function<bool(T)>& predicate) const {
                for (int i = static_cast<int>(_items.size()) - 1; i >= 0; --i) {
                    if (predicate(_items [i])) {
                        return i;
                    }
                }
                return -1;
            }

            void ForEach(const std::function<void(T)>& action) {
                for (const auto& item : _items) {
                    action(item);
                }
            }

            bool TrueForAll(const std::function<bool(T)>& predicate) const {
                for (const auto& item : _items) {
                    if (!predicate(item)) {
                        return false;
                    }
                }
                return true;
            }
        };
    }
}

#pragma once

#include "Common.h"
#include "System/Object.h"
#include "System/Array.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename T>
                class List : public Object {
                public:
                    List() {}
                    List(int capacity) { _items.reserve(capacity); }
                    List(const std::initializer_list<T>& collection) : _items(collection) {}

                    int GetCount() const { return (int)_items.size(); }
                    int GetCapacity() const { return (int)_items.capacity(); }
                    void SetCapacity(int value) { _items.reserve(value); }

                    T& operator[](int index) {
                        return _items.at(index);
                    }

                    const T& operator[](int index) const {
                        return _items.at(index);
                    }

                    void Add(const T& item) {
                        _items.push_back(item);
                    }

                    void AddRange(const std::vector<T>& collection) {
                        _items.insert(_items.end(), collection.begin(), collection.end());
                    }

                    void Clear() {
                        _items.clear();
                    }

                    bool Contains(const T& item) const {
                        return std::find(_items.begin(), _items.end(), item) != _items.end();
                    }

                    int IndexOf(const T& item) const {
                        auto it = std::find(_items.begin(), _items.end(), item);
                        if (it == _items.end()) return -1;
                        return (int)std::distance(_items.begin(), it);
                    }

                    void Insert(int index, const T& item) {
                        _items.insert(_items.begin() + index, item);
                    }

                    bool Remove(const T& item) {
                        auto it = std::find(_items.begin(), _items.end(), item);
                        if (it != _items.end()) {
                            _items.erase(it);
                            return true;
                        }
                        return false;
                    }

                    void RemoveAt(int index) {
                        _items.erase(_items.begin() + index);
                    }

                    void Sort() {
                        std::sort(_items.begin(), _items.end());
                    }

                    Array<T> ToArray() const {
                        Array<T> result((int)_items.size());
                        for (int i = 0; i < (int)_items.size(); i++) {
                            result[i] = _items[i];
                        }
                        return result;
                    }

                    // Iterator support for range-based for loops
                    auto begin() { return _items.begin(); }
                    auto end() { return _items.end(); }
                    auto begin() const { return _items.begin(); }
                    auto end() const { return _items.end(); }

                private:
                    std::vector<T> _items;
                };
            }
        }
    }
}

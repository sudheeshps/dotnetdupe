#pragma once

#include "Common.h"
#include "System/Object.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                template <typename TKey, typename TValue>
                struct KeyValuePair {
                    TKey Key;
                    TValue Value;
                    KeyValuePair(TKey k, TValue v) : Key(k), Value(v) {}
                };

                template <typename TKey, typename TValue>
                class Dictionary : public Object {
                public:
                    Dictionary() {}

                    int GetCount() const { return (int)_map.size(); }

                    TValue& operator[](const TKey& key) {
                        return _map[key];
                    }

                    const TValue& operator[](const TKey& key) const {
                        return _map.at(key);
                    }

                    void Add(const TKey& key, const TValue& value) {
                        if (ContainsKey(key)) throw std::runtime_error("An item with the same key has already been added.");
                        _map[key] = value;
                    }

                    void Clear() {
                        _map.clear();
                    }

                    bool ContainsKey(const TKey& key) const {
                        return _map.find(key) != _map.end();
                    }

                    bool Remove(const TKey& key) {
                        return _map.erase(key) > 0;
                    }

                    bool TryGetValue(const TKey& key, TValue& value) const {
                        auto it = _map.find(key);
                        if (it != _map.end()) {
                            value = it->second;
                            return true;
                        }
                        return false;
                    }

                    std::vector<TKey> GetKeys() const {
                        std::vector<TKey> keys;
                        for (auto const& [key, val] : _map) keys.push_back(key);
                        return keys;
                    }

                    std::vector<TValue> GetValues() const {
                        std::vector<TValue> values;
                        for (auto const& [key, val] : _map) values.push_back(val);
                        return values;
                    }

                    // Iterator support for range-based for loops
                    auto begin() { return _map.begin(); }
                    auto end() { return _map.end(); }
                    auto begin() const { return _map.begin(); }
                    auto end() const { return _map.end(); }

                private:
                    std::unordered_map<TKey, TValue> _map;
                };
            }
        }
    }
}

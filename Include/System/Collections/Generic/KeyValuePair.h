/// \file KeyValuePair.h
/// \brief Defines a key/value pair that can be set or retrieved.
///
/// Modeled after .NET System.Collections.Generic.KeyValuePair<TKey, TValue> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/EqualityHelper.h"

namespace DotNetDupe {
    namespace System {
        namespace Collections {
            namespace Generic {

                /// \struct KeyValuePair
                /// \brief Defines a key/value pair that can be set or retrieved.
                /// \tparam TKey The type of the key.
                /// \tparam TValue The type of the value.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename TKey, typename TValue>
                struct KeyValuePair {
                    TKey Key;     ///< Gets or sets the key in the key/value pair.
                    TValue Value; ///< Gets or sets the value in the key/value pair.

                    /// \brief Initializes a new instance of the KeyValuePair structure with default values.
                    KeyValuePair() = default;

                    /// \brief Initializes a new instance of the KeyValuePair structure with the specified key and value.
                    /// \param k The key defined in the key/value pair.
                    /// \param v The value associated with key.
                    KeyValuePair(const TKey& k, const TValue& v) : Key(k), Value(v) {}

                    /// \brief Equality comparison operator.
                    bool operator==(const KeyValuePair& other) const {
                        return EqualityHelper<TKey>::Equals(Key, other.Key) &&
                               EqualityHelper<TValue>::Equals(Value, other.Value);
                    }

                    /// \brief Inequality comparison operator.
                    bool operator!=(const KeyValuePair& other) const {
                        return !(*this == other);
                    }
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

/// \file IReadOnlyDictionary.h
/// \brief Represents a generic read-only collection of key/value pairs.
///
/// Modeled after .NET System.Collections.Generic.IReadOnlyDictionary<TKey, TValue> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/IReadOnlyCollection.h"
#include "System/Collections/Generic/KeyValuePair.h"

namespace DotNetDupe {
    namespace System {

        template <class T>
        class Array;

        namespace Collections {
            namespace Generic {

                /// \class IReadOnlyDictionary
                /// \brief Represents a generic read-only collection of key/value pairs.
                /// \tparam TKey The type of keys in the read-only dictionary.
                /// \tparam TValue The type of values in the read-only dictionary.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename TKey, typename TValue>
                class IReadOnlyDictionary : public virtual IReadOnlyCollection<KeyValuePair<TKey, TValue>> {
                public:
                    /// \brief Virtual destructor.
                    ~IReadOnlyDictionary() override = default;

                    /// \brief Determines whether the read-only dictionary contains an element that has the specified key.
                    /// \param key The key to locate.
                    /// \return True if the read-only dictionary contains an element that has the specified key; otherwise, false.
                    virtual bool ContainsKey(const TKey& key) const = 0;

                    /// \brief Gets the value that is associated with the specified key.
                    /// \param key The key to locate.
                    /// \param value When this method returns, the value associated with the specified key, if the key is found.
                    /// \return True if the read-only dictionary contains an element with the specified key; otherwise, false.
                    virtual bool TryGetValue(const TKey& key, TValue& value) const = 0;

                    /// \brief Gets the element that has the specified key in the read-only dictionary.
                    /// \param key The key to locate.
                    /// \return The element that has the specified key.
                    virtual const TValue& operator[](const TKey& key) const = 0;

                    /// \brief Gets an array containing the keys of the read-only dictionary.
                    /// \return An Array<TKey> containing the keys.
                    virtual Array<TKey> GetKeys() const = 0;

                    /// \brief Gets an array containing the values in the read-only dictionary.
                    /// \return An Array<TValue> containing the values.
                    virtual Array<TValue> GetValues() const = 0;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

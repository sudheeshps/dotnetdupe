/// \file IDictionary.h
/// \brief Represents a generic collection of key/value pairs.
///
/// Modeled after .NET System.Collections.Generic.IDictionary<TKey, TValue> (ECMA-335).

#pragma once

#include "Common.h"
#include "System/Collections/Generic/ICollection.h"
#include "System/Collections/Generic/IReadOnlyDictionary.h"
#include "System/Collections/Generic/KeyValuePair.h"

namespace DotNetDupe {
    namespace System {

        template <class T>
        class Array;

        namespace Collections {
            namespace Generic {

                /// \class IDictionary
                /// \brief Represents a generic collection of key/value pairs.
                /// \tparam TKey The type of keys in the dictionary.
                /// \tparam TValue The type of values in the dictionary.
                ///
                /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
                template <typename TKey, typename TValue>
                class IDictionary : public virtual ICollection<KeyValuePair<TKey, TValue>>,
                                    public virtual IReadOnlyDictionary<TKey, TValue> {
                public:
                    /// \brief Virtual destructor.
                    ~IDictionary() override = default;

                    /// \brief Gets or sets the element with the specified key.
                    /// \param key The key of the element to get or set.
                    /// \return The element with the specified key.
                    virtual TValue& operator[](const TKey& key) = 0;

                    /// \brief Gets the element with the specified key.
                    /// \param key The key of the element to get.
                    /// \return The element with the specified key.
                    virtual const TValue& operator[](const TKey& key) const override = 0;

                    /// \brief Adds an element with the provided key and value to the IDictionary.
                    /// \param key The object to use as the key of the element to add.
                    /// \param value The object to use as the value of the element to add.
                    virtual void Add(const TKey& key, const TValue& value) = 0;

                    /// \brief Removes the element with the specified key from the IDictionary.
                    /// \param key The key of the element to remove.
                    /// \return True if the element is successfully removed; otherwise, false.
                    virtual bool Remove(const TKey& key) = 0;

                    /// \brief Determines whether the IDictionary contains an element with the specified key.
                    /// \param key The key to locate in the IDictionary.
                    /// \return True if the IDictionary contains an element with the key; otherwise, false.
                    virtual bool ContainsKey(const TKey& key) const override = 0;

                    /// \brief Gets the value associated with the specified key.
                    /// \param key The key whose value to get.
                    /// \param value When this method returns, the value associated with the specified key, if found.
                    /// \return True if the object that implements IDictionary contains an element with the specified key; otherwise, false.
                    virtual bool TryGetValue(const TKey& key, TValue& value) const override = 0;

                    /// \brief Gets an Array containing the keys of the IDictionary.
                    /// \return An Array<TKey> containing the keys.
                    virtual Array<TKey> GetKeys() const override = 0;

                    /// \brief Gets an Array containing the values in the IDictionary.
                    /// \return An Array<TValue> containing the values.
                    virtual Array<TValue> GetValues() const override = 0;
                };

            } // namespace Generic
        } // namespace Collections
    } // namespace System
} // namespace DotNetDupe

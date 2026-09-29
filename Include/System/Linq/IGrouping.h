/// \file IGrouping.h
/// \brief Represents a collection of objects that have a common key.
///
/// Modeled after .NET System.Linq.IGrouping<TKey, TElement> (ECMA-335).

#pragma once

#include "System/Collections/Generic/List.h"
#include "System/Collections/Generic/IEnumerable.h"
#include <utility>

namespace DotNetDupe {
    namespace System {
        namespace Linq {

            /// \class IGrouping
            /// \brief Represents a collection of objects that have a common key.
            /// \tparam TKey The type of the key of the IGrouping.
            /// \tparam TElement The type of the values in the IGrouping.
            ///
            /// \note Conforms to ECMA-335 Partition IV Section 5 (System.Linq.IGrouping<TKey, TElement>).
            /// Standard Citation: ECMA-335 CLI Common Language Infrastructure.
            template <typename TKey, typename TElement>
            class IGrouping : public virtual Collections::Generic::IEnumerable<TElement> {
            private:
                TKey m_key;
                Collections::Generic::List<TElement> m_elements;

            public:
                /// \brief Initializes a new default instance of the IGrouping class.
                IGrouping() = default;

                /// \brief Initializes a new instance with specified key and element list.
                /// \param key The shared group key.
                /// \param elements The collection of elements associated with the key.
                IGrouping(const TKey& key, const Collections::Generic::List<TElement>& elements)
                    : m_key(key), m_elements(elements) {}

                /// \brief Initializes a new instance with specified key and moved element list.
                /// \param key The shared group key.
                /// \param elements The moved collection of elements.
                IGrouping(const TKey& key, Collections::Generic::List<TElement>&& elements)
                    : m_key(key), m_elements(std::move(elements)) {}

                /// \brief Gets the key of the IGrouping.
                /// \return The common key.
                const TKey& Key() const { return m_key; }

                /// \brief Gets the key of the IGrouping (alias for .NET Key property).
                /// \return The common key.
                const TKey& GetKey() const { return m_key; }

                /// \brief Gets the number of elements contained in the group.
                /// \return The count of elements.
                int Count() const { return m_elements.GetCount(); }

                /// \brief Gets the number of elements contained in the group.
                /// \return The count of elements.
                int GetCount() const { return m_elements.GetCount(); }

                /// \brief Gets a const reference to the underlying element list.
                /// \return The list of elements.
                const Collections::Generic::List<TElement>& Elements() const { return m_elements; }

                /// \brief Materializes the group into a new List instance.
                /// \return A copy of the elements list.
                Collections::Generic::List<TElement> ToList() const { return m_elements; }

                /// \brief Gets the element at the specified index.
                /// \param index The zero-based index.
                /// \return Const reference to the element.
                const TElement& operator[](int index) const { return m_elements[index]; }

                /// \brief Returns a pointer to the beginning of the contiguous elements buffer.
                /// \return Const pointer to the first element.
                const TElement* begin() const {
                    return m_elements.GetCount() > 0 ? &m_elements[0] : nullptr;
                }

                /// \brief Returns a pointer to the end of the contiguous elements buffer.
                /// \return Const pointer to the one-past-the-end element.
                const TElement* end() const {
                    return m_elements.GetCount() > 0 ? &m_elements[0] + m_elements.GetCount() : nullptr;
                }

                /// \brief Returns an enumerator that iterates through the group elements.
                Collections::Generic::IEnumeratorPtr<TElement> GetEnumerator() const override {
                    return m_elements.GetEnumerator();
                }

                using ElementType = TElement;
                using KeyType = TKey;
            };

        } // namespace Linq
    } // namespace System
} // namespace DotNetDupe

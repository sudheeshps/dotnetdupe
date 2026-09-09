#pragma once
#include <initializer_list>

/// \struct _init_list_with_indexer
/// \brief Helper wrapper allowing random-access bracket indexing over std::initializer_list.
/// \tparam T The element type in the initializer list.
template <class T>
struct _init_list_with_indexer {
    const std::initializer_list<T>& list; ///< Reference to the wrapped initializer list.

    /// \brief Constructs a wrapper over the provided initializer list.
    /// \param _list The initializer list to wrap.
    _init_list_with_indexer(const std::initializer_list<T>& _list)
        : list(_list) {
    }

    /// \brief Accesses the element at the specified index.
    /// \param index Zero-based element index.
    /// \return The element at the specified position.
    T operator[](unsigned int index) { return *(list.begin() + index); }
};

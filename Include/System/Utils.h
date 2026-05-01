#pragma once
template <class T>
struct _init_list_with_indexer {
    const std::initializer_list<T>& list;
    _init_list_with_indexer(const std::initializer_list<T>& _list)
        : list(_list) {
    }
    T operator[](unsigned int index) { return *(list.begin() + index); }
};

//
// Created by Pedro Jorge on 06-01-2025.
//
#include "shrink_wrapped_pimpl.h"

template<typename T>
pimpl<T>::pimpl(): impl {new T{}} {

}

template<typename T>
pimpl<T>::~pimpl() = default;

template<typename T>
template<typename... Args>
pimpl<T>::pimpl(Args &&... args): impl {new T{std::forward<Args>(args)...}} {

}


template<typename T>
T* pimpl<T>::operator->() {
    return impl.get();
}

template<typename T>
T& pimpl<T>::operator*() {
    return *impl.get();
}


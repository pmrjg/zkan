//
// Created by Pedro Jorge on 06-01-2025.
//

#pragma once
#include <memory>

template <typename T> class pimpl {
    std::unique_ptr<T> impl;
public:
    pimpl();
    ~pimpl();

    template <typename ...Args> pimpl(Args&& ...args);

    T* operator->();
    T& operator*();
};

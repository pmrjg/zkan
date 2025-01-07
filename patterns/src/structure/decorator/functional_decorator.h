//
// Created by Pedro Jorge on 07-01-2025.
//

#pragma once
#include <functional>
#include <string>
#include <utility>

template <typename> struct FunctionalDecorator;

template <typename R, typename... Args>
struct FunctionalDecorator<R(Args...)> {
    std::function<R(Args...)> func;
    std::string name;

    FunctionalDecorator(const std::function<R(Args...)> &func, std::string name) : func(func), name(std::move(name)) {}

    R operator()(Args... args) {
        std::cout << "Entering " << name << std::endl;
        R result = func(args...);
        std::cout << "Exiting " << name << std::endl;

        return result;
    }
};

template <typename R, typename... Args>
auto make_functional_decorator(R (*func)(Args...), const std::string& name) {
    return FunctionalDecorator<R(Args...)>(function<R(Args...)>(func), name);
}
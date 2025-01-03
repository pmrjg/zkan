//
// Created by pmrj on 01-01-2025.
//
#pragma once
#include <string>
#include <vector>
using namespace std;

enum class Color {red, green, blue};
enum class Size {small, medium, large};


struct Product {
    string name;
    Color color;
    Size size;
};

struct ProductFilter {
    vector<Product*> by_color(vector<Product*> products, Color c) {
        vector<Product*> result;

        for (auto& product : products) {
            if (product->color == c) result.push_back(product);
        }

        return result;

    };
};

template <typename T> struct Specification {
    virtual ~Specification() = default;

    virtual bool is_satisfied(T* item) = 0;
};

template <typename T> struct Filter {
    virtual ~Filter() = default;

    virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};


struct BetterFilter : Filter<Product> {
    vector<Product*> filter(vector<Product*> items, Specification<Product>& spec) override {
        vector<Product*> result;

        for (auto& item: items) {
            if (spec.is_satisfied(item)) result.push_back(item);
        }

        return result;
    }
};

struct ColorSpecification : Specification<Product> {
    Color color;

    explicit ColorSpecification(Color c) : color(c) {}

    bool is_satisfied(Product* item) override {
        return item->color == color;
    }
};
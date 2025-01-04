//
// Created by Pedro Jorge on 04-01-2025.
//

#pragma once
#include <map>
#include <memory>
#include <string>

enum class Importance {
    primary,
    secondary,
    tertiary
};

template <typename T, typename Key = std::string>
class Multiton {
public:
    static std::shared_ptr<T> get(const Key& key) {
        if (const auto it = instances.find(key); it != instances.end()) {
            return it->second;
        }

        auto instance = std::make_shared<T>();
        instances[key] = instance;
        return instance;
    }
protected:
    Multiton() = default;
    virtual ~Multiton() = default;
private:
    static std::map<Key, std::shared_ptr<T>> instances;
};

template <typename T, typename Key>
std::map<Key, std::shared_ptr<T>> Multiton<T, Key>::instances;
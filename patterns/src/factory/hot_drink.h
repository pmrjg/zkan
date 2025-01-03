//
// Created by Pedro Jorge on 03-01-2025.
//

#pragma once
#include <memory>
#include <map>
#include <functional>

using namespace std;

struct HotDrink {
    virtual ~HotDrink() = default;
    virtual void prepare(int volume) = 0;
};

struct Tea : HotDrink {
    void prepare(int volume) override {
        std::cout << "Take tea bag, boil water, pour milk and some " << volume << " lemon\n";
    }
};

struct Coffee : HotDrink {
    void prepare(int volume) override {
        std::cout << "Water and beans of " << volume << std::endl;
    }
};

struct HotDrinkFactory {
    virtual unique_ptr<HotDrink> make() const = 0;
};

struct TeaFactory : HotDrinkFactory {
    unique_ptr<HotDrink> make() const override {
        return std::make_unique<Tea>();
    }
};

struct CoffeeFactory : HotDrinkFactory {
    unique_ptr<HotDrink> make() const override {
        return std::make_unique<Coffee>();
    }
};

class DrinkFactory : HotDrinkFactory {
    map<string, unique_ptr<HotDrinkFactory>> factory_map;
public:
    DrinkFactory() {
        factory_map["coffee"] = make_unique<CoffeeFactory>();
        factory_map["tea"] = make_unique<TeaFactory>();
    }

    unique_ptr<HotDrink> make(const string &name) {
        auto drink = factory_map[name]->make();
        drink->prepare(200);
        return drink;
    }
};

class DrinkWithVolumeFactory {
    map<string, function<unique_ptr<HotDrink>()>> fact;

public:
    DrinkWithVolumeFactory() {
        fact["tea"] = [] {
            auto tea = make_unique<Tea>();
            tea->prepare(200);
            return tea;
        };
    }

    unique_ptr<HotDrink> make_drink(const string&name) {
        return fact[name]();
    }
};
//
// Created by Pedro Jorge on 06-01-2025.
//

#pragma once
#include <iostream>
#include <string>
#include <vector>

struct GraphicObject {
    virtual void draw() = 0;
};

struct Circle : GraphicObject {
    void draw() override {
        std::cout << "Circle" << std::endl;
    }
};

struct Group : GraphicObject {
    std::string name;
    std::vector<GraphicObject*> objects;

    Group(const std::string &name) : name(name) {}

    void draw() override {
        std::cout << "Group " << name.c_str() << "contains:" << std::endl;
        for (auto &&o: objects) o->draw();
    }
};
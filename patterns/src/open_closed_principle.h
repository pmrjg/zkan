//
// Created by pmrj on 01-01-2025.
//
#pragma once
#include <string>
using namespace std;

enum class Color {red, green, blue};
enum class Size {small, medium, large};


struct Product {
    string name;
    Color color;
    Size size;
};

//
// Created by Pedro Jorge on 03-01-2025.
//
#_USE_MATH_DEFINES
#pragma once


class Point {
    Point(float x, float y): x(x), y(y) {}
public:
    static Point new_cartesian(float x, float y) {
        return {x, y};
    }

    static Point new_polar(float r, float theta) {
        return {r * cos(theta), r * sin(theta)};
    }

    float x, y;
};
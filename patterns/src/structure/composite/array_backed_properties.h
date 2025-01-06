//
// Created by Pedro Jorge on 06-01-2025.
//
#pragma once

class Creature {
    enum Abilities {str, agl, intl, count};
    std::array<int, count> abilities;

public:
    [[nodiscard]] int get_strength() const { return abilities[str];}
    [[nodiscard]] int get_agility() const { return abilities[agl];}
    [[nodiscard]] int get_intelligence() const { return abilities[intl];}

    void set_strength(int s) { abilities[str] = s;}
    void set_agility(int a) { abilities[agl] = a;}
    void set_intelligence(int a) { abilities[intl] = a;}

    [[nodiscard]] int sum() const { return std::accumulate(abilities.begin(), abilities.end(), 0);}

    [[nodiscard]] double average() const { return sum() / static_cast<double>(count);}

    [[nodiscard]] int max() const {
        return *std::max_element(abilities.begin(), abilities.end());
    }
};
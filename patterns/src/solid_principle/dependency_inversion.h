//
// Created by pmrj on 02-01-2025.
//

#pragma once

enum class Relationship {
    parent,
    child,
    sibling,
};

struct Person {
    string name;
};

struct Relationships {
    /*vector<tuple<Person,Relationship, Person>> relations;

    void add_parent_and_child(const Person& parent, const Person& child) {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }*/
};





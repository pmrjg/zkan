//
// Created by Pedro Jorge on 03-01-2025.
//

#pragma once
#include <ostream>
#include <string>

struct Address {
    std::string street, city;
    int suite;

    Address(const std::string &street, const std::string &city, int suite) : street(street), city(city), suite(suite) {}

    Address(const Address & address): street(address.street),city(address.city), suite(address.suite) {}

    ~Address() {}

    friend std::ostream & operator<<(std::ostream &os, const Address &obj) {
        return os
               << "street: " << obj.street
               << " city: " << obj.city
               << " suite: " << obj.suite;
    }
};

struct Contact {
    std::string name;
    Address* address;

    Contact(const string &name, Address* address) : name(name), address(address) {}

    //Copy constructor
    Contact(const Contact &other): name{other.name}, address{new Address{*other.address}} {

    }

    ~Contact() {
        delete address;
    }

    friend std::ostream & operator<<(std::ostream &os, const Contact &obj) {
        return os
               << "name: " << obj.name
               << " address: " << obj.address;
    }
};

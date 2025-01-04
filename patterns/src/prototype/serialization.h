//
// Created by Pedro Jorge on 04-01-2025.
//

#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;
using namespace boost;

struct Address {
    string street, city;
    int suite;

    Address(const string &street, const string &city, int suite) : street(street), city(city), suite(suite) {}

    Address(const Address & address): street(address.street),city(address.city), suite(address.suite) {}

    ~Address() {}

    friend ostream & operator<<(ostream &os, const Address &obj) {
        return os
               << "street: " << obj.street
               << " city: " << obj.city
               << " suite: " << obj.suite;
    }
};

struct Contact {
    string name;
    Address* address;

    Contact(const string &name, Address* address) : name(name), address(address) {}

    //Copy constructor
    Contact(const Contact &other): name{other.name}, address{new Address{*other.address}} {

    }

    ~Contact() {
        delete address;
    }

    friend ostream & operator<<(ostream &os, const Contact &obj) {
        return os
               << "name: " << obj.name
               << " address: " << obj.address;
    }

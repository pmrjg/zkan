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

    Contact(const std::string &name, Address* address) : name(name), address(address) {}

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

struct EmployeeFactory {
  private:
    static unique_ptr<Contact> new_employee(const std::string &name, const int suite, const Contact& prototype){
      auto result = make_unique<Contact>(prototype);
      result->name = name;
      result->address->suite = suite;
      return result;
};
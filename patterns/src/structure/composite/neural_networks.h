//
// Created by Pedro Jorge on 06-01-2025.
//
#pragma once

#include <vector>
#include <ostream>

// CRTP
template <typename Self>
struct SomeNeurons {
    template <typename T> void connect_to(T& other) {
        for (Neuron& from : *static_cast<Self*>(this)) {
            for (Neuron& to: other) {
                from.out.push_back(&other);
                to.in.push_back(&from);
            }
        }
    }
};

struct Neuron: SomeNeurons<Neuron> {
    std::vector <Neuron*> in, out;
    unsigned int id;

    Neuron() {
        static int id{1};
        this->id = id++;
    }

    void connect_to(Neuron& other) {
        out.push_back(&other);
        other.in.push_back(this);
    }

    friend std::ostream &operator<<(std::ostream &os, const Neuron &n) {
        for (Neuron* nn : n.in) {
            os << nn->id << "\t-->\t[" << n.id << "]\n";
        }

        for (Neuron* nn : n.out) {
            os << "[" << n.id << "]\t-->\t" << nn->id << "\n";
        }

        return os;
    }

    Neuron* begin() {
        return this;
    }

    Neuron* end(){return this+1;}
};

struct NeuronLayer : std::vector<Neuron>, SomeNeurons<NeuronLayer> {
    explicit NeuronLayer(int count) {
        while (count --> 0) emplace_back(Neuron{});
    }

    friend std::ostream &operator<<(std::ostream &os, const NeuronLayer &obj) {
        for (auto & n: obj) os << n;
        return os;
    }
};




#pragma once
#include <random>

constexpr double PI{ 3.141592653589793238462643 };

inline double rand01() {
    //static std::random_device rd;
    //static std::default_random_engine generator(rd());
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distr(0.0, 1.0);
    return distr(generator);
}
#pragma once
#include <random>
#include <ctime>
#include <iostream>

constexpr double PI{ 3.141592653589793238462643 };
constexpr double PI_RECIPROCAL{ 1.0 / PI };

enum PALETTE : int {
    YELLOW = 0xffe815,
    BLUE = 0x5654f7,
    PURPLE = 0x64499a,
    RED = 0xF72349,
    ORANGE = 0xff5c29,
    GREEN = 0x50BF94,
    LIGHT_GREEN = 0xadce33,
    WHITE = 0xffffff,
    PINK = 0xcd97f9,
    BLACK = 0x000000,
    GRAY = 0x444444,
};

inline double rand01() {
    //static std::random_device rd;
    //static std::default_random_engine generator(rd());
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distr(0.0, 1.0);
    return distr(generator);
}

inline double rand11() {
    //static std::random_device rd;
    //static std::default_random_engine generator(rd());
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distr(0.0001, 1.0);
    return distr(generator);
}

inline void timeInfo(clock_t globalTimeStart) {
    clock_t globalTimeEnd = clock();
    clock_t seconds{ globalTimeEnd - globalTimeStart };
    std::cout << "\nTotal time spending: "
        << seconds / 60 / 1000 << "m "
        << seconds / 1000 % 60 + (seconds % 1000 / 1000.0) << "s.\n\n";
}
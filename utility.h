#pragma once
#include <random>

constexpr double PI{ 3.141592653589793238462643 };

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
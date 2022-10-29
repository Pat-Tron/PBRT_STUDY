#pragma once
#include <iostream>

struct Color {
    double R{ 0.0 }, G{ 0.0 }, B{ 0.0 };
    static constexpr double gammaCorrection{ 1.0 / 2.2 };

    Color() = default;
    Color(double r, double g, double b) : R(r), G(g), B(b) {}
    Color operator+(const Color &c) const { return Color(R + c.R, G + c.G, B + c.B); }
    Color operator-(const Color &c) const { return Color(R - c.R, G - c.G, B - c.B); }
    Color operator*(const Color &c) const { return Color(R * c.R, G * c.G, B * c.B); }
    Color operator/(const Color &c) const { return Color(R / c.R, G / c.G, B / c.B); }
    Color &operator+=(const Color &c) { R += c.R; G += c.G; B += c.B; return *this; }
    Color &operator-=(const Color &c) { R -= c.R; G -= c.G; B -= c.B; return *this; }
    Color &operator*=(const Color &c) { R *= c.R; G *= c.G; B *= c.B; return *this; }
    Color &operator/=(const Color &c) { R /= c.R; G /= c.G; B /= c.B; return *this; }

    // Gamma encoding to sRGB
    //int d2i(const double &channel) const {
    //    return static_cast<int>(255.99 * std::pow(channel, gammaCorrection));
    //}
    int d2i(const double &channel) const { return static_cast<int>(255.99 * channel); }
    char d2c(const double &channel) const { return static_cast<char>(d2i(channel)); }

    friend std::ostream &operator<<(std::ostream &os, const Color &c);
};

inline std::ostream &operator<<(std::ostream &os, const Color &c) {
    os << c.d2i(c.R) << ' ' << c.d2i(c.G) << ' ' << c.d2i(c.B) << std::endl;
    return os;
}
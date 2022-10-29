// Start with RayTracing in One Weekend.

#define QOI_IMPLEMENTATION
#include "qoi.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>

struct Color {
    double R{ 0.0 }, G{ 0.0 }, B{ 0.0 };
    static constexpr double gammaCorrection{ 1.0 / 2.2 };

    char d2c(const double &channel) const { return static_cast<char>(d2i(channel)); }
    // Gamma encoding to sRGB
    //int d2i(const double &channel) const {
    //    return static_cast<int>(255.99 * std::pow(channel, gammaCorrection));
    //}
    int d2i(const double &channel) const { return static_cast<int>(255.99 * channel); }

    friend std::ostream &operator<<(std::ostream &os, const Color &c) {
        os << c.d2i(c.R) << ' ' << c.d2i(c.G) << ' ' << c.d2i(c.B) << std::endl;
        return os;
    }
};

constexpr bool PPM{ false };
constexpr bool QOI{ true };

int main() {
    constexpr int width = 500, height = 500;

    std::vector<std::vector<Color>> pixels(height, std::vector<Color>(width));

    std::cout << "Rendering start." << std::endl;
    for (int row{ 0 }; row < height; ++row) {
        std::cout << "\rRendering ROW " << row + 1 << " of " << height << " .";
        for (int col{ 0 }; col < width; ++col) {
            Color &pxl = pixels[row][col];
            pxl.R = 1.0 * row / height;
            pxl.G = 1.0 * col / width;
            pxl.B = 1.0;
        }
    }
    std::cout << "Rendering finished" << std::endl;
    
    if (PPM) {
        std::cout << "Start writing PPM file." << std::endl;
        std::ofstream picOut;
        picOut.open("render_output/image.ppm", 'w');
        picOut << "P3\n" << width << " " << height << "\n255\n";
        for (int row{ 0 }; row < height; ++row) {
            std::cout << "\rWriting ROW " << row + 1 << " of " << height << " .";
            for (int col{ 0 }; col < width; ++col) {
                const Color &pxl = pixels[row][col];
                picOut << pxl;
            }
        }
        picOut.close();
        std::cout << "\nPPM output finished." << std::endl;
    }

    if (QOI) {
        std::cout << "Start writing QOI file." << std::endl;
        char *rgb_pixels = new char[width * height * 3];
        for (int row{ 0 }; row < height; ++row) {
            std::cout << "\rConvert double to char in ROW " << row + 1 << " of " << height << " .";
            for (int col{ 0 }; col < width; ++col) {
                const Color &pxl = pixels[row][col];
                rgb_pixels[col * 3 + row * width * 3] = pxl.d2c(pxl.R);
                rgb_pixels[col * 3 + 1 + row * width * 3] = pxl.d2c(pxl.G);
                rgb_pixels[col * 3 + 2 + row * width * 3] = pxl.d2c(pxl.B);
            }
        }
        std::cout << "\nConverting finished." << std::endl;

        qoi_desc desc;
        desc.width = width;
        desc.height = height;
        desc.channels = 3;
        desc.colorspace = QOI_LINEAR;
        qoi_write("render_output/image.qoi", rgb_pixels, &desc);
        delete[]rgb_pixels;
        std::cout << "QOI output finished." << std::endl;
        return 0;
    }
}
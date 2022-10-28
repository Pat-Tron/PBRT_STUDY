// Start with RayTracing in One Weekend.

//#define QOI_IMPLEMENTATION
//#include "qoi.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

struct Color {
    double R{ 0.0 }, G{ 0.0 }, B{ 0.0 };
private:
    int d2i(const double &channel) const { return static_cast<int>(255.99 * channel); }

    friend std::ostream &operator<<(std::ostream &os, const Color &c) {
        os << c.d2i(c.R) << ' ' << c.d2i(c.G) << ' ' << c.d2i(c.B) << std::endl;
        return os;
    }
};

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

    std::cout << "Rendering finished, start writing pic file." << std::endl;

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
    std::cout << "\nOutput finished." << std::endl;
    return 0;
}
// Start with RayTracing in One Weekend.

#define QOI_IMPLEMENTATION
#include "qoi.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "Color.h"


int main() {

    constexpr bool PPM{ false };
    constexpr bool QOI{ true };
    constexpr int width = 500, height = 500;
    std::vector<std::vector<Color>> pixels(height, std::vector<Color>(width));
    

    // Rendering loop
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
    

    // Output
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
            for (int col{ 0 }; col < width; ++col) {
                const Color &pxl = pixels[row][col];
                rgb_pixels[row * width * 3 + col * 3] = pxl.d2c(pxl.R);
                rgb_pixels[row * width * 3 + col * 3 + 1] = pxl.d2c(pxl.G);
                rgb_pixels[row * width * 3 + col * 3 + 2] = pxl.d2c(pxl.B);
            }
        }
        std::cout << "\nConverting finished." << std::endl;

        qoi_desc desc;
        desc.width = width;
        desc.height = height;
        desc.channels = 3;
        desc.colorspace = QOI_SRGB;
        qoi_write("render_output/image.qoi", rgb_pixels, &desc);
        delete[]rgb_pixels;
        std::cout << "QOI output finished." << std::endl;
        return 0;
    }
}
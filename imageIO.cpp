#define QOI_IMPLEMENTATION
#include "qoi.h"
#include <iostream>
#include <fstream>
#include "imageIO.h"

void outputPic(
    const std::string &filename,
    const PIC_FORMAT &f,
    const std::vector<std::vector<Color>> &pixels
) {
    int height{ static_cast<int>(pixels.size()) };
    int width{ static_cast<int>(pixels[0].size()) };
    
    if (f == PIC_FORMAT::PPM) {
        std::cout << "Start writing PPM file." << std::endl;
        std::ofstream picOut;
        picOut.open(filename + ".ppm", 'w');
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

    if (f == PIC_FORMAT::QOI) {
        std::cout << "Write QOI file." << std::endl;

        // Serialization
        char *rgb_pixels = new char[width * height * 3];
        for (int row{ 0 }; row < height; ++row) {
            for (int col{ 0 }; col < width; ++col) {
                const Color &pxl = pixels[row][col];
                rgb_pixels[row * width * 3 + col * 3] = pxl.d2c(pxl.R);
                rgb_pixels[row * width * 3 + col * 3 + 1] = pxl.d2c(pxl.G);
                rgb_pixels[row * width * 3 + col * 3 + 2] = pxl.d2c(pxl.B);
            }
        }

        qoi_desc desc;
        desc.width = width;
        desc.height = height;
        desc.channels = 3;
        desc.colorspace = QOI_SRGB;
        qoi_write((filename + ".qoi").c_str(), rgb_pixels, &desc);
        delete[]rgb_pixels;
    }
}

void inputQOI(const std::string &filename, std::vector<std::vector<Color>> &pixels) {
    qoi_desc desc;
    void *rawData = qoi_read((filename + ".qoi").c_str(), &desc, 3);
    unsigned char *pixelData = static_cast<unsigned char*>(rawData);
    pixels = std::vector<std::vector<Color>>(desc.height, std::vector<Color>(desc.width));

    for (int row{ 0 }; row < desc.height; ++row) {
        for (int col{ 0 }; col < desc.width; ++col) {
            pixels[row][col] = Color(
                pixelData[row * desc.width * 3 + col * 3] / 255.99,
                pixelData[row * desc.width * 3 + col * 3 + 1] / 255.99,
                pixelData[row * desc.width * 3 + col * 3 + 2] / 255.99
            );
        }
    }
    free(rawData);
}
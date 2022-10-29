// Start with RayTracing in One Weekend.

#include "Ray.h"
#include "Color.h"
#include "Output.h"

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
    
    outputPic("render_output / image.ppm", width, height, PIC_FORMAT::QOI, pixels);
    

    return 0;
}
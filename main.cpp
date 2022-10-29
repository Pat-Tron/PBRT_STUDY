// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"

int main() {

    Camera camera{ 1000, 800, 3.0, 2.0 };
    
    // Rendering loop
    std::cout << "Rendering start." << std::endl;
    std::vector<std::vector<Color>> pixels(camera.height, std::vector<Color>(camera.width));
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
    outputPic("render_output/image.ppm", width, height, PIC_FORMAT::QOI, pixels);
    return 0;
}
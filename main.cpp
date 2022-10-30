// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"

inline Color background(Ray ray) {
    return ray.direction.getUnit() * 0.5 + Vec3(0.5, 0.5, 0.5);
}

int main() {

    Camera camera{ 1000, 800, 0.1, 2.0 };
    
    // Rendering loop
    std::cout << "Rendering start." << std::endl;
    std::vector<std::vector<Color>> pixels(camera.height, std::vector<Color>(camera.width));

    for (int row{ 0 }; row < camera.height; ++row) {
        std::cout << "\rRendering ROW " << row + 1 << " of " << camera.height << " .";
        for (int col{ 0 }; col < camera.width; ++col) {
            pixels[row][col] = background(camera.getRay(col, row));
        }
    }
    std::cout << "Rendering finished" << std::endl;
    
    // Output
    outputPic("render_output/image", camera.width, camera.height, PIC_FORMAT::QOI, pixels);
    return 0;
}
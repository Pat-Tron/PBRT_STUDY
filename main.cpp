// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"
#include "Shape.h"


int main() {

    Camera camera{ 500, 300, 2, 2.0 };
    camera.antialiasing = 5;

    Primitives primitives{ std::vector<std::shared_ptr<const Primitive>>{
        std::make_shared<const Sphere>(Sphere(1.0,   Vec3(0.0, 0.0, 6.0), 0xff2222)),  // small
        std::make_shared<const Sphere>(Sphere(100.0, Vec3(0.0, -101.0, 6.0), 0x22dd22)),  // ground
    } };
    
    // Rendering loop
    std::cout << "Rendering start." << std::endl;
    std::vector<std::vector<Color>> pixels(camera.height, std::vector<Color>(camera.width));
    double uStep{ 1.0 / camera.width / camera.antialiasing };
    double vStep{ 1.0 / camera.height / camera.antialiasing };

    for (int row{ 0 }; row < camera.height; ++row) {
        std::cout << "\rRendering ROW " << row + 1 << " of " << camera.height << " .";
        for (int col{ 0 }; col < camera.width; ++col) {
            double u{ 1.0 * col / camera.width };
            double v{ 1.0 * row / camera.height };
            
            for (int ui{ 0 }; ui < camera.antialiasing; ++ui) {
                for (int vi{ 0 }; vi < camera.antialiasing; ++vi) {
                    Ray ray{ camera.getRay(u + ui * uStep, v + vi * vStep) };
                    HitRec rec;
                    Color result;
                    if (primitives.hit(ray, 0.1, 1e10, rec)) result = rec.normal * 0.5 + Vec3(0.5, 0.5, 0.5);
                    else result = background(ray);
                    pixels[row][col] += result;
                }
            }
            
            pixels[row][col] /= camera.antialiasing * camera.antialiasing;
        }
    }
    std::cout << "Rendering finished" << std::endl;
    
    // Output
    outputPic("render_output/image", camera.width, camera.height, PIC_FORMAT::QOI, pixels);
    return 0;
}
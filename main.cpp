// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"
#include "Shape.h"

constexpr int MAX_DEPTH{ 5 };

Color render(const Ray &ray, const Primitives &prims) {
    HitRec rec;
    static int depth{ 0 };
    if (prims.hit(ray, 0.0000001, 1e10, rec)) {
        if (depth < MAX_DEPTH) {
            ++depth;
            return render(rec.mat->scatter(ray, rec), prims) * rec.mat->albedo * rec.mat->reflectance;
        } else {
            depth = 0;
            return rec.mat->albedo;
        }
    } else {
        depth = 0;
        return background(ray);
    }
}

int main() {
    Camera camera{ 1000, 750, 2, 2.0 };
    camera.antialiasing = 1;

    Lambertian diffuseGray(0x5E5959);
    Lambertian diffuseYellow(0xF6DC85);
    Metal shinyRed(0xF72349);
    Metal matteGreen(0x50BF94, 0.2);

    Primitives primitives{ std::vector<std::shared_ptr<const Primitive>>{
        std::make_shared<const Sphere>(Sphere(50.0, Vec3(0.0, -51.0, 6.0),  &diffuseGray)),  // ground
        std::make_shared<const Sphere>(Sphere(1.0,  Vec3(-1.5, -0.04, 4.5), &diffuseYellow)),  // l small
        std::make_shared<const Sphere>(Sphere(1.0,  Vec3(0.0, 0.0, 6.0),    &shinyRed)),  // m small
        std::make_shared<const Sphere>(Sphere(1.0,  Vec3(1.5, -0.04, 7.5),  &matteGreen)),  // r small
        std::make_shared<const Sphere>(Sphere(0.5,  Vec3(0.7, -0.52, 4.0),  &diffuseGray)),  // front small
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
                    pixels[row][col] += render(camera.getRay(u + ui * uStep, v + vi * vStep), primitives);
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
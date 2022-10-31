// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"
#include "Shape.h"

#include <random>
std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_real_distribution<double> distr(0.0, 1.0);
inline double rand01() { return distr(generator); }
constexpr double PI{ 3.141592653589793238462643 };

constexpr int MAX_DEPTH{ 5 };
constexpr double REFLECTANCE{ 1.0 };

Vec3 randomSampleInHemiSphere(Vec3 normal) {
    // ∞Î«Ú√Êæ˘‘»»ˆµ„ https://zhuanlan.zhihu.com/p/340929847
    float phi = rand01() * 2.0 * PI;
    float theta = acos(1.0 - rand01());
    Vec3 pos = Vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));

    // quaternion rotation
    Vec3 up{ 0.0, 1.0, 0.0 };
    double angle{ acos(normal * up) };
    Vec3 axis{ (up ^ normal).normalized() };
    double c{ cos(angle) };
    return pos * c + axis * (1 - c) * (axis * pos) + (axis ^ pos) * sin(angle);
}

Color render(const Ray &ray, const Primitives &prims) {
    HitRec rec;
    static int depth{ 0 };
    if (prims.hit(ray, 0.0000001, 1e10, rec)) {
        if (depth < MAX_DEPTH) {
            Vec3 newDir{ randomSampleInHemiSphere(rec.normal) };
            ++depth;
            return render(Ray(rec.p, newDir), prims) * rec.albedo * REFLECTANCE;
        } else {
            depth = 0;
            return rec.albedo;
        }
    } else {
        depth = 0;
        return background(ray);
    }
}

int main() {

    Camera camera{ 1000, 700, 2, 2.0 };
    camera.antialiasing = 10;

    Primitives primitives{ std::vector<std::shared_ptr<const Primitive>>{
        std::make_shared<const Sphere>(Sphere(50.0, Vec3(0.0, -51.0, 6.0),  0x5E5959)),  // ground
        std::make_shared<const Sphere>(Sphere(1.0,  Vec3(-1.5, -0.04, 4.5), 0xF72349)),  // l small
        std::make_shared<const Sphere>(Sphere(1.0,  Vec3(0.0, 0.0, 6.0),    0xF6DC85)),  // m small
        std::make_shared<const Sphere>(Sphere(1.0,  Vec3(1.5, -0.04, 7.5),  0x50BF94)),  // r small
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
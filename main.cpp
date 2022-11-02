// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"
#include "Shape.h"

int main() {
    Camera camera(P1K, 0.5);
    camera.position = Vec3(20, 4.5, -2);
    camera.faceAt(Vec3(0.0, 0.8, 0.0));
    camera.focal = 3.0 ;
    camera.antialiasing = 1 ;
    camera.maxDepth = 10;

    Lambertian diffuseGround(0x5654f7);
    Lambertian diffuseYellow(0xffe815);
    Metal shinyRed(0xF72349);
    Metal matteGreen(0x50BF94, 0.1);
    Dielectric glass(0xffffFF, 1.4); // 0x2222DD

    double squareSize{ 7.0 };
    Vec3 corners[4]{
        Vec3(squareSize * 0.5, 0.0, squareSize * 0.5),
        Vec3(-squareSize * 0.5, 0.0, squareSize * 0.5),
        Vec3(-squareSize * 0.5, 0.0, -squareSize * 0.5),
        Vec3(squareSize * 0.5, 0.0, -squareSize * 0.5)
    };
    Primitives primitives{ std::vector<std::shared_ptr<const Primitive>>{
        std::make_shared<Triangle>(Triangle(corners[0], corners[3], corners[1], &diffuseGround)), // ground
        std::make_shared<Triangle>(Triangle(corners[3], corners[2], corners[1], &diffuseGround)), // ground
        std::make_shared<Sphere>(Sphere(1.0,  Vec3(-1.5, 1, 1.5), &diffuseYellow)),  // l small
        std::make_shared<Sphere>(Sphere(1.0,  Vec3(0.0, 1, 0.0), &shinyRed)),  // m small
        std::make_shared<Sphere>(Sphere(1.0,  Vec3(1.5, 1, -1.5), &matteGreen)),  // r small
        std::make_shared<Sphere>(Sphere(0.7,  Vec3(2, 0.7, 0.8), &glass)),  // front small
    } };

    camera.randerLoop(primitives);

    // Output
    outputPic("image", PIC_FORMAT::QOI, camera.pixels);
    return 0;
}
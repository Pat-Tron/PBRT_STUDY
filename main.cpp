// Start with RayTracing in One Weekend.

#include "Output.h"
#include "Camera.h"
#include "Shape.h"
#include <ctime>

int main() {
    std::time_t globalTimeStart = std::time(nullptr);

    Camera camera(P1K, 0.5);
    camera.position = Vec3(20, 4.5, -2);
    camera.focal = 3.0 ;
    camera.antialiasing = 5;
    camera.maxDepth = 5;
    camera.aperture = 0.2;
    camera.defocusScale = 0.9;
    camera.faceAt(Vec3(0.0, 0.8, 0.0));
    camera.motionBlur = true;

    AABB::padding = 0.00001;
    Primitive::timeStart = camera.timeStart;
    Primitive::timeEnd = camera.timeEnd;

    Lambertian diffuseGround(0x5654f7);
    Lambertian diffuseYellow(0xffe815);
    Lambertian diffusePurple(0x64499a);
    Metal shinyRed(0xF72349);
    Metal shinyOrange(0xff5c29);
    Metal matteGreen(0x50BF94, 0.1);
    Metal matteGreen2(0xadce33, 0.2);
    Dielectric glass(0xffffFF, 1.4); // 0x2222DD
    Dielectric glass2(0xddddFF, 1.3); // 0x2222DD

    double squareSize{ 7.0 };
    Vec3 corners[4]{
        Vec3(squareSize * 0.5, 0.0, squareSize * 0.5),
        Vec3(-squareSize * 0.5, 0.0, squareSize * 0.5),
        Vec3(-squareSize * 0.5, 0.0, -squareSize * 0.5),
        Vec3(squareSize * 0.5, 0.0, -squareSize * 0.5)
    };
    std::vector<primPointer> primitives{
        std::make_shared<Triangle>(Triangle(corners[0], corners[3], corners[1], &diffuseGround)), // ground
        std::make_shared<Triangle>(Triangle(corners[3], corners[2], corners[1], &diffuseGround)), // ground
        std::make_shared<Sphere>(Sphere(1.0,  Vec3(-1.5, 1, 1.5), &diffuseYellow)),  // l
        std::make_shared<Sphere>(Sphere(1.0,  Vec3(0.0, 1, 0.0), &shinyRed)),  // m
        std::make_shared<Sphere>(Sphere(1.0,  Vec3(1.5, 1, -1.5), &matteGreen)),  // r
        std::make_shared<Sphere>(Sphere(0.7,  Vec3(2, 0.7, 0.8), &glass)),  // front small

        std::make_shared<Sphere>(Sphere(0.2,  Vec3(2.5, 0.2, -1.8), &diffusePurple, Vec3(0, 0, 3))),
        std::make_shared<Sphere>(Sphere(0.3,  Vec3(2, 0.3, -0.5), &glass2)),
        std::make_shared<Sphere>(Sphere(0.4,  Vec3(-3, 0.4, 3), &matteGreen2)),
        std::make_shared<Sphere>(Sphere(0.4,  Vec3(0.8, 0.4, -2.7), &shinyOrange))
    };

    // Random balls
    for (int i{ 0 }; i < 0; ++i) {
        double randX{ (rand01() - 0.5) * squareSize };
        double randZ{ (rand01() - 0.5) * squareSize };
        double randR{ rand01() * 0.3 + 0.2 };
        primitives.push_back(
            std::make_shared<Sphere>(Sphere(randR, Vec3(randX, randR, randZ), &diffusePurple))
        );
    }
    
    camera.randerLoop(primitives);

    std::time_t globalTimeEnd = std::time(nullptr);
    time_t seconds{ globalTimeEnd - globalTimeStart };
    std::cout << "\nTotal time spending: " << seconds / 60 << "m " << seconds % 60 << "s.\n\n";

    // Output
    outputPic("image", PIC_FORMAT::QOI, camera.pixels);
    return 0;
}
// Start with RayTracing in One Weekend.

#include "imageIO.h"
#include "Camera.h"
#include "Geometry.h"
#include "Transformation.h"

int main() {
    clock_t globalTimeStart = clock();

    Camera camera(P1K, 0.5);
    camera.position = Vec3(0, 3, 40);
    camera.focal = 2.0;
    camera.antialiasing = 5;
    camera.maxDepth = 5;
    //camera.aperture = 0.2;
    camera.defocusScale = 0.9;
    camera.faceAt = Vec3(0.0, 5, 0.0);
    //camera.NO_BG = true;
    //camera.motionBlur = true;
    

    camera.antialiasing = 1; camera.maxDepth = 1;
    
    using TF = Transformation;
    Geometry geos = Geometry() +
        Square(Lambertian(GRAY), 10) * TF(TF::T, 0, 0, 0) +
        Square(Lambertian(PINK), 10) * TF(TF::T, 0, 2, 0) +
        Square(Lambertian(YELLOW), 10) * TF(TF::T, 0, 4, 0);
        //PrimBall(DiffuseLight(Color(100.0)), 0.2, Vec3(0, 4, 0));
    
    auto pixels = camera.randerLoop(geos.prims);
    
    outputPic("image", PIC_FORMAT::QOI, pixels);

    timeInfo(globalTimeStart);
    return 0;
}
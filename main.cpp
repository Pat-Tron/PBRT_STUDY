// Start with RayTracing in One Weekend.

#include "imageIO.h"
#include "Camera.h"
#include "Geometry.h"
#include "Transformation.h"

int main() {
    clock_t globalTimeStart = clock();

    Camera camera(1080, 1080);
    camera.position = Vec3(0, 5, 25.);
    camera.focal = 2.0;
    camera.antialiasing = 3;
    camera.maxDepth = 10;
    //camera.aperture = 0.2;
    camera.defocusScale = 0.9;
    camera.faceAt = Vec3(0.0, 5, 0.0);
    camera.NO_BG = true;
    //camera.motionBlur = true;
    

    camera.antialiasing = 30; camera.maxDepth = 20;
    
    using TF = Transformation;
    
    Geometry geos = Geometry() +
        PrimBall(Metal((Color(PURPLE) * 1.5).clamp(), 0.7), 3) * TF(TF::T, 0, 3, 0) +
        PrimBall(DiffuseLight(Color(WHITE)  * 6), 1) * TF(TF::T, 0, 10, 0) +
        PrimBall(DiffuseLight(Color(BLUE)   * 6), 1) * TF(TF::T, -4, 10, 0) +
        PrimBall(DiffuseLight(Color(ORANGE) * 6), 1) * TF(TF::T, 4, 10, 0) +
        Square(DiffuseLight(Color(GREEN)    * 2.5), 10) * TF(TF::T, 0, -5, 0) +
        VolumeGeo(15.0, 15.0, 15.0, 0.1, WHITE) * TF(TF::T, 0, 5.5, 0);

    auto pixels = camera.randerLoop(geos.prims);
    
    outputPic("image", PIC_FORMAT::QOI, pixels);
    timeInfo(globalTimeStart);
    return 0;
}
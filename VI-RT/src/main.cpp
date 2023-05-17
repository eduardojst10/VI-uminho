#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/StandardRenderer.hpp"
#include "Image/ImagePPM.hpp"
#include "Shader/AmbientShader.hpp"
#include "Shader/WhittedShader.hpp"
#include "Shader/DistributedShader.hpp"
#include "Shader/PathTracerShader.hpp"
#include "Light/AmbientLight.hpp"
#include "Light/PointLight.hpp"
#include "Light/AreaLight.hpp"


void createAreaLights(Scene &scene){
    // Define the power and normal vector for all lights.
    RGB power(0.15f, 0.15f, 0.15f);
    RGB powerCenter(0.65f, 0.65f, 0.65f);
    Vector n(0, -1, 0);

    // Define the y-coordinate for all lights (ceiling height).
    float y = 548.0f;

    // Define the width and depth of each light source.
    float lightWidth = 556.0f / 3.0f;
    float lightDepth = 559.2f / 3.0f;

    // Define the offsets to center the lights in each grid cell.
    float offsetX = lightWidth / 2.0f;
    float offsetZ = lightDepth / 2.0f;


    
    // Loop over the grid cells.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            // Only create lights at the corners and the center of the grid.
            RGB p;
            if ((i == 1 && j == 1) || (i != 1 && j != 1)) {
                if(i == 1 && j == 1){
                    p = powerCenter;
                }else p = power;
                // Calculate the center of the current grid cell.
                float centerX = (i + 0.5f) * lightWidth;
                float centerZ = (j + 0.5f) * lightDepth;

                // Define the vertices of the first triangle (bottom-left half of the square).
                Point v1(centerX - offsetX, y, centerZ - offsetZ);
                Point v2(centerX + offsetX, y, centerZ - offsetZ);
                Point v3(centerX - offsetX, y, centerZ + offsetZ);
                AreaLight *al1 = new AreaLight(p, v1, v2, v3, n);
                scene.lights.push_back(al1);
                scene.numLights++;

                // Define the vertices of the second triangle (top-right half of the square).
                Point v4(centerX + offsetX, y, centerZ + offsetZ);
                AreaLight *al2 = new AreaLight(p, v4, v3, v2, n);
                scene.lights.push_back(al2);
                scene.numLights++;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;

    //change for specific path in local machine
    std::string my_path = "C:/Users/USER/Documents/GitHub/VI-uminho/VI-RT/src/Scene/tinyobjloader/models/cornell_box_VI.obj";

    //AmbientLight *ambient = new AmbientLight(RGB(0.05,0.05,0.05));
    //scene.lights.push_back(ambient);
    //scene.numLights++;

    //PointLight *pl1 = new PointLight(RGB(0.65,0.65,0.65),Point(288,508,282));
    //scene.lights.push_back(pl1);
    //scene.numLights++;

    // AreaLight properties
    RGB power(0.65f,0.65f,0.65f);
    Point v1(343.0, 548.0, 227.0), v2(343.0, 548.0, 332.0), v3(213.0, 548.0, 332.0);
    // assumimos y-axis is up).
    Vector n(0,-1,0);
    AreaLight *al = new AreaLight(power, v1, v2, v3, n);
    scene.lights.push_back(al);
    scene.numLights++;
    //createAreaLights(scene);
    success = scene.Load(my_path); 

    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load Success!:-)\n";
    scene.printSummary();
    
    // Image resolution
    const int W = 1024;
    const int H = 1024;

    img = new ImagePPM(W,H); 
    // Camera parameters 
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,1,0};
    const float fovW = M_PI/2, fovH = fovW * (H/W);
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH); 

    // create the shader
    RGB background(0.0f, 0.0f, 1.0f);
    shd = new PathTracerShader(&scene, background);


    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);
    //std::cout << "- Before Rendering!" << std::endl;
    
    // render
    myRender.Render();
    img->Save("C:/Users/USER/Documents/GitHub/VI-uminho/VI-RT/cornell_box.ppm");

    std::cout << "That's all!" << std::endl;
    return 0;
}

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/StandardRenderer.hpp"
#include "Image/ImagePPM.hpp"
#include "Shader/AmbientShader.hpp"
#include "Light/AmbientLight.hpp"


int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    Shader *shd;
    bool success;

    //5 faces 
    //change for specific path in local machine
    std::string my_path = "C:/Users/USER/Documents/GitHub/VI-uminho/VI-RT/src/Scene/tinyobjloader/models/cornell_box2.obj";

    success = scene.Load(my_path); 

    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load Success!:-)\n";

    scene.printSummary();
    std::cout << std::endl;
    
    // Image resolution
    const int W = 1024;
    const int H = 1024;
    
    AmbientLight ambient = AmbientLight(RGB(0.9,0.9,0.9));
    scene.lights.push_back(&ambient);
    scene.numLights++;

    img = new ImagePPM(W,H);
    
    // Camera parameters // Testar isto
    const Point Eye ={280,275,-330}, At={280,265,0};
    const Vector Up={0,-1,0};
    const float fovW = M_PI/2, fovH = fovW * (H/W);
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH); 
    // create the shader
    RGB background(0.05, 0.05, 0.55);
    shd = new AmbientShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);
    // render
    //std::cout << "Before Rendering!" << std::endl;

    myRender.Render();
    img->Save("C:/Users/USER/Documents/GitHub/VI-uminho/VI-RT/fgs.ppm");
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}

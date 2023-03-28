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
    std::string my_path = "C:/Users/USER/Documents/GitHub/VI-uminho/VI-RT/src/Scene/tinyobjloader/models/cornell_box.obj";

    success = scene.Load(my_path); 

    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load Success!! :-)\n";
    std::cout << std::endl;

    scene.printSummary();
    std::cout << std::endl;
    
    // Image resolution
    const int W= 640;
    const int H= 480;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters // Testar isto
    const Point Eye ={0,0,0}, At={0,0,1};
    const Vector Up={0,1,0};
    const float fovW = 3.14f/3.f, fovH = 3.14f/3.f;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH); 
    // create the shader
    RGB background(0.05, 0.05, 0.55);
    shd = new AmbientShader(&scene, background);
    // declare the renderer
    StandardRenderer myRender (cam, &scene, img, shd);
    // render
    //std::cout << "Before Rendering!" << std::endl;

    myRender.Render();
   

    img->Save("C:/Users/USER/Documents/GitHub/VI-uminho/VI-RT/MyImage.ppm");
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}

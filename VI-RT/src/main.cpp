#include <iostream>
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Renderer/renderer.hpp"
#include "Image/ImagePPM.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *img;    // Image
    bool success;

    //5 faces 
    std::cout << "Estamos aqui :o\n";  
    std::string my_path = "Scene/tinyobjloader/models/cornell_box.obj";

    success = scene.Load(my_path);
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }        
    std::cout << "Scene Load: SUCCESS!! :-)\n";
    scene.printSummary();
    std::cout << std::endl;
    
        // Image resolution
    const int W= 640;
    const int H= 480;
    
    img = new ImagePPM(W,H);
    
    // Camera parameters
    const Point Eye ={0,0,0}, At={0,0,1};
    const Vector Up={0,1,0};
    const float fovW = 3.14f/3.f, fovH = 3.14f/3.f;
    cam = new Perspective(Eye, At, Up, W, H, fovW, fovH);
    // declare the renderer
    Renderer myRender (&cam, &scene);
    // render
    myRender.Render();
    // save the image
    img->Save("MyImage.ppm");
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}

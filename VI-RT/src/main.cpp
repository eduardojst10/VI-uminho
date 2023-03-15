//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "Scene/scene.hpp"
#include "Camera/perspective.hpp"
#include "Renderer/renderer.hpp"

int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective cam(640, 480);  // image resolution
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
    
    // declare the renderer
    Renderer myRender (&cam, &scene);
    // render
    myRender.Render();
    // save the image
    cam.SaveImage();
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}

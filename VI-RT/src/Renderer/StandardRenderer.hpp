#ifndef StandardRenderer_hpp
#define StandardRenderer_hpp

#include "renderer.hpp"
#include <cstdlib>

class StandardRenderer: public Renderer {
public:
    StandardRenderer (Camera *cam, Scene * scene, Image * img, Shader *shd): Renderer(cam, scene, img, shd) {}
    void Render ();
};

#endif /* StandardRenderer_hpp */
#ifndef renderer_hpp
#define renderer_hpp

#include "../Camera/camera.hpp"
#include "../Scene/scene.hpp"
#include "../Image/image.hpp"

class Renderer {
protected:
    Camera *cam;
    Scene *scene;
    Image * img;
public:
    Renderer (Camera *cam, Scene * scene, Image * img): cam(cam), scene(scene), img(img) {}
    void Render () {}
};

#endif /* renderer_hpp */
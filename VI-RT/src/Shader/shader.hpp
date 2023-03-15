#ifndef shader_hpp
#define shader_hpp

#include "../Scene/scene.hpp"
#include "../utils/RGB.hpp"

class Shader {
protected:
    Scene *scene;
public:
    Shader (Scene *_scene): scene(_scene) {}
    ~Shader () {}
    virtual RGB shade (bool intersected, Intersection isect) {return RGB();}
};

#endif /* shader_hpp */
#ifndef AmbientShader_hpp
#define AmbientShader_hpp

#include "shader.hpp"

class AmbientShader: public Shader {
    RGB background;
public:
    AmbientShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect);
};

#endif /* AmbientShader_hpp */
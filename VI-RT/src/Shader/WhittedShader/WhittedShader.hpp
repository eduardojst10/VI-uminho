#ifndef WhittedShader_hpp
#define WhittedShader_hpp

#include "../shader.hpp"
#include "../../Primitive/BRDF/Phong.hpp"
#include "../../Rays/ray.hpp"

class WhittedShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f);
public:
    WhittedShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect);
};

#endif /* AmbientShader_hpp */
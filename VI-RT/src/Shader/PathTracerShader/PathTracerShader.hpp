#ifndef PathTracerShaderShader_hpp
#define PathTracerShaderShader_hpp
#define _USE_MATH_DEFINES
#include "../shader.hpp"
#include "../../Primitive/BRDF/Phong.hpp"
#include "../../Rays/ray.hpp"
#include "../../Light/AreaLight.hpp"
#include <cmath>

class PathTracerShader: public Shader {
    RGB background;
    RGB diffuseReflection(Intersection isect, Phong *f, int depth);
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f, int depth);
    float continue_p;
    int MAX_DEPTH;
public:
    PathTracerShader (Scene *scene, RGB bg): background(bg),Shader(scene) {continue_p = 0.5f; MAX_DEPTH=2;}
    RGB shade (bool intersected, Intersection isect, int depth);
};


#endif /* PathTracerShader_hpp */
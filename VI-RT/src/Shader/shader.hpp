#ifndef shader_hpp
#define shader_hpp

#include "../Scene/scene.hpp"
#include "../utils/RGB.hpp"

/*
    Provide a realistic and visually appealing representation of the 3D scene that accurately
    captures the interaction of light and surfaces within the scene
*/

class Shader {
protected:
    Scene *scene;
public:
    Shader (Scene *_scene): scene(_scene) {}
    ~Shader () {}
    virtual RGB shade (bool intersected, Intersection isect, int depth) {return RGB();}

};

#endif /* shader_hpp */
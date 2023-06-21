#include "AmbientShader.hpp"

RGB AmbientShader::shade(bool intersected, Intersection isect) {
    RGB color(0.,0.,0.);

    // if no intersection, return background
    if (!intersected) {
        return (background);
    }

    // verify whether the intersected object has an ambient component
    Phong *f = (Phong *)isect.f;

    if (f->Ka.isZero()) return color;
    RGB Ka = f->Ka;

    // ambient shade
    // Loop over scene's light sources and process Ambient Lights
    for (auto light_itr = this->scene->lights.begin(); light_itr != this->scene->lights.end() ; light_itr++) {
        Point p_dummy;

        // is it an ambient light ?
        if ((*light_itr)->type != AMBIENT_LIGHT) continue;

        color += Ka * (*light_itr)->L(p_dummy);
    }

    return color;
};
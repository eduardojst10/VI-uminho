#include "DistributedShader.hpp"

RGB DistributedShader:: specularReflection(Intersection isect, Phong *f){
    // para já está igual ao whittedShader
    RGB color(0.,0.,0.);

    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);
    specular.adjustOrigin(isect.gn);

    Intersection s_isect;
    // trace ray
    bool intersected = scene->trace(specular, &s_isect);
    // shade this intersection
    RGB reflected_color = shade (intersected, s_isect);

    return f->Ks * reflected_color;
}

RGB DistributedShader::directLighting (Intersection isect, Phong *f) {
    RGB color(0.,0.,0.);

    for (auto l = scene->lights.begin() ; l != scene->lights.end() ; l++) {
        if ((*l)->type == AMBIENT_LIGHT) {  // is it an ambient light ?
            if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                color += Ka * (*l)->L();
            }
            continue;
        }
        if ((*l)->type == POINT_LIGHT) {  // is it a point light ?
            if(!f->Kd.isZero()){
                Point lpoint;
                // get position and radiance of the light source
                RGB L = (*l)->Sample_L(NULL,&lpoint);

                // compute the direction from the intersection to the light
                Vector Ldir = isect.p.point2vec(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize();

                //compute the cosine (Ldir, shading normal)
                float cosL = Ldir.dot(isect.sn);
                if (cosL>0.){ //the light is NOT behind the primitive
                    Ray shadow(isect.p,Ldir);
                    shadow.adjustOrigin(isect.gn);
                    if(scene->visibility(shadow,Ldistance-EPSILON)) color += f->Kd * L * cosL;
                }
            }
            continue;

        }
        if ((*l)->type == AREA_LIGHT) { // is it an area light ?
            if(!f -> Kd.isZero()){
                RGB L, Kd = f->Kd;
                Point lpoint;
                float l_pdf;
                // l é iterador
                AreaLight *al = (AreaLight *) (*l);
                float rnd[2];
                rnd[0] = ((float)rand()) / ((float)RAND_MAX);
                rnd[1] = ((float)rand()) / ((float)RAND_MAX);
                L = al->Sample_L(rnd, &lpoint, l_pdf);
                // compute the direction from the intersection point to the light source
                Vector Ldir = isect.p.point2vec(lpoint);
                const float Ldistance = Ldir.norm();
                // now normalize Ldir
                Ldir.normalize();

                // cosine between Ldir and the shading normal at the intersection point
                float cosL = Ldir.dot(isect.sn);
                // cosine between Ldir and the area light source normal
                float cosL_LA = Ldir.dot(al->gem->normal);
                //shade
                if (cosL>0. && cosL_LA<=0.) {
                    //generate ray
                    Ray shadow(isect.p,Ldir);
                    shadow.adjustOrigin(isect.gn);
                    if(scene->visibility(shadow,Ldistance-EPSILON)){
                        color+= (Kd * L * cosL) / l_pdf;
                    }
                }
            }
        }
    }
    return color;
}


RGB DistributedShader::shade(bool intersected, Intersection isect) {
    RGB color(0.,0.,0.);

    if (!intersected) return (background);
    // intersection with a light source
    if (isect.isLight) return isect.Le;
    Phong *f = (Phong *)isect.f;
    // if there is a specular component sample it
    if (!f->Ks.isZero()) color += specularReflection (isect, f);
    // if there is a diffuse component do direct light
    if (!f->Kd.isZero()) color += directLighting(isect, f);
    return color;
};
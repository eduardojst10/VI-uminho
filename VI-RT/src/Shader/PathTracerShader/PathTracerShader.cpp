#include "PathTracerShader.hpp"

RGB PathTracerShader:: specularReflection(Intersection isect, Phong *f, int depth){
    // para já está igual ao whittedShader
    RGB color(0.,0.,0.);
    Vector Rdir, s_dir;
    float pdf;
    Intersection s_isect;
    // generate the specular ray
    float cos = isect.gn.dot(isect.wo);
    Rdir = 2.f * cos * isect.gn - isect.wo;
    if(f->Ns >= 1000){
        Ray specular(isect.p, Rdir);

        specular.adjustOrigin(isect.gn);

        // trace ray
        bool intersected = scene->trace(specular, &s_isect);
        // shade this intersection
        RGB reflected_color = shade (intersected, s_isect,depth+1);
        color = f->Ks * reflected_color;
        return color;

    }else{ //glossy case -> glossy materials
        float rnd[2];
        rnd[0] = ((float)rand()) / ((float)RAND_MAX);
        rnd[1] = ((float)rand()) / ((float)RAND_MAX);
        Vector S_around_N;
        const float cos_theta = powf(rnd[1], 1.f/(f->Ns+1.f));
        const float aux_r1 = powf(rnd[1], 2.f/(f->Ns+1.f));
        S_around_N.Y = sinf(2.f*M_PI*rnd[0])*sqrtf(1.f-aux_r1);
        S_around_N.X = cosf(2.f*M_PI*rnd[0])*sqrtf(1.f-aux_r1);
        pdf = (f->Ns+1.f)*powf(cos_theta, f->Ns)/(2.f*M_PI);
        Vector Rx, Ry;
        Rdir.CoordinateSystem(&Rx, &Ry);
        s_dir = S_around_N.Rotate (Rx, Ry, Rdir);
        Ray specular(isect.p, s_dir);
        specular.adjustOrigin(isect.gn);
        bool intersected = scene->trace(specular, &s_isect);
        RGB Rcolor = shade (intersected, s_isect, depth+1);
        color = (f->Ks * Rcolor * powf(cos_theta, f->Ns)/(2.f*M_PI)) / pdf ;
        return color;
    }

}

RGB PathTracerShader::directLighting(Intersection isect, Phong *f) {
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


RGB PathTracerShader:: diffuseReflection(Intersection isect, Phong *f, int depth){
    RGB color(0.,0.,0.);
    Vector dir; float pdf;
// actual direction distributed around N: 2 random number in [0,1[
    float rnd[2];
    rnd[0] = ((float)rand()) / ((float)RAND_MAX);
    rnd[1] = ((float)rand()) / ((float)RAND_MAX);
    Vector D_around_Z;
    float cos_theta= D_around_Z.Z = sqrtf(rnd[1]); // cos sampling
    D_around_Z.Y = sinf(2.f*M_PI*rnd[0])*sqrtf(1.f-rnd[1]);
    D_around_Z.X = cosf(2.f*M_PI*rnd[0])*sqrtf(1.f-rnd[1]);
    pdf = cos_theta / M_PI;
    Vector Rx, Ry;
    isect.gn.CoordinateSystem(&Rx, &Ry);

    Ray diffuse(isect.p, D_around_Z.Rotate(Rx, Ry, isect.gn));
    // temos o ray - trace and shade it recursively
    //faltava o adjustOrigin - secundary ray -> new origin
    diffuse.adjustOrigin(isect.gn);
    Intersection d_isect; //new intersection recursively
    bool intersected = scene->trace(diffuse, &d_isect);
    // if light source return 0 ; handled by direct
    if (!d_isect.isLight) { // shade this intersection
        RGB Rcolor = shade (intersected, d_isect, depth+1);
        color = (f->Kd * cos_theta * Rcolor) /pdf ;
    }
    return color;
}

RGB PathTracerShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.f,0.f,0.f);
    if (!intersected) return (background);
    // intersection with a light source
    if (isect.isLight) return isect.Le;
    Phong *f = (Phong *)isect.f;
    float rnd_russian = ((float) rand()/ (float) RAND_MAX);
    //russian roulette entre a especular e a difusa
    if (depth <MAX_DEPTH || rnd_russian < continue_p) {
        RGB lcolor;
        float s_p = f->Ks.Y() / (f->Ks.Y()+f->Kd.Y());
        float rnd = ((float)rand()) / ((float)RAND_MAX);
        if (rnd < s_p) lcolor = specularReflection(isect, f ,depth) / s_p;

        else lcolor = diffuseReflection(isect,f,depth) / (1.f-s_p);

        if(depth < MAX_DEPTH) //rnd_russian < continue_p false (sem russian roulette)
            color+=lcolor;
        else color+= lcolor / continue_p;
    }
    // if there is a diffuse component do direct light - without bouncing
    if (!f->Kd.isZero()) color += directLighting(isect, f);
    return color;
};
#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y,ss;
    //const float cam_jitter = 1.0f;
    // get resolution from the camera
    this->cam->getResolution(&W,&H); 
    // main rendering loop: get primary rays from the camera until done

    // number of samples per pixel
    const int spp = 16;
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected = false;
            RGB color;
            //iterate through the pixel samples
            for (ss = 0; ss < spp; ss++){
                RGB this_color;
                // taking multiple random samples within each pixel and averaging their color contributions
                // float jitterV[2] = [rand(0 .. 1), rand(0 .. 1)];
                float jitterV[2] = {static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX};
                // Generate Ray (camera)
                this->cam->GenerateRay(x,y,&primary,jitterV);
                
                // trace ray (scene), type of intersection
                intersected = this->scene->trace(primary,&isect);
            
                // shade this intersection (shader)
                this_color = this->shd->shade(intersected,isect,0);     
                color += this_color;
            }
            color = color / spp;
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
        } // loop over columns
    }   // loop over rows
}
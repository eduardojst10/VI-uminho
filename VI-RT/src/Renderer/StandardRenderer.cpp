#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y;
    const float cam_jitter = 1.0f;

    // get resolution from the camera
    this->cam->getResolution(&W,&H);
    //std:: cout << "Width: " << W <<  " Height: " << H << "\n"; 
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected = false;
            RGB color;
            // Generate Ray (camera)
            this->cam->GenerateRay(x,y,&primary,&cam_jitter);
            // trace ray (scene)
            intersected = this->scene->trace(primary,&isect);
            // shade this intersection (shader)
            color = this->shd->shade(intersected,isect); 

        
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
#include "StandardRenderer.hpp"

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y;

    // get resolution from the camera
    // ...
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            // Generate Ray (camera)
            // ...
            
            // trace ray (scene)
            // ...
            
            // shade this intersection (shader)
            // ...
            
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
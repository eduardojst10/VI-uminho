#include "StandardRenderer.hpp"
#include <iostream>
#include <cmath>

void StandardRenderer::Render () {
    int W=0,H=0;  // resolution
    int x,y,ss;
    // resolution from the camera
    this->cam->getResolution(&W,&H);
    // number of samples per pixel
    const int spp = 16;
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected = false;
            RGB color(0.f, 0.f,0.f);
            //iterate through the pixel samples
            for (ss = 0; ss < spp; ss++){
                RGB this_color;
                // taking multiple random samples within each pixel and averaging their color contributions
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
        }
        // print progress bar
        float progress = (float)(y+1)/H;
        int barWidth = 40; // Changed the width to 40 characters
        std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << round(progress * 100.0) << "%\r"; // Used round to get the correct percentage
        std::cout.flush();
    }
    std::cout << std::endl;
}

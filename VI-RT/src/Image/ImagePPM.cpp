#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <exception>

void ImagePPM::ToneMap () {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].R) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].G) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].B) * 255);
        }
    }

}

bool ImagePPM::Save (std::string filename) {
    
    if(this->W == 0 || this->H == 0){
        std::cerr <<  "Can't save an empty image" << std::endl;
        return false;
    }

    // convert from float to {0,1,..., 255}
    ToneMap();

    // write imageToSave to file    
    std::cout << filename << std::endl;
    std::ofstream ofs;
    try{
        ofs.open(filename, std::ios::binary); //need to spec. binary mode for windows
        if(ofs.fail()){
          throw("Cant open output file");
          return false;  
        } 
        ofs << "P6\n" << this->W << " " << this->H << "\n255\n";

        for(int j = 0; j < H; j++){
            for(int i= 0; i< W; i++){
                unsigned char r = imageToSave[j * W + i].val[0];
                unsigned char g = imageToSave[j * W + i].val[1];
                unsigned char b = imageToSave[j * W + i].val[2];
                ofs.write(reinterpret_cast<const char*>(&r), sizeof(r));
                ofs.write(reinterpret_cast<const char*>(&g), sizeof(g));
                ofs.write(reinterpret_cast<const char*>(&b), sizeof(b));
            }
        }
        //unsigned char r, g,b;
        //r = this->imageToSave->val[0];
        //g = this->imageToSave->val[1];
        //b = this->imageToSave->val[2];
        //ofs << r << g << b;
        ofs.close();    
    }catch(std::string err){
        std:: cerr << err << std:: endl;
        ofs.close();
        return false;
    }


    // Details and code on PPM files available at:
    // https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/reading-writing-images.html
    
    return true;
}

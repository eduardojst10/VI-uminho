#ifndef IMAGEPFM_HPP
#define IMAGEPFM_HPP

#include <fstream>
#include <string>
#include "image.hpp"

class ImagePFM: public Image {
public:
    ImagePFM(const int W, const int H) : Image(W, H) {}
    bool Save(std::string filename);
};

#endif // IMAGEPFM_HPP

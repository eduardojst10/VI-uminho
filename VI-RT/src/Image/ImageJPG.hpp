#ifndef IMAGEJPG_HPP
#define IMAGEJPG_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "image.hpp"

class ImageJPG: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } JPG_pixel;
    JPG_pixel* imageToSave;

    void ToneMap_original();
    void ToneMap_ACES();
    void ToneMap_Uncharted();
    void ToneMap_Reinhards();

public:
    ImageJPG(const int W, const int H) : Image(W, H) {}
    bool Save(std::string filename);

    RGB AcesFilmicToneMapping(RGB color);
};

#endif // IMAGEJPG_HPP

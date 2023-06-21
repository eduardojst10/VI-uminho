#ifndef ImagePPM_hpp
#define ImagePPM_hpp

#include "image.hpp"

class ImagePPM : public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel* imageToSave;

    void ToneMap_original();
    void ToneMap_ACES();
    void ToneMap_Uncharted();
    void ToneMap_Reinhards();

public:
    ImagePPM(const int W, const int H) : Image(W, H) {}
    bool Save(std::string filename);
    RGB* GetImage() const;
    RGB AcesFilmicToneMapping(RGB color);
};


#endif /* ImagePPM_hpp */

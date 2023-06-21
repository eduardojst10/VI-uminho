#ifndef ImageEXR_hpp
#define ImageEXR_hpp

#include "image.hpp"
#include <opencv2/opencv.hpp>

class ImageEXR : public Image {
public:
    ImageEXR(): Image() {}
    ImageEXR(const int W, const int H): Image(W, H) {}

    virtual bool Save(std::string filename) override;
};

#endif /* ImageEXR_hpp */

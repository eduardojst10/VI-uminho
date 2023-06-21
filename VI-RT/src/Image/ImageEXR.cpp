#include "ImageEXR.hpp"

bool ImageEXR::Save(std::string filename) {
    cv::Mat hdr_image(H, W, CV_32FC3);

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            RGB &rgb = imagePlane[i*W+j];
            hdr_image.at<cv::Vec3f>(i, j) = cv::Vec3f(rgb.B, rgb.G, rgb.R);
        }

    std::vector<int> exr_params;
    exr_params.push_back(cv::IMWRITE_EXR_TYPE);
    exr_params.push_back(cv::IMWRITE_EXR_TYPE_FLOAT); //specify the datatype of a pixel. Here it is set as float

    cv::imwrite(filename, hdr_image, exr_params);
    return true;
}

#include "ImagePFM.hpp"
#include <iostream>

bool ImagePFM::Save(std::string filename) {
    if (W == 0 || H == 0) {
        std::cerr << "Can't save an empty image" << std::endl;
        return false;
    }

    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    // Write PFM header
    file << "PF\n" << W << " " << H << "\n-1.0\n";

    // Write pixel data
    for (int j = H-1; j >= 0; --j) {
        for (int i = 0; i < W; ++i) {
            RGB& color = imagePlane[j * W + i];
            file.write(reinterpret_cast<char*>(&color.R), sizeof(float));
            file.write(reinterpret_cast<char*>(&color.G), sizeof(float));
            file.write(reinterpret_cast<char*>(&color.B), sizeof(float));
        }
    }

    file.close();

    return true;
}

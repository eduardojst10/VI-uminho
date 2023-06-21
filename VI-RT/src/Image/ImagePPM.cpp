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

void ImagePPM::ToneMap_original() {
    imageToSave = new PPM_pixel[W * H];
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            imageToSave[j * W + i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].R) * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].G) * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j * W + i].B) * 255);
        }
    }
}

// Reinhard's tone map
void ImagePPM::ToneMap_Reinhards() {
    float Lwhite = 0.0f;

    // First pass: find the maximum luminance in the image
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            RGB& color = imagePlane[j * W + i];
            float L = 0.2126f * color.R + 0.7152f * color.G + 0.0722f * color.B;
            Lwhite = std::max(Lwhite, L);
        }
    }

    imageToSave = new PPM_pixel[W * H];

    // Second pass: scale the luminance of each pixel using the tone mapping operator
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            RGB& color = imagePlane[j * W + i];
            float L = 0.2126f * color.R + 0.7152f * color.G + 0.0722f * color.B;
            float Ld = L * (1 + (L / (Lwhite * Lwhite))) / (1 + L);  // Apply the tone mapping
            float scale = Ld / L;

            // Apply scale to RGB colors and clamp them between 0 and 1
            float R = std::max(0.f, std::min(scale * color.R, 1.f));
            float G = std::max(0.f, std::min(scale * color.G, 1.f));
            float B = std::max(0.f, std::min(scale * color.B, 1.f));

            // Convert the result to byte format and save to imageToSave
            imageToSave[j * W + i].val[0] = (unsigned char)(R * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(G * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(B * 255);
        }
    }
}

// This is the aproximated ACES method
RGB ImagePPM::AcesFilmicToneMapping(RGB color) {
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    RGB mapped;
    mapped.R = (color.R * (a * color.R + b)) / (color.R * (c * color.R + d) + e);
    mapped.G = (color.G * (a * color.G + b)) / (color.G * (c * color.G + d) + e);
    mapped.B = (color.B * (a * color.B + b)) / (color.B * (c * color.B + d) + e);

    // Clamp the output to [0,1]
    mapped.R = std::max(0.f, std::min(mapped.R, 1.f));
    mapped.G = std::max(0.f, std::min(mapped.G, 1.f));
    mapped.B = std::max(0.f, std::min(mapped.B, 1.f));

    return mapped;
}

// Filmic ACES tone map
void ImagePPM::ToneMap_ACES() {
    imageToSave = new PPM_pixel[W * H];

    // Define your background color here (This is an example)
    RGB backgroundColor = {0.0f, 0.0f, 1.0f};

    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            RGB originalColor = imagePlane[j * W + i];

            // Check if the current pixel color is the background color
            if(originalColor.R == backgroundColor.R &&
               originalColor.G == backgroundColor.G &&
               originalColor.B == backgroundColor.B) {
                // Skip tone mapping and convert original color to byte format
                imageToSave[j * W + i].val[0] = (unsigned char)(originalColor.R * 255);
                imageToSave[j * W + i].val[1] = (unsigned char)(originalColor.G * 255);
                imageToSave[j * W + i].val[2] = (unsigned char)(originalColor.B * 255);
            } else {
                // Apply tone mapping if it's not the background color
                RGB mappedColor = AcesFilmicToneMapping(originalColor);
                imageToSave[j * W + i].val[0] = (unsigned char)(mappedColor.R * 255);
                imageToSave[j * W + i].val[1] = (unsigned char)(mappedColor.G * 255);
                imageToSave[j * W + i].val[2] = (unsigned char)(mappedColor.B * 255);
            }
        }
    }
}

// Filmic Uncharted 2 tone map
void ImagePPM::ToneMap_Uncharted() {
    // Constants for Filmic Tone Mapping
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;

    imageToSave = new PPM_pixel[W * H];

    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; ++i) {
            RGB& color = imagePlane[j * W + i];

            // Check if the color is RGB(0, 0, 1)
            if (color.R == 0 && color.G == 0 && color.B == 1) {
                // Keep the original color and skip tone mapping
                imageToSave[j * W + i].val[0] = (unsigned char) color.R * 255;
                imageToSave[j * W + i].val[1] = (unsigned char) color.G * 255;
                imageToSave[j * W + i].val[2] = (unsigned char) color.B * 255;
                continue;
            }

            // Apply the filmic tone mapping operator to each color channel
            float R = color.R * (A * color.R + C * B) / (color.R * (A * color.R + B) + D * E) + F - E / F;
            float G = color.G * (A * color.G + C * B) / (color.G * (A * color.G + B) + D * E) + F - E / F;
            float B = color.B * (A * color.B + C * B) / (color.B * (A * color.B + B) + D * E) + F - E / F;

            // Clamp the results between 0 and 1
            R = std::max(0.f, std::min(R, 1.f));
            G = std::max(0.f, std::min(G, 1.f));
            B = std::max(0.f, std::min(B, 1.f));

            // Convert the result to byte format and save to imageToSave
            imageToSave[j * W + i].val[0] = (unsigned char)(R * 255);
            imageToSave[j * W + i].val[1] = (unsigned char)(G * 255);
            imageToSave[j * W + i].val[2] = (unsigned char)(B * 255);
        }
    }
}



    bool ImagePPM::Save(std::string filename) {
    if (W == 0 || H == 0) {
        std::cerr << "Can't save an empty image" << std::endl;
        return false;
    }
    // convert from float to {0,1,..., 255}
    // ToneMap_original();
     ToneMap_ACES();
    //ToneMap_Uncharted();
    //ToneMap_Reinhards();
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); //need to spec. binary mode for windows
        if (ofs.fail()) {
            throw std::runtime_error("Can't open output file");
        }
        ofs << "P6\n"
            << this->W << " " << this->H << "\n255\n";

        for (int j = 0; j < H; j++) {
            for (int i = 0; i < W; i++) {
                unsigned char r = imageToSave[j * W + i].val[0];
                unsigned char g = imageToSave[j * W + i].val[1];
                unsigned char b = imageToSave[j * W + i].val[2];
                ofs.write(reinterpret_cast<const char*>(&r), sizeof(r));
                ofs.write(reinterpret_cast<const char*>(&g), sizeof(g));
                ofs.write(reinterpret_cast<const char*>(&b), sizeof(b));
            }
        }

        ofs.close();
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        ofs.close();
        return false;
    }

    return true;
}

RGB* ImagePPM::GetImage() const {
    return imagePlane;
}

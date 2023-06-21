#ifndef Phong_hpp
#define Phong_hpp

#include "../../utils/RGB.hpp"
#include "BRDF.hpp"

class Phong: public BRDF {
public:
    RGB Ka, //Represents the ambient reflection coefficient of the material
    Kd, //Represents the diffuse reflection coefficient of the material
    Ks, //Represents the specular reflection coefficient of the material
    Kt; //Represents the transmission coefficient of the material
    float Ns;    //Shininess: Se o Ns for muito grande este comporta-se como um espelho shininess

    Phong() :
            Ka(RGB(0.0f, 0.0f, 0.0f)),
            Kd(RGB(0.0f, 0.0f, 0.0f)),
            Ks(RGB(0.0f, 0.0f, 0.0f)),
            Kt(RGB(0.0f, 0.0f, 0.0f)),
            Ns(0.0f)
    {}
};

#endif /* Phong_hpp */

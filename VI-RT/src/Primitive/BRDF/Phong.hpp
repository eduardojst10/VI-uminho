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
    float Ns;    //Se o Ns for muito grande este comporta-se como um espelho shininess
};

/*
    Ka: represents the ambient reflection coefficient of the material
    Kd: represents the diffuse reflection coefficient of the material
    Ks: represents the specular reflection coefficient of the material
    Kt: represents the transmission coefficient of the material
    Ns: represents the shininess exponent of the material
*/

#endif /* Phong_hpp */

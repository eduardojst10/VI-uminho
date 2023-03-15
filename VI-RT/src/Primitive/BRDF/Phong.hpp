#ifndef Phong_hpp
#define Phong_hpp

#include "../../utils/RGB.hpp"
#include "BRDF.hpp"

class Phong: public BRDF {
public:
    RGB Ka, //Propriedade do material
    Kd, 
    Ks, 
    Kt;
    float Ns;    //Se o Ns for muito grande este comporta-se como um espelho
};

#endif /* Phong_hpp */

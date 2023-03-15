#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
#include <vector>
#include "../Primitive/primitive.hpp"
#include "../Light/light.hpp"
#include "../Rays/ray.hpp"
#include "../Rays/intersection.hpp"
#include "../Primitive/BRDF/BRDF.hpp"

class Scene {
    int numPrimitives, numLights, numBRDFs;
    std::vector <Primitive> prims;
    std::vector <Light> lights;
    std::vector <BRDF> BRDFs;
public:
    Scene (): numPrimitives(0), numLights(0), numBRDFs(0) {}
    bool Load (const std::string &fname);
    bool SetLights (void) { return true; };
    bool trace (Ray r, Intersection *isect);
    void printSummary(void) {
        std::cout << "#primitives = " << numPrimitives << " ; ";
        std::cout << "#lights = " << numLights << " ; ";
        std::cout << "#materials = " << numBRDFs << " ;" << std::endl;
    }
};

#endif /* Scene_hpp */

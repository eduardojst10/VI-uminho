#ifndef AccelStruct_hpp
#define AccelStruct_hpp

#include "Ray.hpp"
#include "Intersection.hpp"
#include "Scene.hpp"

class AccelStruct {
public:
    AccelStruct () {}
    ~AccelStruct () {}
    void build (Scene s) {}
    bool trace (Scene s, Ray r, Intersection *isect) {}
};

#endif /* AccelStruct_hpp */

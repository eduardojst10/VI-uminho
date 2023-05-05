#ifndef geometry_hpp
#define geometry_hpp

#include "../BB.hpp"
#include "../../Rays/ray.hpp"
#include "../../Rays/intersection.hpp"

class Geometry {
public:
    Geometry () {}
    ~Geometry () {}
    // return True if r intersects this geometric primitive
    // returns data about intersection on isect
    virtual bool intersect (Ray r, Intersection *isect) = 0;
    // geometric primitive bounding box
    BB bb;  // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
};

#endif /* geometry_hpp */

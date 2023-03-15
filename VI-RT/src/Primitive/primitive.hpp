#ifndef primitive_hpp
#define primitive_hpp

#include "Geometry/geometry.hpp"
#include "BRDF/BRDF.hpp"

typedef struct Primitive {
    Geometry *g;
    int material_ndx;
} Primitive;

#endif /* primitive_hpp */

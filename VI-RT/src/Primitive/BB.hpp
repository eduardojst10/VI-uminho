#ifndef BB_hpp
#define BB_hpp

#include "../utils/vector.hpp"
#include "../Rays/ray.hpp"


/*
    The purpose of the face bounding box (BB bb) is to provide
    an approximate axis-aligned bounding box that encloses
    the triangle of the mesh represented by the Face struct.

    This bounding box can be used for several purposes in a rendering algorithm,
    such as accelerating collision detection, culling invisible triangles or accelerating ray-tracing.
*/
typedef struct BB {
    Point min, max;
    void update (Point p) {
        if (p.X < min.X) min.X = p.X;
        else if (p.X > max.X) max.X = p.X;
        if (p.Y < min.Y) min.Y = p.Y;
        else if (p.Y > max.Y) max.Y = p.Y;
        if (p.Z < min.Z) min.Z = p.Z;
        else if (p.Z > max.Z) max.Z = p.Z;
    }

    bool intersect(Ray r){
        return false;
    }

} BB;

#endif /* AABB_hpp */

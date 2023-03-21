#ifndef Ray_hpp
#define Ray_hpp

#include "../utils/vector.hpp"

class Ray {
public:
    Point o; // ray origin
    Vector dir; // ray direction, Ray vector
    Ray () {}
    Ray (Point o, Vector d): o(o),dir(d) {}
    ~Ray() {}
};

#endif /* Ray_hpp */

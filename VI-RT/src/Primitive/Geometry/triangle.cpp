#include "triangle.hpp"
#include <iostream>
#include<limits>
const float MAX_FLOAT = std::numeric_limits<float>::max();

bool Triangle::intersect (Ray r, Intersection *isect) {
    const float EPSILON = 1e-6f;
    Vector h, s, q;
    float a, f, u, v;
    h = r.dir.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0f / a;
    s = v1.point2vec(r.o);
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = f * r.dir.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dot(q);
    if (t > EPSILON) {
        // ray intersection
        Point inter = r.o + (r.dir * t);
        isect->wo = r.dir  * static_cast<float>(-1);
        Vector wo = r.dir * static_cast<float>(-1);
        normal.Faceforward(wo);
        isect->depth = t;
        isect->p = inter;
        isect->gn = normal;
        isect->sn = normal;
        isect->wo.normalize();
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

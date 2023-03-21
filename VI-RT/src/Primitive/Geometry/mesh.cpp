#include "mesh.hpp"
#define MAXFLOAT 0.2
// see pbrt book (3rd ed.), sec 3.6.2, pag 157
/*
    Link: https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm


*/
bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {
    bool success;
    if(!f.bb.intersect(r)) return false;

    const float EPSILON = 0.0000001;
    Point v0 = this->vertices.at(f.vert_ndx[0]);
    Point v1 = this->vertices.at(f.vert_ndx[1]);
    Point v2 = this->vertices.at(f.vert_ndx[2]);

    Vector edge1, edge2, h, s, q;

    return true;
}

bool Mesh::intersect (Ray r, Intersection *isect) {
    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth= MAXFLOAT;
    // intersect the ray with the mesh BB
    
    if (!intersect) return false;
    
    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it=faces.begin() ; face_it != faces.end() ; face_it++) {
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
        if (!intersect_this_face) continue;
        
        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }
    
    return intersect;
}

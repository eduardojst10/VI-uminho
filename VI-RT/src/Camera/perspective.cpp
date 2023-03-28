#include "perspective.hpp"
//meter a void?
bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    Vector p = Vector(x,y,1);
    float rDirX, rDirY, rDirZ;
    rDirX =  this->c2w[0][0] * p.X + this->c2w[0][1] * p.Y + this->c2w[0][2] * p.Z;
    rDirY = this->c2w[1][0] * p.X + this->c2w[1][1] * p.Y + this->c2w[1][2] * p.Z;
    rDirZ = this->c2w[2][0] * p.X + this->c2w[2][1] * p.Y + this->c2w[2][2] * p.Z;
    r->dir = Vector(rDirX,rDirY,rDirZ);
    // temos de normalizar
    r->dir.normalize();
    r->o = this->Eye;
    return true;
}

#include "perspective.hpp"

void Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    float normalized_x = (static_cast<float>(x) / static_cast<float>(W - 1)) * 2.0f - 1.0f;
    float normalized_y = (static_cast<float>(y) / static_cast<float>(H - 1)) * 2.0f - 1.0f;

    Vector p = Vector(normalized_x,normalized_y,1.0f);
    float rDirX, rDirY, rDirZ;
    rDirX =  this->c2w[0][0] * p.X + this->c2w[0][1] * p.Y + this->c2w[0][2] * p.Z;
    rDirY = this->c2w[1][0] * p.X + this->c2w[1][1] * p.Y + this->c2w[1][2] * p.Z;
    rDirZ = this->c2w[2][0] * p.X + this->c2w[2][1] * p.Y + this->c2w[2][2] * p.Z;
    r->dir = Vector(rDirX,rDirY,rDirZ);
    // temos de normalizar
    r->dir.normalize();
    r->o = this->Eye;
    r->x = x;
    r->y = y;
    
}

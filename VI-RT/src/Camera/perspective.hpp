#ifndef perspective_hpp
#define perspective_hpp

#include "camera.hpp"
#include "../Rays/ray.hpp"
#include "../utils/vector.hpp"

class Perspective: public Camera {
    Point Eye, At;
    Vector Up;
    float fovW, fovH;
    int W, H;
    float c2w[3][3];  // camera 2 world transform
public:
    Perspective (const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH): Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH)  {
        // compute camera 2 world transform
        // camera direction (F)
        Vector F = Eye.point2vec(At);
        F.normalize();

        // camera right vector (R)
        Vector R = F.cross(Up);
        R.normalize();

        // camera up vector (U)
        Vector U = R.cross(F);
        U.normalize();

        // Fill in the c2w matrix
        c2w[0][0] = R.X;
        c2w[0][1] = R.Y;
        c2w[0][2] = R.Z;
        c2w[1][0] = U.X;
        c2w[1][1] = U.Y;
        c2w[1][2] = U.Z;
        c2w[2][0] = F.X;
        c2w[2][1] = F.Y;
        c2w[2][2] = F.Z;

    }
    void GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter);
    void getResolution (int *_W, int *_H) {*_W=W; *_H=H;}
};

#endif /* perspective_hpp */

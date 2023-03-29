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
        //camera direction
        Vector vdirection = Eye.point2vec(At);
        vdirection.normalize(); 

        //camera right vector
        Vector Up_normalized = Up;
        Up_normalized.normalize();
        Vector r = Up.cross(vdirection);
        r.normalize();
        c2w[0][0] = r.X;
        c2w[0][1] = r.Y;
        c2w[0][2] = r.Z;
        c2w[1][0] = Up.X;
        c2w[1][1] = Up.Y;
        c2w[1][2] = Up.Z;
        c2w[2][0] = vdirection.X;
        c2w[2][1] = vdirection.Y;
        c2w[2][2] = vdirection.Z;
        
    }
    void GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL);
    void getResolution (int *_W, int *_H) {*_W=W; *_H=H;}
};

#endif /* perspective_hpp */

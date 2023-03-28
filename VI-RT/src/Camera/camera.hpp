#ifndef camera_hpp
#define camera_hpp

#include "../Rays/ray.hpp"

// based on pbrt book, sec 6.1, pag. 356
class Camera {
public:
    Camera () {}
    ~Camera() {}
    virtual void GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter=NULL)=0;
    virtual void getResolution (int *_W, int *_H) {*_W=0; *_H=0;}
};

#endif /* camera_hpp */

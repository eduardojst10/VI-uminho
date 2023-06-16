#ifndef RGB_hpp
#define RGB_hpp

class RGB {
public:
    float R, G, B;
    RGB ():R(0.),G(0.),B(0.) {}
    RGB (float r, float g, float b):R(r),G(g),B(b) {}
    RGB (float *rgb):R(rgb[0]),G(rgb[1]),B(rgb[2]) {}
    ~RGB () {}
    RGB& operator+=(const RGB& rhs){
        this->R += rhs.R;
        this->G += rhs.G;
        this->B += rhs.B;
        return *this;
    }
    RGB operator+(RGB const& obj)
    {
        RGB res;
        res.R = R + obj.R;
        res.G = G + obj.G;
        res.B = B + obj.B;
        return res;
    }
    RGB operator*(RGB const& obj)
    {
        RGB res;
        res.R = R * obj.R;
        res.G = G * obj.G;
        res.B = B * obj.B;
        return res;
    }
    RGB operator*(float value)
    {
        return RGB(R * value, G * value, B * value);
    }

    RGB operator/(float value){
        return RGB(R / value, G / value, B / value);
    }
    float Y() {
        return (R*0.2126f + G*0.7152f + B*0.0722f );
    }
    bool isZero () {
        return ((R==0.) && (G==0.) && (B==0.));
    }
};

#endif /* RGB_hpp */

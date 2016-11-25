#ifndef FRACTAL_H
#define FRACTAL_H

#include<vecmath.h>
#include<cmath>

class Fractal {
public:
    virtual float DE(Vector3f pos) const = 0;
};

class Mandelbulb: public Fractal {
public:
    virtual float DE(Vector3f pos) const override;
};
#endif

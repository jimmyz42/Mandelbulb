#include "Fractal.h"

float Mandelbulb::DE(Vector3f pos) const {
//CONST
int maxIters = 20;
float escapeRadius = 4.0;
float bulbPower = 8;
//END
    Vector3f z = pos;
    float dr = 1.0, r = 0.0;
    for(int i=0; i < maxIters; i++) {
        r = z.abs();
        if(r > escapeRadius) break;
        float theta = acos(z.z()/r);
        float phi = atan2(z.y(), z.x());
        dr = pow(r, bulbPower-1.0) * bulbPower * dr + 1.0;
        
        float zmag = pow(r, bulbPower);
        theta *= bulbPower;
        phi *= bulbPower;

        z = zmag * Vector3f(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)) + pos;
    }
    return 0.5 * log(r) * r / dr;
}

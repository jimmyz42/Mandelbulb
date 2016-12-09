#include "Fractal.h"

inline float Fractal::clamp(const Vector3f& L, const Vector3f& N) const {
    return fmax(Vector3f::dot(L, N), 0);
}

inline Vector3f Fractal::getNormal(const Vector3f& pos, float eps = 1.0e-6) const {
    Vector3f epsX(eps, 0, 0), epsY(0, eps, 0), epsZ(0, 0, eps);
    Vector3f normal(DE(pos+epsX)-DE(pos-epsX), DE(pos+epsY)-DE(pos-epsY), DE(pos+epsZ)-DE(pos-epsZ));
    normal.normalize();
    return normal;
}

Vector3f Fractal::shade(const Ray& ray, const Vector3f& pos, const Vector3f &dirToLight, const Vector3f &lightIntensity) const
{
    Vector3f normal = getNormal(pos);
    Vector3f diffuseColor = getDiffuseColor(pos);
    Vector3f specularColor = getSpecularColor(pos);
    float shininess = getShininess(pos);

    Vector3f diffuse = clamp(dirToLight, normal) * lightIntensity * diffuseColor;
    Vector3f reye = ray.getDirection() - 2 * Vector3f::dot(ray.getDirection(), normal) * normal;
    Vector3f specular = pow(clamp(dirToLight, reye), shininess) * lightIntensity * specularColor;
    return diffuse + specular;
}

//TODO implement these in subclass
// Default implementation is just aquamarine diffuse, white specular

inline Vector3f Fractal::getDiffuseColor(const Vector3f& pos) const {
    return Vector3f(0.5, 1, 0.83);
}

inline Vector3f Fractal::getSpecularColor(const Vector3f& pos) const {
    return Vector3f(1, 1, 1);
}

inline float Fractal::getShininess(const Vector3f& pos) const {
    return 0.1f;
}

float Mandelbulb::DE(Vector3f pos) const {
    Vector3f z = pos;
    float dr = 1.0, r = 0.0;
    for(int i=0; i < _maxIters; i++) {
        r = z.abs();
        if(r > _escapeRadius) break;

        float theta = acos(z.z()/r);
        float phi = atan2(z.y(), z.x());
        dr = pow(r, _bulbPower-1.0) * _bulbPower * dr + 1.0;
        
        float zmag = pow(r, _bulbPower);
        theta *= _bulbPower;
        phi *= _bulbPower;

        z = zmag * Vector3f(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)) + pos;
    }
    
    return 0.5 * log(r) * r / dr;
}

float Mandelbox::DE(Vector3f pos) const {
    Vector3f z = pos;
    float dr = 1.0;
    for(int i=0; i < _maxIters; i++) {
        boxFold(z, dr);
        z *= _scaleBox;
        dr *= fabs(_scaleBox);
        sphereFold(z, dr);
        z = _scaleSph * z + pos;
        dr = dr * fabs(_scaleSph) + 1.0;
    }
    return z.abs() / dr;
}

void Mandelbox::sphereFold(Vector3f& z, float& dr) const {
    float radiusSq = z.absSquared();
    if(radiusSq < _innerRadiusSq) {
        float tmp = _outerRadiusSq / _innerRadiusSq;
        z *= tmp;
        dr *= tmp;
    } else if(radiusSq < _outerRadiusSq) {
        float tmp = _outerRadiusSq / radiusSq;
        z *= tmp;
        dr *= tmp;
    }
}

void Mandelbox::boxFold(Vector3f& z, float& dr) const {
    for(int i=0; i<3; i++) {
        if(z[i] > _foldLimit) z[i] = 2 * _foldLimit - z[i];
        if(z[i] < -_foldLimit) z[i] = -2 * _foldLimit - z[i];
    }
}








#ifndef FRACTAL_H
#define FRACTAL_H

#include<vecmath.h>
#include<cmath>
#include "Ray.h"

class Fractal {
public:
    virtual float DE(Vector3f pos) const = 0;
    Vector3f getNormal(const Vector3f& pos, float eps = 1.0e-6) const;
    Vector3f shade(const Ray& r, const Vector3f& pos, const Vector3f &dirToLight, const Vector3f &lightIntensity) const;
    virtual Vector3f getDiffuseColor(const Vector3f& pos) const;
    virtual Vector3f getSpecularColor(const Vector3f& pos) const;
    virtual float getShininess(const Vector3f& pos) const;

private:
    float clamp(const Vector3f& L, const Vector3f& N) const;
};

class Mandelbulb: public Fractal {
public:
    Mandelbulb(int maxIters = 6 /* 20 */, float escapeRadius = 4.0, float bulbPower = 8):
        _maxIters(maxIters), _escapeRadius(escapeRadius), _bulbPower(bulbPower) {}
    virtual float DE(Vector3f pos) const override;

private:
    int _maxIters;
    float _escapeRadius;
    float _bulbPower;
};

class Mandelbox: public Fractal {
public:
    Mandelbox(int maxIters = 6 /* 20 */, float escapeRadius = 4.0, float scaleSph = 1.5, float scaleBox = 1,
        float foldLimit = 1, float innerRadius = 0.5, float outerRadius = 1):
        _maxIters(maxIters), _escapeRadius(escapeRadius), _scaleSph(scaleSph), _scaleBox(scaleBox),
        _foldLimit(foldLimit), _innerRadiusSq(innerRadius * innerRadius), _outerRadiusSq(outerRadius * outerRadius) {}
    virtual float DE(Vector3f pos) const override;

private:
    void sphereFold(Vector3f& z, float& dr) const;
    void boxFold(Vector3f& z, float& dr) const;

    int _maxIters;
    float _escapeRadius;
    float _scaleSph;
    float _scaleBox;
    float _foldLimit;
    float _innerRadiusSq;
    float _outerRadiusSq;    
};
#endif

#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

#include <vecmath.h>
#include <float.h>
#include <math.h>

class Camera
{
public:
    virtual ~Camera() {}

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual float getTMin() const = 0;
};

/// Fill in functions and add more fields if necessary
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const Vector3f &center,
        const Vector3f &direction,
        const Vector3f &up,
        float angleradians) :
        _center(center),
        _direction(direction.normalized()),
        _up(up),
        _angle(angleradians)
    {
        _horizontal = Vector3f::cross(direction, up).normalized();
    }

    virtual Ray generateRay(const Vector2f &point) override
    {
        // BEGIN STARTER
        float d = 1.0f / (float)std::tan(_angle / 2.0f);
        Vector3f newDir = d * _direction + point[0] * _horizontal + point[1] * _up;
        newDir = newDir.normalized();

        return Ray(_center, newDir);
        // END STARTER
    }

    virtual float getTMin() const override
    {
        return 0.0f;
    }

private:
    Vector3f _center;
    Vector3f _direction;
    Vector3f _up;
    float _angle;
    Vector3f _horizontal;
};

class SphericalCamera: public Camera
{
public:
    SphericalCamera(const Vector3f& center): _center(center) {}

    virtual Ray generateRay(const Vector2f& point) override {
        float phi = point[0] * 2 * M_PI, theta = point[1] * M_PI;
        Vector3f dir(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
        return Ray(_center, dir);
    }

    virtual float getTMin() const override {
        return 0.0f;
    }

private:
    Vector3f _center;
};

#endif //CAMERA_H

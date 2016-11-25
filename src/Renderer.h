#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include "ArgParser.h"
#include "Fractal.h"

class Hit;
class Vector3f;
class Ray;

class Renderer
{
  public:
    // Instantiates a renderer for the given scene.
    Renderer(const ArgParser &args);
    void Render();
  private:
    Vector3f traceRay(const Ray &ray, float tmin, int bounces, 
                      Hit &hit) const;

    ArgParser _args;
    Fractal *_fractal;
};

#endif // RENDERER_H

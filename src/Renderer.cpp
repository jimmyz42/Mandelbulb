#include "Renderer.h"

#include "ArgParser.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "VecUtils.h"

#include <limits>
#include <math.h>

using namespace std;

Renderer::Renderer(const ArgParser &args) :
    _args(args)
{
//    _fractal = new Mandelbulb();
    _fractal = new Mandelbox();
}

void Renderer::Render() {
    int w = _args.width;
    int h = _args.height;
    
    Image image(w, h);

    //PerspectiveCamera cam(Vector3f::FORWARD * 6, Vector3f::FORWARD, Vector3f::UP, 75);
    PerspectiveCamera cam(Vector3f(0, 5, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 1), 75);
    //PerspectiveCamera cam(Vector3f(0.05, 0.84, 0.2), Vector3f(0, 1, 0), Vector3f(0, 0, 1), 75);

    //SphericalCamera cam(Vector3f(10, 0, 0), M_PI/2);
    //SphericalCamera cam(Vector3f(0.17, 0.84, 0.2), M_PI/2);
//PARAM
//sz = 2 for normal NDC coords
float sz = 2;
//END
    for (int y = 0; y < h; ++y) {
        float ndcy = sz * ((y / (h - 1.0f)) - 0.5f);
        for (int x = 0; x < w; ++x) {
            float ndcx = sz * ((x / (w - 1.0f)) - 0.5f);
            
            Ray r = cam.generateRay(Vector2f(ndcx, ndcy));
            //Ray r = cam.generateRay(Vector2f(1.0*x/w, 1.0*y/h));

            Hit h;
            Vector3f color = traceRay(r, cam.getTMin(), _args.bounces, h);

            image.setPixel(x, y, color);
        }
    }
    if (_args.output_file.size()) {
        image.savePNG(_args.output_file);
    }
}

Vector3f Renderer::traceRay(const Ray &r, float tmin, int bounces, Hit &h) const {
//MAKE CONST LATER
//int maxSteps = 150;
//float minDist = 1e-9;

//spherical
int maxSteps = 150;
float minDist = 1e-5;

//int maxSteps = 120;
//float minDist = 1e-9;
// LATER
    float t = tmin;
    int steps = 0;
    Vector3f color;
    for(; steps < maxSteps; steps++) {
        float dist = _fractal->DE(r.getOrigin() + t * r.getDirection());
//cout<<dist<<endl;
//Vector3f pos = r.getOrigin() + t * r.getDirection();
//cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
        t += dist;
        if(dist < minDist) break;
    }

//cout<<steps<<" "<<t<<endl;
    return Vector3f(1.0 - 1.0*steps/maxSteps);
}

// Original code (commented)
/*void
Renderer::Render()
{
    int w = _args.width;
    int h = _args.height;

    Image image(w, h);
    Image nimage(w, h);
    Image dimage(w, h);

    // loop through all the pixels in the image
    // generate all the samples

    // This look generates camera rays and callse traceRay.
    // It also write to the color, normal, and depth images.
    // You should understand what this code does.
    Camera* cam = _scene.getCamera();
    for (int y = 0; y < h; ++y) {
        float ndcy = 2 * (y / (h - 1.0f)) - 1.0f;
        for (int x = 0; x < w; ++x) {
            float ndcx = 2 * (x / (w - 1.0f)) - 1.0f;
            // Use PerspectiveCamera to generate a ray.
            // You should understand what generateRay() does.
            Ray r = cam->generateRay(Vector2f(ndcx, ndcy));

            Hit h;
            Vector3f color = traceRay(r, cam->getTMin(), _args.bounces, h);

            image.setPixel(x, y, color);
            nimage.setPixel(x, y, (h.getNormal() + 1.0f) / 2.0f);
            float range = (_args.depth_max - _args.depth_min);
            if (range) {
                dimage.setPixel(x, y, Vector3f((h.t - _args.depth_min) / range));
            }
        }
    }
    // END SOLN

    // save the files 
    if (_args.output_file.size()) {
        image.savePNG(_args.output_file);
    }
    if (_args.depth_file.size()) {
        dimage.savePNG(_args.depth_file);
    }
    if (_args.normals_file.size()) {
        nimage.savePNG(_args.normals_file);
    }
}

Vector3f
Renderer::traceRay(const Ray &r,
    float tmin,
    int bounces,
    Hit &h) const
{
    // The starter code only implements basic drawing of sphere primitives.
    // You will implement phong shading, recursive ray tracing, and shadow rays.

    if (_scene.getGroup()->intersect(r, tmin, h)) {
        Vector3f color = _scene.getAmbientLight() * h.material->getDiffuseColor();
        Vector3f tolight(0), intensity(0);
        float distToLight;
        Vector3f inter = r.pointAtParameter(h.t);

        for(int i=0; i<_scene.getNumLights(); i++) {
            _scene.getLight(i)->getIllumination(inter, tolight, intensity, distToLight);
            if(_args.shadows) {
                Ray shadowRay(inter, tolight.normalized());
                Hit shadowHit;
                bool hasHit = _scene.getGroup()->intersect(shadowRay, 0.01, shadowHit); 
            
                if(!hasHit || shadowHit.t >= distToLight) {
                    color += h.material->shade(r, h, tolight, intensity);
                }
            } else {
                color += h.material->shade(r, h, tolight, intensity);
            }
        }
        if(bounces > 0) {
            Vector3f newDir = r.getDirection() - 2 * Vector3f::dot(r.getDirection(), h.normal) * h.normal;
            Ray newRay(inter, newDir.normalized());
            Hit newHit;
            color += h.material->getSpecularColor() * traceRay(newRay, 0.01, bounces-1, newHit);
        }
        return color;
    } else {
        return _scene.getBackgroundColor(r.getDirection());
    };
}*/


#include "Material.h"
Vector3f Material::shade(const Ray &ray,
    const Hit &hit,
    const Vector3f &dirToLight,
    const Vector3f &lightIntensity)
{
    // implement Diffuse and Specular phong terms
    Vector3f diffuse = clamp(dirToLight, hit.normal) * lightIntensity * hit.material->_diffuseColor;
    Vector3f reye = ray.getDirection() - 2 * Vector3f::dot(ray.getDirection(), hit.normal) * hit.normal;
    Vector3f specular = pow(clamp(dirToLight, reye), hit.material->_shininess) * lightIntensity * hit.material->_specularColor;
    return diffuse + specular;
}

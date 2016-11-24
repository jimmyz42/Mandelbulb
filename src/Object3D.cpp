#include "Object3D.h"

bool Sphere::intersect(const Ray &r, float tmin, Hit &h) const
{
    // BEGIN STARTER

    // We provide sphere intersection code for you.
    // You should model other intersection implementations after this one.

    // Locate intersection point ( 2 pts )
    const Vector3f &rayOrigin = r.getOrigin(); //Ray origin in the world coordinate
    const Vector3f &dir = r.getDirection();

    Vector3f origin = rayOrigin - _center;      //Ray origin in the sphere coordinate

    float a = dir.absSquared();
    float b = 2 * Vector3f::dot(dir, origin);
    float c = origin.absSquared() - _radius * _radius;

    // no intersection
    if (b * b - 4 * a * c < 0) {
        return false;
    }

    float d = sqrt(b * b - 4 * a * c);

    float tplus = (-b + d) / (2.0f*a);
    float tminus = (-b - d) / (2.0f*a);

    // the two intersections are at the camera back
    if ((tplus < tmin) && (tminus < tmin)) {
        return false;
    }

    float t = 10000;
    // the two intersections are at the camera front
    if (tminus > tmin) {
        t = tminus;
    }

    // one intersection at the front. one at the back 
    if ((tplus > tmin) && (tminus < tmin)) {
        t = tplus;
    }

    if (t < h.getT()) {
        Vector3f normal = r.pointAtParameter(t) - _center;
        normal = normal.normalized();
        h.set(t, this->material, normal);
        return true;
    }
    // END STARTER
    return false;
}

// Add object to group
void Group::addObject(Object3D *obj) {
    m_members.push_back(obj);
}

// Return number of objects in group
int Group::getGroupSize() const {
    return (int)m_members.size();
}

bool Group::intersect(const Ray &r, float tmin, Hit &h) const
{
    // BEGIN STARTER
    // we implemented this for you
    bool hit = false;
    for (Object3D* o : m_members) {
        if (o->intersect(r, tmin, h)) {
            hit = true;
        }
    }
    return hit;
    // END STARTER
}

// Transform n dot P = d INTO n dot P + d = 0
Plane::Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
    this->normal = normal;
    this->d = -d;
}
bool Plane::intersect(const Ray &r, float tmin, Hit &h) const
{
    Vector3f rayOrigin = r.getOrigin();
    Vector3f dir = r.getDirection();
    float t = -(d + Vector3f::dot(this->normal, rayOrigin)) / Vector3f::dot(this->normal, dir);
    if(t > tmin && t < h.getT()) {
        h.set(t, this->material, this->normal);
        return true;
    }
    return false;
}
bool Triangle::intersect(const Ray &r, float tmin, Hit &h) const 
{
    Vector3f rayOrigin = r.getOrigin();
    Vector3f dir = r.getDirection();
    Matrix3f mat = Matrix3f(this->_v[0] - this->_v[1], this->_v[0] - this->_v[2], dir);
    bool singular = false;
    Matrix3f inv = mat.inverse(&singular, 1e-9);
    if(singular) return false;

    Vector3f sol = inv * (this->_v[0] - rayOrigin);
    if(sol[0] + sol[1] <= 1 && sol[0] >= 0 && sol[1] >= 0 && sol[2] > tmin && sol[2] < h.getT()) {
        Vector3f normal = (1-sol[0]-sol[1])*this->_normals[0] + sol[0]*this->_normals[1] + sol[1]*this->_normals[2];
        h.set(sol[2], this->material, normal.normalized());
        return true;
    }
    return false;
}


Transform::Transform(const Matrix4f &m,
    Object3D *obj) : _object(obj) {
    this->mInv = m.inverse(NULL, 1e-9);
}
bool Transform::intersect(const Ray &r, float tmin, Hit &h) const
{
    Vector3f osOrig = (this->mInv * Vector4f(r.getOrigin(), 1)).homogenized().xyz();
    // do not normalize! so that t stays the same
    Vector3f osDir = (this->mInv * Vector4f(r.getDirection(), 0)).xyz();
    Ray transR(osOrig, osDir);
    bool ret = this->_object->intersect(transR, tmin, h);
    if(ret) {
        h.normal = (this->mInv.transposed() * Vector4f(h.normal, 0)).xyz();
        // normalize normal !!
        h.normal.normalize();
    }
    return ret;
}

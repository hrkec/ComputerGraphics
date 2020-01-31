#ifndef RG3_HITTABLE_H
#define RG3_HITTABLE_H

#include "ray.h"

class material;

struct hit_record{
    double t;
    vec3 p;
    vec3 normal;
    material *mat;
};

class hittable{
public:
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const = 0;
};

#endif //RG3_HITTABLE_H

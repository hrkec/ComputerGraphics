#ifndef RG3_SPHERE_H
#define RG3_SPHERE_H

#include "hittable.h"

class sphere: public hittable{
public:
    vec3 center;
    double radius;
    material *mat;

    sphere() {}
    sphere(vec3 cen, double r, material *m) : center(cen), radius(r), mat(m)  {};
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& record) const;

};

bool sphere::hit(const class ray &r, double tmin, double tmax, hit_record& record) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius*radius;
    double d = b * b - a * c;
    
    if (d > 0) {
        double temp = (-b - sqrt(d))/a;
        if (temp > tmin && temp < tmax) {
            record.t = temp;
            record.p = r.point_at_parameter(record.t);
            record.normal = (record.p - center) / radius;
            record.mat = mat;
            return true;
        }
        temp = (-b + sqrt(d)) / a;

        if (temp > tmin && temp < tmax) {
            record.t = temp;
            record.p = r.point_at_parameter(record.t);
            record.normal = (record.p - center) / radius;
            record.mat = mat;
            return true;
        }
    }

    return false;
}

#endif //RG3_SPHERE_H

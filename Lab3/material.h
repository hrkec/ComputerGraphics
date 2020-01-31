#ifndef RG3_MATERIAL_H
#define RG3_MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "random.h"

class material{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 reflect(const vec3&v, const vec3& n){
    return v - 2 * dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, double ni_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    double dt = dot(uv, n);
    double d = 1.0 - ni_nt * ni_nt * (1 - dt * dt);
    if (d > 0) {
        refracted = ni_nt * (uv - n * dt) - n * sqrt(d);
        return true;
    }
    else
        return false;
}

double schlick(double cosine, double n){
    double r0 = (1 - n) / (1 + n);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

class lambertian: public material{
public:
    vec3 albedo;

    lambertian(const vec3& a) : albedo(a){}
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override{
        attenuation = albedo;
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        return true;
    }
};

class metal: public material{
public:
    vec3 albedo;
    double fuzz;

    metal(const vec3& a, double f) : albedo(a){
        if(f < 1) fuzz = f;
        else fuzz = 1;
    }

    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override{
        attenuation = albedo;
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

class glass : public material {
public:
    double idx;

    glass(double ri) : idx(ri) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        attenuation = vec3(1.0, 1.0, 1.0);
        double ni_nt, reflect_p, cosine;
        vec3 normal, refracted;
        vec3 reflected = reflect(r_in.direction(), rec.normal);

        if (dot(r_in.direction(), rec.normal) > 0) {
            normal = -rec.normal;
            ni_nt = idx;
            cosine = idx * dot(r_in.direction(), rec.normal)
                     / r_in.direction().length();
        } else {
            normal = rec.normal;
            ni_nt = 1.0 / idx;
            cosine = -1 * dot(r_in.direction(), rec.normal)
                     / r_in.direction().length();
        }

        if (refract(r_in.direction(), normal, ni_nt, refracted)) {
            reflect_p = schlick(cosine, idx);
        } else {
            reflect_p = 1.0;
        }

        if(random_double() < reflect_p){
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }
};

#endif //RG3_MATERIAL_H

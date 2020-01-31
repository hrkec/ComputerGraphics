//
// Created by hrkec on 10/12/2019.
//

#ifndef RG2_VEC3D_H
#define RG2_VEC3D_H

#endif //RG2_VEC3D_H

#include <iostream>

class vec3d{
public:
    // Components
    double x_, y_, z_;

    // Constructors
    vec3d(double = 0, double = 0, double = 0);
    vec3d(vec3d, double);

    vec3d operator+(const vec3d&) const;
    vec3d operator-(const vec3d&) const;
    vec3d operator*(double) const;
    vec3d operator/(double) const;

    double operator*(const vec3d&) const;
    vec3d cross(const vec3d&) const;

    double norm() const;
    vec3d unit() const;
    void normalize();
    vec3d inverse() const;
    vec3d projection(const vec3d&) const;
    double angle(const vec3d&) const;
    void rotate(const vec3d&, double);

    friend std::ostream& operator<<(std::ostream&, const vec3d&);
};
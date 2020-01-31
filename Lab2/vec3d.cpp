//
// Created by hrkec on 10/12/2019.
//

#include "vec3d.h"
#include <math.h>

vec3d::vec3d(double x, double y, double z) : x_(x), y_(y), z_(z) {}

vec3d::vec3d(vec3d vector, double magnitude) {
    vector.normalize();
    if(magnitude == 0){
        throw "Error : Magnitude = 0";
    }
    x_ = magnitude * vector.x_;
    y_ = magnitude * vector.y_;
    z_ = magnitude * vector.z_;
}

vec3d vec3d::operator+(const vec3d &vector) const {
    return vec3d(x_ + vector.x_, y_ + vector.y_, z_ + vector.z_);
}

vec3d vec3d::operator-(const vec3d &vector) const {
    return vec3d(x_ - vector.x_, y_ - vector.y_, z_ - vector.z_);
}

vec3d vec3d::operator*(double value) const {
    return vec3d(x_ * value, y_ * value, z_ * value);
}

vec3d vec3d::operator/(double value) const {
    return vec3d(x_ / value, y_ / value, z_ / value);
}

double vec3d::operator*(const vec3d &vector) const {
    return x_ * vector.x_ + y_ * vector.y_ + z_ * vector.z_;
}

vec3d vec3d::cross(const vec3d &vector) const {
    vec3d result(y_ * vector.z_ - z_ * vector.y_, z_ * vector.x_ - x_ * vector.z_, x_ * vector.y_ - y_ * vector.x_);
    return result;
}

double vec3d::norm() const {
    return sqrt(x_*x_ + y_*y_ + z_*z_);
}

vec3d vec3d::unit() const {
    double n = norm();
    if(n == 0)
        throw "Norm = 0!";
    return vec3d(x_ / n, y_ / n, z_ / n);
}

void vec3d::normalize() {
    double n = norm();
    if(n == 0)
        throw "Norm = 0!";
    x_ = x_ / n;
    y_ = y_ / n;
    z_ = z_ / n;
}

vec3d vec3d::inverse() const {
    return (*this)*-1;
}

vec3d vec3d::projection(const vec3d &vector) const {
    double n = vector.norm();
    if (n == 0){
        throw "Invalid";
    }
    return vector.unit()*(operator*(vector)/n);
}

double vec3d::angle(const vec3d &vector) const {
    double n = norm();
    double on = vector.norm();
    return acos(operator*(vector)/(n * on));
}

void vec3d::rotate(const vec3d &vec, double angle) {
    double rotationMatrix[4][4];
    double L = (vec.x_*vec.x_ + vec.y_ * vec.y_ + vec.z_ * vec.z_);
    double u2 = vec.x_ * vec.x_;
    double v2 = vec.y_ * vec.y_;
    double w2 = vec.z_ * vec.z_;

    rotationMatrix[0][0] = (u2 + (v2 + w2) * cos(angle)) / L;
    rotationMatrix[0][1] = (vec.x_ * vec.y_ * (1 - cos(angle)) - vec.z_ * sqrt(L) * sin(angle)) / L;
    rotationMatrix[0][2] = (vec.x_ * vec.z_ * (1 - cos(angle)) + vec.y_ * sqrt(L) * sin(angle)) / L;
    rotationMatrix[0][3] = 0.0;

    rotationMatrix[1][0] = (vec.x_ * vec.y_ * (1 - cos(angle)) + vec.z_ * sqrt(L) * sin(angle)) / L;
    rotationMatrix[1][1] = (v2 + (u2 + w2) * cos(angle)) / L;
    rotationMatrix[1][2] = (vec.y_ * vec.z_ * (1 - cos(angle)) - vec.x_ * sqrt(L) * sin(angle)) / L;
    rotationMatrix[1][3] = 0.0;

    rotationMatrix[2][0] = (vec.x_ * vec.z_ * (1 - cos(angle)) - vec.y_ * sqrt(L) * sin(angle)) / L;
    rotationMatrix[2][1] = (vec.y_ * vec.z_ * (1 - cos(angle)) + vec.x_ * sqrt(L) * sin(angle)) / L;
    rotationMatrix[2][2] = (w2 + (u2 + v2) * cos(angle)) / L;
    rotationMatrix[2][3] = 0.0;

    rotationMatrix[3][0] = 0.0;
    rotationMatrix[3][1] = 0.0;
    rotationMatrix[3][2] = 0.0;
    rotationMatrix[3][3] = 1.0;

    double inputMatrix[4][1] = {0.0, 0.0, 0.0};
    double outputMatrix[4][1] = {0.0, 0.0, 0.0, 0.0};

    inputMatrix[0][0] = x_;
    inputMatrix[1][0] = y_;
    inputMatrix[2][0] = z_;
    inputMatrix[3][0] = 1.0;

    for(int i = 0; i < 4; i++ ){
        for(int j = 0; j < 1; j++){
            outputMatrix[i][j] = 0;
            for(int k = 0; k < 4; k++){
                outputMatrix[i][j] += rotationMatrix[i][k] * inputMatrix[k][j];
            }
        }
    }

    x_ = (float)outputMatrix[0][0];
    y_ = (float)outputMatrix[1][0];
    z_ = (float)outputMatrix[2][0];
}

std::ostream& operator << (std::ostream& out, const vec3d& vec){
    out << "[" << vec.x_ << ", " << vec.y_ << ", " << vec.z_ << "]";
    return out;
}
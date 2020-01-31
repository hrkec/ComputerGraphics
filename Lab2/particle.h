//
// Created by hrkec on 10/12/2019.
//

#ifndef RG2_PARTICLE_H
#define RG2_PARTICLE_H

#endif //RG2_PARTICLE_H

#include "vec3d.h"
#include "constants.h"
#include "randdouble.h"

class Particle{
private:
    int mass_;
    vec3d velocity_;
    vec3d position_;

public:
    Particle(double = rand() % (MAX_MASS - MIN_MASS) + MIN_MASS,
        vec3d = vec3d(vec3d(rand_double(), rand_double(), rand_double()), MIN_INIT_VELOCITY + rand() % (MAX_INIT_VELOCITY - MIN_INIT_VELOCITY)),
        vec3d = vec3d((1-2*rand_double())*LENGTH, (1-2*rand_double())*LENGTH, (1-2*rand_double())*LENGTH));

    void advance(double, vec3d = vec3d(0.0, 0.0, 0.0));

    void draw(int);

    int getMass() const {
        return mass_;
    }

    void setMass(int mass) {
        mass_ = mass;
    }

    const vec3d &getVelocity() const {
        return velocity_;
    }

    void setVelocity(const vec3d &velocity) {
        velocity_ = velocity;
    }

    const vec3d &getPosition() const {
        return position_;
    }

    void setPosition(const vec3d &position) {
        position_ = position;
    }

    ~Particle();

    unsigned long int lifetime;
    unsigned long int createdAt;
};
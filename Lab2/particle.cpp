//
// Created by hrkec on 10/12/2019.
//

#include "particle.h"
#include "randdouble.h"
#include <chrono>

Particle::Particle(double mass, vec3d velocity, vec3d position) : mass_(mass), position_(position), velocity_(velocity) {
    createdAt = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    lifetime = (rand() % 11 + 0.5) * 2500;
}

void Particle::advance(double t, vec3d force) {
    vec3d acceleration = force / mass_;

    velocity_ = velocity_+ acceleration * (t / 1000.0);

    if(velocity_.norm() >= MAX_VELOCITY){
        velocity_ = vec3d(velocity_.unit(), MAX_VELOCITY);
    }

    position_ = position_ + velocity_ * (t / 1000.0);

    if(position_.x_ <= -LENGTH){
        position_.x_= LENGTH;
    } else if (position_.x_ >= LENGTH){
        position_.x_ = -LENGTH;
    }

    if(position_.y_ <= -LENGTH){
        position_.y_= LENGTH;
    } else if (position_.y_ >= LENGTH){
        position_.y_ = -LENGTH;
    }

    if(position_.z_ <= -LENGTH){
        position_.z_= LENGTH;
    } else if (position_.z_ >= LENGTH){
        position_.z_ = -LENGTH;
    }
}

Particle::~Particle() {}
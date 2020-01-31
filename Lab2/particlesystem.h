//
// Created by hrkec on 10/12/2019.
//

#ifndef RG2_PARTICLESYSTEM_H
#define RG2_PARTICLESYSTEM_H

#endif //RG2_PARTICLESYSTEM_H

#include <GL/glut.h>
#include "particle.h"
#include <vector>

class ParticleSystem{
private:
    std::vector<Particle> particles;
    vec3d gravityPoint;
//    unsigned long int creationTime = 8000, timeOld, lastCreated;
//    GLuint tex_2d;

public:
    ParticleSystem(int);

    void advance(double);

    void setGravity(vec3d = vec3d(0, 0, 0));

    bool addParticles(int);

    bool deleteParticles(int);

    void draw();

    ~ParticleSystem();
};
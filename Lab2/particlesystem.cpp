//
// Created by hrkec on 10/12/2019.
//

#include "particlesystem.h"
#include <GL/glut.h>
#include <math.h>
#include <chrono>
//#include "SOIL.h"

ParticleSystem::ParticleSystem(int n) {
    if(n > MAX_PARTICLES)
        n = MAX_PARTICLES;

    for(int i = 0; i < n; i++){
        Particle particle;
        particles.push_back(particle);
    }

}

void ParticleSystem::advance(double time) {
     for(auto& particle : particles){
         vec3d force = vec3d((gravityPoint - particle.getPosition()).unit(), FORCE_MAG);
         particle.advance(time, force);
     }
}

void ParticleSystem::setGravity(vec3d gravity) {
    gravityPoint = gravity;
}

bool ParticleSystem::addParticles(int n) {
    int i;
    for(i = 0; i < n && particles.size() < MAX_PARTICLES; i++){
        Particle particle;
        particles.push_back(particle);
    }
    return (i >= n);
}

bool ParticleSystem::deleteParticles(int n) {
    int i;
    for(i = 0; i < n && particles.size() > 0; i++){
        particles.pop_back();
    }
    return (i >= n);
}

void ParticleSystem::draw() {
    double k;
    for(auto& particle : particles){
        vec3d position = particle.getPosition();
        k = (gravityPoint - position).norm() / (1.5 * LENGTH);
        k -= 0.7;
        glColor4f(1, 0.647 + k, 0, 1 - k);
//        glColor4f(1, k, 1 - k, 1);
//        glBegin(GL_POINTS);
//        glVertex3f(position.x_, position.y_, position.z_);
//        glEnd();
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glBegin(GL_QUADS);
        glNormal3f(0.0, 0.0, 1.0);
        glTexCoord2d(1, 1); glVertex3f(position.x_ + 2.5, position.y_ - 2.5, 0.0);
        glTexCoord2d(1, 0); glVertex3f(position.x_ + 2.5, position.y_ + 2.5, 0.0);
        glTexCoord2d(0, 0); glVertex3f(position.x_ - 2.5, position.y_ + 2.5, 0.0);
        glTexCoord2d(0, 1); glVertex3f(position.x_ - 2.5, position.y_ - 2.5, 0.0);
        glEnd();

//        glBegin(GL_QUADS);
//        glNormal3f(0.0, 0.0, 1.0);
//        glTexCoord2d(1, 1); glVertex3f(0.0, 0.0, 0.0);
//        glTexCoord2d(1, 0); glVertex3f(0.0, 1.0, 0.0);
//        glTexCoord2d(0, 0); glVertex3f(1.0, 1.0, 0.0);
//        glTexCoord2d(0, 1); glVertex3f(1.0, 0.0, 0.0);
//        glEnd();

        glDisable(GL_TEXTURE_2D);

    }
}

ParticleSystem::~ParticleSystem() {}

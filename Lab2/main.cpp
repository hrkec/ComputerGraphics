#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include "particlesystem.h"
#include "SOIL.h"

ParticleSystem particleSystem(NUMBER_OF_PARTICLES);

GLuint width = 800;
GLuint height = 800;
GLuint tex_2d;

void init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char theKey, int mouseX, int mouseY);
void mouse(int mouseX, int mouseY);

int main(int argc, char** argv) {
    srand(time(0));
    particleSystem.setGravity();

    init();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Računalna grafika - sustav čestica");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouse);

    tex_2d = SOIL_load_OGL_texture
            (
                    "smoke.bmp",
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
            );

    glutMainLoop();
    return 0;
}

void init(){
    glPointSize(1);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void reshape(int w, int h){
    width = w; height = h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-LENGTH, LENGTH, -LENGTH, LENGTH, -LENGTH, LENGTH);
    glMatrixMode(GL_MODELVIEW);
}

void display(){
    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();

//    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    particleSystem.advance(DELTA);
    particleSystem.draw();

    glDisable(GL_TEXTURE_2D);
//    glPopAttrib();
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    switch (key){
        case 'a':
        case 'A':
            particleSystem.addParticles(100);
            break;
        case'd':
        case'D':
            particleSystem.deleteParticles(100);
            break;
        case's':
        case'S':
            SOIL_save_screenshot("awesome.bmp", SOIL_SAVE_TYPE_BMP, 0, 0, 1024, 768);
            break;

        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouse(int x, int y){
    double ww = double(x) / width;
    double wh = double(y) / height;

    particleSystem.setGravity(vec3d((2 * ww - 1)*LENGTH, (1 - 2 * wh) * LENGTH, 0));
}
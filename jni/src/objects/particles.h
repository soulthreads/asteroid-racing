#ifndef PARTICLES_H
#define PARTICLES_H

#include "engine.h"

class Particles
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint u_ColorHandle;
    GLuint u_SizeHandle;
    GLuint u_TexUnitHandle;

    GLuint a_PositionHandle;
    GLuint a_LifeTimeHandle;

    GLuint particleTex;
    float particleSize;
    float decayFactor;

    vec3 particlesColor;
    GLuint maxCount;
    typedef struct {
        vec3 position;
        vec3 velocity;
        float lifeTime;
    } particle;

    vector<particle> particles;
    vector<GLfloat> vertexData;
    GLuint stride;
    GLuint components;
    size_t cursor;

public:
    Particles(vec3 color, GLuint maxParticles, GLfloat size, float decay);
    ~Particles();
    void draw (Engine &engine);
    void addParticles (vec3 position, vec3 velocity, int count);
};

#endif // PARTICLES_H

#ifndef PARTICLES_H
#define PARTICLES_H

#include "engine.h"

struct particle {
    vec3 position;
    vec3 velocity;
    float lifeTime;
};

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

    vector<particle> particles;
    vector<GLfloat> vertexData;
    GLuint stride;
    GLuint components;
    size_t cursor;

    int token;
    void init();

public:
    Particles (vec3 color, GLuint maxParticles, GLfloat size, float decay);
    ~Particles ();
    void draw ();
    void addParticles (vec3 position, vec3 velocity, int count);
    void setParticlesColor(const vec3 &value);
    void setParticleSize(float value);
    vector<particle> &getParticles();
    void reset();
};

#endif // PARTICLES_H

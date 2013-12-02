#ifndef PARTICLES_H
#define PARTICLES_H

#include "engine.h"

using namespace std;

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

    glm::vec3 particlesColor;
    GLuint maxCount;
    typedef struct {
        glm::vec3 position;
        glm::vec3 velocity;
        float lifeTime;
    } particle;

    vector<particle> particles;
    vector<GLfloat> vertexData;
    GLuint stride;
    GLuint components;
    size_t cursor;

public:
    Particles(glm::vec3 color, GLuint maxParticles, GLfloat size, float decay);
    ~Particles();
    void draw (Engine &engine);
    void addParticles (glm::vec3 position, glm::vec3 velocity, int count);
};

#endif // PARTICLES_H

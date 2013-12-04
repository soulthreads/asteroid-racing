#ifndef SHIP_H
#define SHIP_H

#include "engine.h"
#include "particles.h"
#include "asteroids.h"

class Ship
{
private:
    GLuint program;

    GLuint u_MvpMatrixHandle, u_MvMatrixHandle;
    GLuint u_LightPosHandle;
    GLuint u_EyePosHandle;

    GLuint a_PositionHandle;
    GLuint a_NormalHandle;
    GLuint a_ColorHandle;

    GLuint vbo;
    GLuint stride, nvertices;

    mat4 modelMatrix, mvMatrix, mvpMatrix;

    unique_ptr<Particles> throttleParticles;
    unique_ptr<Particles> fireParticles;
    float throttleTime;

public:
    Ship (Engine &engine);
    ~Ship ();

    void draw (Engine &engine);
    void update(Engine &engine, vector<asteroid> &asteroids);
};

#endif // SHIP_H

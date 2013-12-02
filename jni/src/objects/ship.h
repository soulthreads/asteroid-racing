#ifndef SHIP_H
#define SHIP_H

#include "engine.h"
#include "util/assets.h"
#include "util/shader.h"
#include "particles.h"

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

    glm::mat4 modelMatrix, mvMatrix, mvpMatrix;

    unique_ptr<Particles> particles;

public:
    Ship (Engine &engine);
    ~Ship ();

    void draw (Engine &engine);
    void update(Engine &engine);
};

#endif // SHIP_H

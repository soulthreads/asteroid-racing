#ifndef CUBE_H
#define CUBE_H

#include "engine.h"

class Cube
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint a_PositionHandle;
    GLuint a_ColorHandle;

    GLuint vbo, ibo;

public:
    Cube ();
    ~Cube ();
    void draw (mat4 mvp);
};

#endif // CUBE_H

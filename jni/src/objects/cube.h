#ifndef CUBE_H
#define CUBE_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "util/assets.h"
#include "util/shader.h"

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
    void draw (glm::mat4 mvp);
};

#endif // CUBE_H

#ifndef SKYBOX_H
#define SKYBOX_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "util/assets.h"
#include "util/shader.h"

#include "sun.h"

class Skybox
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint u_TextureUnitHandle;
    GLuint a_PositionHandle;

    GLuint vbo, ibo;
    GLuint textures;

    unique_ptr<Sun> sun;

public:
    Skybox (Engine &engine);
    ~Skybox ();
    void draw (Engine &engine);
};

#endif // SKYBOX_H

#ifndef SKYBOX_H
#define SKYBOX_H

#include "engine.h"

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

    int token;
    void init(Engine &engine);

public:
    Skybox (Engine &engine);
    ~Skybox ();
    void draw (Engine &engine);
};

#endif // SKYBOX_H

#include "skybox.h"

#define LOG_TAG "skybox"
#include "util/logs.h"

Skybox::Skybox() {
    init ();
    sun = unique_ptr<Sun> (new Sun (vec3(engine.state.lightPos), engine.width/6.0));
}

Skybox::~Skybox () {
    if (program) {
        glDeleteProgram (program);
        glDeleteBuffers (1, &vbo);
        vbo = 0;
        glDeleteBuffers (1, &ibo);
        ibo = 0;
        glDeleteTextures (1, &textures);
        textures = 0;
    }
}

void Skybox::init () {
    token = engine.token;
    program = buildProgramFromAssets ("shaders/skybox.vsh", "shaders/skybox.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_TextureUnitHandle = glGetUniformLocation (program, "u_TextureUnit");
    a_PositionHandle = glGetAttribLocation (program, "a_Position");

    GLfloat skyboxVertices [] = {
        // front vertices
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        // back vertices
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };

    GLushort skyboxElements [] = {
        // front
        2, 1, 0,
        0, 3, 2,
        // top
        6, 2, 3,
        3, 7, 6,
        // back
        5, 6, 7,
        7, 4, 5,
        // bottom
        1, 5, 4,
        4, 0, 1,
        // left
        3, 0, 4,
        4, 7, 3,
        // right
        6, 5, 1,
        1, 2, 6
    };

    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glGenBuffers (1, &ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (skyboxElements), skyboxElements, GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

    const std::string paths[] = {"textures/left.png", "textures/right.png",
                           "textures/top.png", "textures/bottom.png",
                           "textures/front.png", "textures/back.png"};
    textures = loadCubeMapFromAssets (paths);

    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, textures);
}

void Skybox::draw () {
    if (engine.token != token) init ();

    glUseProgram (program);

    mat4 mvp = engine.projectionMatrix * engine.viewMatrix;
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (mvp));

//    glActiveTexture (GL_TEXTURE0);
//    glBindTexture (GL_TEXTURE_CUBE_MAP, textures);
    glUniform1i (u_TextureUnitHandle, 0);

    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (a_PositionHandle);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    int size; glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    glDepthFunc (GL_LEQUAL);
    glDrawElements (GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    glDepthFunc (GL_LESS);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ARRAY_BUFFER, 0);

    sun->draw (engine);
}

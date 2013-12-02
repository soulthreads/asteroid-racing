#include "cube.h"

Cube::Cube () {
    program = buildProgramFromAssets ("shaders/simple.vsh", "shaders/simple.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_ColorHandle = glGetAttribLocation (program, "a_Color");

    GLfloat cubeVertices [] = {
        // front vertices    front colors
        -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
        // back vertices     back colors
        -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f
    };

    GLushort cubeElements [] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        3, 2, 6,
        6, 7, 3,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // left
        4, 0, 3,
        3, 7, 4,
        // right
        1, 5, 6,
        6, 2, 1
    };

    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers (1, &ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (cubeElements), cubeElements, GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

Cube::~Cube () {
    if (vbo) {
        glDeleteBuffers (1, &vbo);
        vbo = 0;
    }

    if (ibo) {
        glDeleteBuffers (1, &ibo);
        ibo = 0;
    }
}

void Cube::draw (glm::mat4 mvp) {
    glUseProgram (program);

    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, glm::value_ptr (mvp));

    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    glEnableVertexAttribArray (a_PositionHandle);
    glVertexAttribPointer (a_ColorHandle, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void *)12);
    glEnableVertexAttribArray (a_ColorHandle);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);
    int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements (GL_TRIANGLES, size/sizeof (GLushort), GL_UNSIGNED_SHORT, 0);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
}

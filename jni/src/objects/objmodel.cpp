#include "objmodel.h"

using namespace std;

ObjModel::ObjModel(const string &pathToObj, const string &pathToMtl)
{
    stride = 3 * 3 * sizeof(GLfloat);

    program = buildProgramFromAssets ("shaders/obj.vsh", "shaders/obj.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_MvMatrixHandle = glGetUniformLocation (program, "u_MvMatrix");
    u_LightPosHandle = glGetUniformLocation (program, "u_LightPos");
    u_EyePosHandle = glGetUniformLocation (program, "u_EyePos");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_NormalHandle = glGetAttribLocation (program, "a_Normal");
    a_ColorHandle = glGetAttribLocation (program, "a_Color");

    vbo = loadObjFromAssets (pathToObj, pathToMtl, numOfVertices);
}

ObjModel::~ObjModel () {
    if (vbo) {
        glDeleteBuffers (1, &vbo);
        vbo = 0;
    }
}

void ObjModel::draw (glm::mat4 mvp, glm::mat4 mv, glm::vec3 lighPos, glm::vec3 eyePos) {
    glUseProgram (program);

    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, glm::value_ptr (mvp));
    glUniformMatrix4fv (u_MvMatrixHandle, 1, GL_FALSE, glm::value_ptr (mv));
    glUniform3fv (u_LightPosHandle, 1, glm::value_ptr (lighPos));
    glUniform3fv (u_EyePosHandle, 1, glm::value_ptr (eyePos));

    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_NormalHandle, 3, GL_FLOAT, GL_FALSE, stride, (void *)12);
    glEnableVertexAttribArray (a_NormalHandle);

    glVertexAttribPointer (a_ColorHandle, 3, GL_FLOAT, GL_FALSE, stride, (void *)24);
    glEnableVertexAttribArray (a_ColorHandle);

    glDrawArrays (GL_TRIANGLES, 0, numOfVertices / 3);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void ObjModel::drawDepth () {
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glDrawArrays (GL_TRIANGLES, 0, numOfVertices / 3);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
}

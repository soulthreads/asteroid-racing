#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "engine.h"

class ObjModel
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint u_MvMatrixHandle;
    GLuint u_LightPosHandle;
    GLuint u_EyePosHandle;

    GLuint a_PositionHandle;
    GLuint a_NormalHandle;
    GLuint a_ColorHandle;

    GLuint vbo;
    GLuint stride;
    GLuint numOfVertices;

public:
    ObjModel (const string &pathToObj, const string &pathToMtl);
    ~ObjModel ();
    void draw (mat4 mvp, mat4 mv, vec3 lighPos, vec3 eyePos);
    void drawDepth();
};

#endif // OBJMODEL_H

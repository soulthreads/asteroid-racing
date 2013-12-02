#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <string>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assets.h"
#include "shader.h"

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
    void draw (glm::mat4 mvp, glm::mat4 mv, glm::vec3 lighPos, glm::vec3 eyePos);
    void drawDepth();
};

#endif // OBJMODEL_H

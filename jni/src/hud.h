#ifndef HUD_H
#define HUD_H

#include "engine.h"

using namespace std;

class Hud
{
private:
    GLuint program;

    GLuint u_MvpMatrixHandle;
    GLuint u_TexUnitHandle;

    GLuint a_PositionHandle;
    GLuint a_TexCoordsHandle;

    vector<GLfloat> vertexData;

    GLuint stride;

    GLuint controllerTex, controllerBgTex, throttleTex, fireTex;
    glm::vec3 controllerCenter, throttleCenter, fireCenter;
    glm::vec3 controllerOffset;
    static const float controllerBgSize = 0.4;
    static const float controllerSize = 0.2;
    static const float throttleSize = 0.2;
    static const float fireSize = 0.2;

    glm::mat4 modelMatrix;

public:
    Hud(Engine &engine);
    ~Hud ();
    void handleTouch (Engine &engine, float x, float y);
    void draw (Engine &engine);
};

#endif // HUD_H

#ifndef HUD_H
#define HUD_H

#include "engine.h"
#include "objects/ship.h"

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
    vec3 controllerCenter, throttleCenter, fireCenter;
    vec3 controllerOffset;
    static const float controllerBgSize = 0.4;
    static const float controllerSize = 0.2;
    static const float throttleSize = 0.2;
    static const float fireSize = 0.2;

    mat4 modelMatrix;

    bool rotating;

    int token;
    void init(Engine &engine);

public:
    Hud(Engine &engine);
    ~Hud ();
    void handleTouch (Engine &engine, Ship &ship, float x, float y);
    void draw (Engine &engine);
    bool getRotating() const;
    void setRotating(bool value);
};

#endif // HUD_H

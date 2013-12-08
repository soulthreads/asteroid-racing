#ifndef HUD_H
#define HUD_H

#include "engine.h"

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
    vec2 controllerOffset;
    float controllerBgSize = 0.4;
    float controllerSize = 0.2;
    float throttleSize = 0.2;
    float fireSize = 0.2;

    mat4 modelMatrix;

    bool rotating;
    bool rSet, tSet, fSet;

    int token;
    void init();

public:
    Hud ();
    ~Hud ();
    void handleTouch (float x, float y);
    void draw ();
    bool getRotating() const;
    void setRotating(bool value);
    void newTouchEvent();
    void touchUp(float x, float y);
};

#endif // HUD_H

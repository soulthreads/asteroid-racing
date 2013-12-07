#ifndef SUN_H
#define SUN_H

#include "engine.h"

class Sun
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint u_TexUnitHandle;
    GLuint u_SizeHandle;

    GLuint a_PositionHandle;

    GLuint sunTex;

    vec3 position;
    float size;

    int token;
    void init ();

public:
    Sun(vec3 sunPosition, float sunSize);
    ~Sun ();
    void draw ();
};

#endif // SUN_H

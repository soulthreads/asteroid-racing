#ifndef ASTEROID_H
#define ASTEROID_H

#include "engine.h"

class Asteroids
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint u_MvMatrixHandle;
    GLuint u_LightPosHandle;

    GLuint a_PositionHandle;
    GLuint a_NormalHandle;

    typedef struct {
        GLuint vbo, ibo;
        vec3 position;
        vec3 velocity;
        quat orientation;
        quat rot;
        float radius;
    } asteroid;

    GLuint vbo, ibo;
    vector <asteroid> asteroids;

    vector<vec3> icoVertices;
    vector<uvec3> icoIndexes;
    map <uint, GLushort> icoIndexCache;
    GLushort index;
    void createIcosphere(uint subdivisions);
    GLushort getMiddlePoint(GLushort p1, GLushort p2);

public:
    Asteroids ();
    ~Asteroids ();
    void draw (Engine &engine);
};

#endif // ASTEROID_H

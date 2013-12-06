#ifndef ASTEROID_H
#define ASTEROID_H

#include "engine.h"
#include "particles.h"

typedef struct {
    GLuint vbo;
    vector<GLfloat> vertices;
    vec3 position;
    vec3 velocity;
    quat orientation;
    quat rot;
    float radius;
    float stamina;
    bool blownUp;
} asteroid;

class Asteroids
{
private:
    GLuint program;
    GLuint u_MvpMatrixHandle;
    GLuint u_MvMatrixHandle;
    GLuint u_LightPosHandle;
    GLuint u_ColorHandle;
    GLuint u_Color2Handle;

    GLuint a_PositionHandle;
    GLuint a_NormalHandle;

    GLuint ibo;
    vector <asteroid> asteroids;
    unique_ptr<Particles> explosion;
    static const size_t particlesCount = 512;

    vector<vec3> icoVertices;
    vector<uvec3> icoIndexes;
    vector<GLushort> indexData;
    map <uint, GLushort> icoIndexCache;
    GLushort index;
    void createIcosphere(uint subdivisions);
    GLushort getMiddlePoint(GLushort p1, GLushort p2);

    int token;
    void init(Engine &engine);
    void updateVBOs();

public:
    Asteroids (Engine &engine);
    ~Asteroids ();
    void draw (Engine &engine);
    vector<asteroid> &getAsteroids();
    void addAsteroid(vec3 position, float radius);
};

#endif // ASTEROID_H

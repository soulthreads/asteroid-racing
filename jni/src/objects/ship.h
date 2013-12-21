#ifndef SHIP_H
#define SHIP_H

#include "engine.h"

class Asteroids;
class Particles;

class Ship
{
private:
    GLuint program;

    GLuint u_MvpMatrixHandle, u_MvMatrixHandle;
    GLuint u_LightPosHandle;
    GLuint u_EyePosHandle;

    GLuint a_PositionHandle;
    GLuint a_NormalHandle;
    GLuint a_ColorHandle;

    GLuint vbo;
    GLuint stride, nvertices;

    mat4 modelMatrix, mvMatrix, mvpMatrix;

    vec3 position, velocity;
    quat orientation;
    float shipSize = 0.8f;

    bool throttle, fire;
    unique_ptr<Particles> throttleParticles;
    unique_ptr<Particles> fireParticles;
    float throttleTime, fireTime;
    bool fireStopping;

    bool exploded;

    int token;
    void init();

    void explode();

public:
    Ship ();
    ~Ship ();

    void draw ();
    void update(Asteroids &as);
    vec3 getPosition() const;
    void setPosition(const vec3 &value);
    vec3 getVelocity() const;
    void setVelocity(const vec3 &value);
    quat getOrientation() const;
    void setOrientation(const quat &value);
    void setThrottle(bool value);
    void setFire(bool value);
    void saveState();
    void restoreState();
    void reset();
};

#endif // SHIP_H

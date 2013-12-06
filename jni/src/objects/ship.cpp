#include "ship.h"

using namespace std;

Ship::Ship (Engine &engine) {
    stride = 3 * 3 * sizeof (GLfloat);

    position = vec3 (0);
    velocity = vec3 (0);
    orientation = angleAxis (0.f, vec3 (0, 0, 1));

    restoreState (engine);

    throttle = false;
    fire = false;
    throttleParticles = unique_ptr<Particles> (new Particles (vec3(1,0.5,0.1), 1024,
                                                      engine.width/16.0, 1/256.0));
    fireParticles = unique_ptr<Particles> (new Particles (vec3(1,0.1,0.01), 512,
                                                          engine.width/16.0, 1/1024.0));
    throttleTime = 0; fireTime = 0;
}

Ship::~Ship () {
    if (vbo) {
        glDeleteBuffers (1, &vbo);
        vbo = 0;
    }
}

void Ship::init (Engine &engine) {
    token = engine.token;

    program = buildProgramFromAssets ("shaders/ship.vsh", "shaders/ship.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_MvMatrixHandle = glGetUniformLocation (program, "u_MvMatrix");
    u_LightPosHandle = glGetUniformLocation (program, "u_LightPos");
    u_EyePosHandle = glGetUniformLocation (program, "u_EyePos");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_NormalHandle = glGetAttribLocation (program, "a_Normal");
    a_ColorHandle = glGetAttribLocation (program, "a_Color");

    vbo = loadObjFromAssets ("objects/ship.obj", "objects/ship.mtl", nvertices);
}

void Ship::update (Engine &engine, vector<asteroid> &asteroids) {
    vec3 dv = orientation * vec3 (0, 0, 1);
    vec3 up = orientation * vec3 (0, 1, 0);
    vec3 rt = orientation * vec3 (1, 0, 0);

    float dt = engine.delta * 0.001;

    float speedFactor = length (velocity) > 0 ?
                (3.0-dot (normalize (velocity), normalize (dv))) / 2.0 : 1.0;

    if (throttle) {
        // TODO: add point lights from throttle
        velocity += dv * dt * speedFactor * 10.f;
        throttleParticles->addParticles (position-0.4f*dv,
                                 velocity-2.0f*dv, 2);
        throttleParticles->addParticles (position-0.4f*dv-0.06f*up+0.22f*rt,
                                 velocity-2.0f*dv, 2);
        throttleParticles->addParticles (position-0.4f*dv-0.06f*up-0.22f*rt,
                                 velocity-2.0f*dv, 2);
        throttleTime += dt * 0.1f;
    } else {
        throttleTime *= 0.98;
    }
    throttleParticles->setParticlesColor (mix (vec3(1,0.5,0.1), vec3(0.1, 0.5, 1), throttleTime));

    if (fire && !fireStopping && (fireTime < 8)) {
        fireTime += 8;
        for (int i = 0; i < 4; ++i) {
        fireParticles->addParticles (position + 0.12f*up + 0.2f*rt - 0.3f*i*dv,
                                     velocity + speedFactor * 100.f*dv, 1);
        fireParticles->addParticles (position + 0.12f*up - 0.2f*rt - 0.3f*i*dv,
                                     velocity + speedFactor * 100.f*dv, 1);
        }
    } else {
        fireStopping = fireTime > 0;
        if (fireStopping) fireTime -= dt * 100;
    }

    for (auto &a : asteroids) {
        if (length (a.position-position) <= shipSize+a.radius*1.1f) {
            a.stamina -= length(velocity) / a.radius;
            velocity =  position - a.position;
        }
    }

    position += velocity * dt;

    for (auto &p : fireParticles->getParticles ()) {
        if (p.lifeTime < 10) {
            for (auto &a : asteroids) {
                if (length(a.position-p.position) <= length(p.velocity*dt)+a.radius) {
                    a.stamina -= 0.2/a.radius;
                    p.lifeTime = 1000;
                }
            }
        }
    }
}

void Ship::draw(Engine &engine) {
    if (token != engine.token) init (engine);

    glUseProgram (program);

    modelMatrix = translate (mat4(1), position) * mat4_cast(orientation);

    mvMatrix = engine.viewMatrix * modelMatrix;
    mvpMatrix = engine.projectionMatrix * mvMatrix;

    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (mvpMatrix));
    glUniformMatrix4fv (u_MvMatrixHandle, 1, GL_FALSE, value_ptr (mvMatrix));

    mat4 lightMatrix = engine.viewMatrix * translate (mat4(1), position);
    glUniform3fv (u_LightPosHandle, 1, value_ptr (lightMatrix * engine.state.lightPos));
    glUniform3fv (u_EyePosHandle, 1, value_ptr (engine.state.eyePos));

    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_NormalHandle, 3, GL_FLOAT, GL_FALSE, stride, (void*)12);
    glEnableVertexAttribArray (a_NormalHandle);

    glVertexAttribPointer (a_ColorHandle, 3, GL_FLOAT, GL_FALSE, stride, (void*)24);
    glEnableVertexAttribArray (a_ColorHandle);

    glDrawArrays (GL_TRIANGLES, 0, nvertices / 3);

    glBindBuffer (GL_ARRAY_BUFFER, 0);

    throttleParticles->draw (engine);
    fireParticles->draw (engine);
}

void Ship::saveState (Engine &engine) {
    engine.state.shipPosition = position;
    engine.state.shipVelocity = velocity;
    engine.state.shipOrientation = orientation;
}

void Ship::restoreState (Engine &engine) {
    position = engine.state.shipPosition;
    velocity = engine.state.shipVelocity;
    orientation = engine.state.shipOrientation;
}

void Ship::reset () {
    position = vec3 (0);
    velocity = vec3 (0);
    orientation = angleAxis (0.f, vec3 (0, 0, 1));
}

vec3 Ship::getPosition() const
{
    return position;
}

void Ship::setPosition(const vec3 &value)
{
    position = value;
}

vec3 Ship::getVelocity() const
{
    return velocity;
}

void Ship::setVelocity(const vec3 &value)
{
    velocity = value;
}

quat Ship::getOrientation() const
{
    return orientation;
}

void Ship::setOrientation(const quat &value)
{
    orientation = value;
}

void Ship::setThrottle(bool value)
{
    throttle = value;
}

void Ship::setFire(bool value)
{
    fire = value;
}

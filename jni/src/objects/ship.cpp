#include "ship.h"

using namespace std;

Ship::Ship (Engine &engine) {
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
    stride = 3 * 3 * sizeof (GLfloat);

    throttleParticles = unique_ptr<Particles> (new Particles (vec3(1,0.5,0.1), 1024,
                                                      engine.width/16.0, 1/256.0));
    fireParticles = unique_ptr<Particles> (new Particles (vec3(1,0.1,0.01), 512,
                                                          engine.width/16.0, 1/1024.0));
    throttleTime = 0;
}

Ship::~Ship () {
    if (vbo) {
        glDeleteBuffers (1, &vbo);
        vbo = 0;
    }
}

void Ship::update (Engine &engine, vector<asteroid> &asteroids) {
    vec3 dv = engine.state.shipQuat * vec3 (0, 0, 1);
    vec3 up = engine.state.shipQuat * vec3 (0, 1, 0);
    vec3 rt = engine.state.shipQuat * vec3 (1, 0, 0);

    float dt = engine.delta * 0.001;

    float speedFactor = length (engine.state.shipVel) > 0 ?
                (3.0-dot (normalize (engine.state.shipVel), normalize (dv))) / 2.0 : 1.0;

    if (engine.state.throttle) {
        engine.state.shipVel += dv * dt * speedFactor * 10.f;
        throttleParticles->addParticles (engine.state.shipPos-0.4f*dv,
                                 engine.state.shipVel-2.0f*dv, 2);
        throttleParticles->addParticles (engine.state.shipPos-0.4f*dv-0.06f*up+0.22f*rt,
                                 engine.state.shipVel-2.0f*dv, 2);
        throttleParticles->addParticles (engine.state.shipPos-0.4f*dv-0.06f*up-0.22f*rt,
                                 engine.state.shipVel-2.0f*dv, 2);
        throttleTime += dt * 0.1f;
    } else {
        throttleTime *= 0.98;
    }
    throttleParticles->setParticlesColor (mix (vec3(1,0.5,0.1), vec3(0.1, 0.5, 1), throttleTime));

    if (engine.state.fire) {
        fireParticles->addParticles (engine.state.shipPos + 0.12f*up + 0.2f*rt,
                                     engine.state.shipVel + speedFactor * 100.f*dv, 1);
        fireParticles->addParticles (engine.state.shipPos + 0.12f*up - 0.2f*rt,
                                     engine.state.shipVel + speedFactor * 100.f*dv, 1);
    }

    engine.state.shipPos += engine.state.shipVel * dt;

    for (auto &p : fireParticles->getParticles ()) {
        if (p.lifeTime < 10) {
            for (auto &a : asteroids) {
                if (length(a.position-p.position) <= length(p.velocity*dt)+a.radius) {
                    a.stamina -= 0.2/a.radius;
                    if (a.stamina <= 0) a.blownUp = true;
                    p.lifeTime = 1000;
                }
            }
        }
    }
}

void Ship::draw(Engine &engine) {
    glUseProgram (program);

    modelMatrix = translate (mat4(1), engine.state.shipPos)
            * mat4_cast(engine.state.shipQuat);

    mvMatrix = engine.viewMatrix * modelMatrix;
    mvpMatrix = engine.projectionMatrix * mvMatrix;

    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (mvpMatrix));
    glUniformMatrix4fv (u_MvMatrixHandle, 1, GL_FALSE, value_ptr (mvMatrix));

    mat4 lightMatrix = engine.viewMatrix * translate (mat4(1), engine.state.shipPos);
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

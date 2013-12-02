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

    particles = unique_ptr<Particles> (new Particles (glm::vec3(1,0.5,0.1), 1024,
                                                      engine.width/12, 1/256.0));
}

Ship::~Ship () {
    if (vbo) {
        glDeleteBuffers (1, &vbo);
        vbo = 0;
    }
}

void Ship::update (Engine &engine) {
    glm::vec3 dv = engine.state.shipQuat * glm::vec3 (0, 0, 1);
    glm::vec3 up = engine.state.shipQuat * glm::vec3 (0, 1, 0);
    glm::vec3 rt = engine.state.shipQuat * glm::vec3 (1, 0, 0);

    if (engine.state.throttle) {
        engine.state.shipVel += dv * (float)(engine.delta / 200.0);
        particles->addParticles (engine.state.shipPos-0.4f*dv,
                                 engine.state.shipVel-2.0f*dv, 2);
        particles->addParticles (engine.state.shipPos-0.4f*dv-0.08f*up+0.22f*rt,
                                 engine.state.shipVel-2.0f*dv, 2);
        particles->addParticles (engine.state.shipPos-0.4f*dv-0.08f*up-0.22f*rt,
                                 engine.state.shipVel-2.0f*dv, 2);
    }

    engine.state.shipPos += engine.state.shipVel * (float)(engine.delta * 0.001);
}

void Ship::draw(Engine &engine) {
    glUseProgram (program);

    modelMatrix = glm::translate (glm::mat4(1), engine.state.shipPos)
            * glm::mat4_cast(engine.state.shipQuat);

    mvMatrix = engine.viewMatrix * modelMatrix;
    mvpMatrix = engine.projectionMatrix * mvMatrix;

    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, glm::value_ptr (mvpMatrix));
    glUniformMatrix4fv (u_MvMatrixHandle, 1, GL_FALSE, glm::value_ptr (mvMatrix));
    glUniform3fv (u_LightPosHandle, 1, glm::value_ptr (engine.viewMatrix * engine.state.lightPos));
    glUniform3fv (u_EyePosHandle, 1, glm::value_ptr (engine.state.eyePos));

    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_NormalHandle, 3, GL_FLOAT, GL_FALSE, stride, (void*)12);
    glEnableVertexAttribArray (a_NormalHandle);

    glVertexAttribPointer (a_ColorHandle, 3, GL_FLOAT, GL_FALSE, stride, (void*)24);
    glEnableVertexAttribArray (a_ColorHandle);

    glDrawArrays (GL_TRIANGLES, 0, nvertices / 3);

    glBindBuffer (GL_ARRAY_BUFFER, 0);

    particles->draw (engine);
}

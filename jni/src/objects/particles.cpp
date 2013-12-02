#include "particles.h"
#include "util/assets.h"
#include "util/shader.h"

using namespace std;

Particles::Particles(glm::vec3 color, GLuint maxParticles, GLfloat size, float decay)
    : maxCount (maxParticles), particlesColor (color), particleSize (size), decayFactor (decay)
{
    components = 3 + 1;
    stride = components * sizeof (GLfloat);
    particle dp = {glm::vec3 (0.1), glm::vec3 (0), 1000.0};
    for (int i = 0; i < maxCount; i++) {
        particles.push_back (dp);
    }
    vertexData.reserve (maxCount * components);
    cursor = maxCount;

    program = buildProgramFromAssets ("shaders/particle.vsh", "shaders/particle.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_ColorHandle = glGetUniformLocation (program, "u_Color");
    u_SizeHandle = glGetUniformLocation (program, "u_Size");
    u_TexUnitHandle = glGetUniformLocation (program, "u_TexUnit");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_LifeTimeHandle = glGetAttribLocation (program, "a_LifeTime");

    particleTex = loadTextureFromAsset ("textures/particle.png");
    glActiveTexture (GL_TEXTURE2);
    glBindTexture (GL_TEXTURE_2D, particleTex);
}

Particles::~Particles()
{
    if (particleTex) {
        glDeleteTextures (1, &particleTex);
        particleTex = 0;
    }
}

void Particles::draw(Engine &engine)
{
    for (auto iter = particles.begin (); iter != particles.end (); ++iter) {
        iter->position += iter->velocity * (float)(engine.delta * 0.001);
        iter->lifeTime += engine.delta * decayFactor;
    }
    for (int i = 0; i < maxCount; ++i) {
        vertexData[i*components] = particles[i].position[0];
        vertexData[i*components+1] = particles[i].position[1];
        vertexData[i*components+2] = particles[i].position[2];
        vertexData[i*components+3] = particles[i].lifeTime;
    }

    glUseProgram (program);
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, glm::value_ptr (engine.projectionMatrix * engine.viewMatrix));
    glUniform3fv (u_ColorHandle, 1, glm::value_ptr (particlesColor));
    glUniform1f (u_SizeHandle, particleSize);

//    glActiveTexture (GL_TEXTURE2);
//    glBindTexture (GL_TEXTURE_2D, particleTex);
    glUniform1i (u_TexUnitHandle, 2);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, stride, vertexData.data ());
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_LifeTimeHandle, 1, GL_FLOAT, GL_FALSE, stride, vertexData.data () + 3);
    glEnableVertexAttribArray (a_LifeTimeHandle);

    glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
    glDepthMask (false);
    glDrawArrays (GL_POINTS, 0, maxCount);
    glDepthMask (true);
    glDisable (GL_BLEND);
}

void Particles::addParticles(glm::vec3 position, glm::vec3 velocity, int count)
{
    for (int i = 0; i < count; ++i) {
        if (cursor++ >= maxCount) cursor = 0;

        particles[cursor].position = position + glm::ballRand (0.01f);
        particles[cursor].velocity = velocity + glm::ballRand (0.1f);
        particles[cursor].lifeTime = 0.0f;
    }
}

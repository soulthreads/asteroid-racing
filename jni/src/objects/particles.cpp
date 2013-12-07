#include "particles.h"

Particles::Particles(vec3 color, GLuint maxParticles, GLfloat size, float decay)
    : maxCount (maxParticles), particlesColor (color), particleSize (size), decayFactor (decay)
{
    components = 3 + 1;
    stride = components * sizeof (GLfloat);
    vertexData.reserve (maxCount * components);
    cursor = maxCount;

    reset ();
}

Particles::~Particles()
{
    if (particleTex) {
        glDeleteTextures (1, &particleTex);
        particleTex = 0;
    }
}

void Particles::reset () {
    particles.clear ();

    particle dp = {vec3 (1000), vec3 (0), 1000.0};
    for (int i = 0; i < maxCount; ++i) {
        particles.push_back (dp);
    }
}

void Particles::init () {
    token = engine.token;
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

void Particles::draw()
{
    if (token != engine.token) init ();

    for (auto &p : particles) {
        p.position += p.velocity * (float)(engine.delta * 0.001);
        p.lifeTime += engine.delta * decayFactor;
    }
    for (int i = 0; i < maxCount; ++i) {
        vertexData[i*components] = particles[i].position[0];
        vertexData[i*components+1] = particles[i].position[1];
        vertexData[i*components+2] = particles[i].position[2];
        vertexData[i*components+3] = particles[i].lifeTime;
    }

    glUseProgram (program);
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (engine.projectionMatrix * engine.viewMatrix));
    glUniform3fv (u_ColorHandle, 1, value_ptr (particlesColor));
    glUniform1f (u_SizeHandle, particleSize);

    glActiveTexture (GL_TEXTURE2);
    glBindTexture (GL_TEXTURE_2D, particleTex);
    glUniform1i (u_TexUnitHandle, 2);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, stride, vertexData.data ());
    glEnableVertexAttribArray (a_PositionHandle);

    glVertexAttribPointer (a_LifeTimeHandle, 1, GL_FLOAT, GL_FALSE, stride, vertexData.data () + 3);
    glEnableVertexAttribArray (a_LifeTimeHandle);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glDepthMask (false);
    glDrawArrays (GL_POINTS, 0, maxCount);
    glDepthMask (true);
    glDisable (GL_BLEND);
}

void Particles::addParticles(vec3 position, vec3 velocity, int count)
{
    for (int i = 0; i < count; ++i) {
        if (++cursor >= maxCount) cursor = 0;

        particles[cursor].position = position + ballRand (0.01f);
        particles[cursor].velocity = velocity + ballRand (0.1f);
        particles[cursor].lifeTime = 0.0f;
    }
}

void Particles::setParticlesColor(const vec3 &value)
{
    particlesColor = value;
}

vector<particle>& Particles::getParticles()
{
    return particles;
}

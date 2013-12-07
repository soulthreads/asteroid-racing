#include "sun.h"

Sun::Sun(vec3 sunPosition, float sunSize)
    : position (sunPosition), size (sunSize)
{
}

Sun::~Sun()
{
    if (sunTex) {
        glDeleteTextures (1, &sunTex);
        sunTex = 0;
    }
}

void Sun::init (Engine &engine) {
    token = engine.token;
    program = buildProgramFromAssets ("shaders/sun.vsh", "shaders/sun.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_SizeHandle = glGetUniformLocation (program, "u_Size");
    u_TexUnitHandle = glGetUniformLocation (program, "u_TexUnit");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");

    sunTex = loadTextureFromAsset ("textures/sun.png");
}

void Sun::draw(Engine &engine)
{
    if (token != engine.token) init (engine);

    glUseProgram (program);
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE,
                        value_ptr (engine.projectionMatrix * engine.viewMatrix));
    glUniform1f (u_SizeHandle, size);

    glActiveTexture (GL_TEXTURE3);
    glBindTexture (GL_TEXTURE_2D, sunTex);
    glUniform1i (u_TexUnitHandle, 3);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, 0, value_ptr (position));
    glEnableVertexAttribArray (a_PositionHandle);

    glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
    glDepthFunc (GL_LEQUAL);
    glDrawArrays (GL_POINTS, 0, 1);
    glDepthFunc (GL_LESS);
    glDisable (GL_BLEND);
}

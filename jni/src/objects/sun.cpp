#include "sun.h"
#include "util/assets.h"
#include "util/shader.h"

Sun::Sun(glm::vec3 sunPosition, float sunSize)
    : position (sunPosition), size (sunSize)
{
    program = buildProgramFromAssets ("shaders/sun.vsh", "shaders/sun.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_SizeHandle = glGetUniformLocation (program, "u_Size");
    u_TexUnitHandle = glGetUniformLocation (program, "u_TexUnit");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");

    sunTex = loadTextureFromAsset ("textures/sun.png");
}

Sun::~Sun()
{
    if (sunTex) {
        glDeleteTextures (1, &sunTex);
        sunTex = 0;
    }
}

void Sun::draw(Engine &engine)
{
    glUseProgram (program);
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE,
                        glm::value_ptr (engine.projectionMatrix * engine.viewMatrix));
    glUniform1f (u_SizeHandle, size);

    glActiveTexture (GL_TEXTURE3);
    glBindTexture (GL_TEXTURE_2D, sunTex);
    glUniform1i (u_TexUnitHandle, 3);

    glVertexAttribPointer (a_PositionHandle, 3, GL_FLOAT, GL_FALSE, 0, glm::value_ptr (position));
    glEnableVertexAttribArray (a_PositionHandle);

    glEnable (GL_BLEND);
    glBlendFunc (GL_ONE, GL_ONE);
    glDepthFunc (GL_LEQUAL);
    glDrawArrays (GL_POINTS, 0, 1);
    glDepthFunc (GL_LESS);
    glDisable (GL_BLEND);
}

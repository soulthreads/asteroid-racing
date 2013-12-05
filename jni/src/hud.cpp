#include "hud.h"

Hud::Hud(Engine &engine)
{
    stride = (2 + 2) * sizeof (GLfloat);

    GLfloat vs[] =  {
        // x, y, s, t
        -1,  1,  0, 0,
        -1, -1,  0, 1,
         1,  1,  1, 0,
        -1, -1,  0, 1,
         1, -1,  1, 1,
         1,  1,  1, 0
    };
    vertexData.assign (vs, vs+24);

    program = buildProgramFromAssets ("shaders/hud.vsh", "shaders/hud.fsh");
    validateProgram (program);
    u_MvpMatrixHandle = glGetUniformLocation (program, "u_MvpMatrix");
    u_TexUnitHandle = glGetUniformLocation (program, "u_TexUnit");

    a_PositionHandle = glGetAttribLocation (program, "a_Position");
    a_TexCoordsHandle = glGetAttribLocation (program, "a_TexCoords");

    controllerTex = loadTextureFromAsset ("textures/controller.png");
    controllerBgTex = loadTextureFromAsset ("textures/controller_bg.png");
    throttleTex = loadTextureFromAsset ("textures/throttle.png");
    fireTex = loadTextureFromAsset ("textures/fire.png");

    controllerCenter = vec3 (-engine.aspectRatio+controllerBgSize, -1+controllerBgSize, 0);
    throttleCenter = vec3 (engine.aspectRatio-throttleSize, -1+throttleSize, 0);
    fireCenter = vec3 (engine.aspectRatio-fireSize, -1+2.f*throttleSize+fireSize, 0);
}

Hud::~Hud()
{
    if (controllerTex) {
        glDeleteTextures (1, &controllerTex);
        glDeleteTextures (1, &controllerBgTex);
        glDeleteTextures (1, &throttleTex);

        controllerTex = 0;
        controllerBgTex = 0;
        throttleTex = 0;
    }
}

void Hud::handleTouch(Engine &engine, Ship &ship, float x, float y)
{
    float wx = x * engine.aspectRatio;
    if ((wx < controllerCenter[0]+controllerBgSize)
            && (y < controllerCenter[1] + controllerBgSize)) {
        rotating = true;
        float dx = wx - controllerCenter[0];
        float dy = y - controllerCenter[1];

        vec3 up = ship.getOrientation () * vec3 (0, 1, 0);
        quat xaxis = angleAxis (-5*dx, up);
        quat yaxis = angleAxis (5*dy, vec3(-1, 0, 0));
        ship.setOrientation (normalize (xaxis * ship.getOrientation () * yaxis));

        controllerOffset[0] = dx;
        controllerOffset[1] = dy;
        engine.state.camRot = clamp (engine.state.camRot + 0.05f*vec2(dx, dy),
                                     vec2(-0.5,-0.5), vec2(0.5, 0.5));
    } else if ((wx > throttleCenter[0] - throttleSize)
               && (y < throttleCenter[1] + throttleSize)) {
        ship.setThrottle (true);
    } else if ((wx > fireCenter[0] - fireSize)
               && (y < fireCenter[1] + fireSize)) {
        ship.setFire (true);
    }
}

void Hud::draw(Engine &engine)
{
    glUseProgram (program);
    glVertexAttribPointer (a_PositionHandle, 2, GL_FLOAT, GL_FALSE, stride, &vertexData[0]);
    glEnableVertexAttribArray (a_PositionHandle);
    glVertexAttribPointer (a_TexCoordsHandle, 2, GL_FLOAT, GL_FALSE, stride, &vertexData[2]);
    glEnableVertexAttribArray (a_TexCoordsHandle);

    glActiveTexture (GL_TEXTURE1);
    glUniform1i (u_TexUnitHandle, 1);

    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    modelMatrix = translate (mat4 (1), controllerCenter);
    modelMatrix = scale (modelMatrix, vec3(controllerBgSize));
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr(engine.orthoMatrix * modelMatrix));
    glBindTexture (GL_TEXTURE_2D, controllerBgTex);
    glDrawArrays (GL_TRIANGLES, 0, 6);

    modelMatrix = translate (mat4 (1), controllerCenter+controllerOffset);
    modelMatrix = scale (modelMatrix, vec3(controllerSize));
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr(engine.orthoMatrix * modelMatrix));
    glBindTexture (GL_TEXTURE_2D, controllerTex);
    glDrawArrays (GL_TRIANGLES, 0, 6);
    controllerOffset *= 0.8f;

    modelMatrix = translate (mat4 (1), throttleCenter);
    modelMatrix = scale (modelMatrix, vec3 (throttleSize));
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (engine.orthoMatrix * modelMatrix));
    glBindTexture (GL_TEXTURE_2D, throttleTex);
    glDrawArrays (GL_TRIANGLES, 0, 6);

    modelMatrix = translate (mat4 (1), fireCenter);
    modelMatrix = scale (modelMatrix, vec3 (fireSize));
    glUniformMatrix4fv (u_MvpMatrixHandle, 1, GL_FALSE, value_ptr (engine.orthoMatrix * modelMatrix));
    glBindTexture (GL_TEXTURE_2D, fireTex);
    glDrawArrays (GL_TRIANGLES, 0, 6);

    glDisable (GL_BLEND);
    glEnable (GL_DEPTH_TEST);
}

bool Hud::getRotating() const
{
    return rotating;
}

void Hud::setRotating(bool value)
{
    rotating = value;
}

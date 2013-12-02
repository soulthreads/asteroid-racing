#ifndef ENGINE_H
#define ENGINE_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>

#include <vector>
#include <string>
#include <memory>

struct saved_state {
    glm::vec3 eyePos;
    glm::vec2 camRot;
    glm::vec4 lightPos;
    glm::vec3 shipPos;
    glm::vec3 shipVel;
    glm::quat shipQuat;
    bool throttle;
};

typedef struct {
    struct android_app* app;

    bool animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    float aspectRatio;
    glm::mat4 projectionMatrix;
    glm::mat4 orthoMatrix;
    glm::mat4 viewMatrix;
    double delta;
    struct saved_state state;
} Engine;

#endif // ENGINE_H

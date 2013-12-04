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
#include <glm/gtc/noise.hpp>

#include <vector>
#include <map>
#include <string>
#include <memory>

using std::vector;
using std::map;
using std::string;
using std::unique_ptr;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::uvec3;
using glm::normalize;
using glm::cross;
using glm::dot;
using glm::mix;

using glm::linearRand;
using glm::ballRand;
using glm::sphericalRand;

using glm::mat4;
using glm::value_ptr;
using glm::translate;
using glm::scale;
using glm::rotate;
using glm::perspective;
using glm::ortho;
using glm::lookAt;

using glm::quat;
using glm::angleAxis;
using glm::mat4_cast;
using glm::simplex;

#include "util/assets.h"
#include "util/shader.h"

struct saved_state {
    vec3 eyePos;
    vec2 camRot;
    vec4 lightPos;
    vec3 shipPos;
    vec3 shipVel;
    quat shipQuat;
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
    mat4 projectionMatrix;
    mat4 orthoMatrix;
    mat4 viewMatrix;
    double delta;
    struct saved_state state;
} Engine;

#endif // ENGINE_H

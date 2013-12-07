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
#include <glm/gtx/intersect.hpp>

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::function;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::uvec3;
using glm::length;
using glm::normalize;
using glm::cross;
using glm::dot;
using glm::mix;

using glm::clamp;
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

enum GameState {
    GAME_START_MENU,
    GAME_PLAYING,
    GAME_WIN_MENU,
    GAME_OVER_MENU,
    GAME_PAUSE_MENU,
    GAME_SELECT_MENU,
    GAME_STATS_MENU
};

struct saved_state {
    vec3 shipPosition;
    vec3 shipVelocity;
    quat shipOrientation;
    vec3 eyePos;
    vec2 camRot;
    vec4 lightPos;
};

struct Engine {
    struct android_app* app;

    int token;
    bool animating;
    bool exitFlag;
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
    GameState gameState;
    struct saved_state state;
};

extern Engine engine;

#endif // ENGINE_H

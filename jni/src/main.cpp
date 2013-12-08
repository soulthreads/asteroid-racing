#include <jni.h>
#include <cerrno>
#include <cstring>
#include <ctime>

#include "engine.h"

#include <android_native_app_glue.h>
#include <android/sensor.h>
#include <android/asset_manager.h>

#define LOG_TAG "AsteroidRacing"
#include "util/logs.h"

#include "menu.h"
#include "hud.h"
#include "objects/skybox.h"

#include "game.h"

Engine engine;

unique_ptr<Menu> menu;
unique_ptr<Hud> hud;
unique_ptr<Skybox> skybox;
unique_ptr<Particles> envp;
unique_ptr<Text> text;

unique_ptr<Asteroids> ast;
unique_ptr<Ship> ship;

inline static double now_ms() {
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}

/**
 * Initialize an EGL context for the current display.
 */
static int engineInitDisplay (Engine &engine) {
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 1,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
    LOGI ("Number of EGL configs: %d\n", numConfigs);
    EGLConfig confs[numConfigs];
    eglChooseConfig (display, attribs, confs, numConfigs, &numConfigs);
    config = confs[0];

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine.app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine.app->window, NULL);

    const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context = eglCreateContext(display, config, NULL, context_attribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine.display = display;
    engine.context = context;
    engine.surface = surface;
    engine.width = w;
    engine.height = h;
    engine.aspectRatio = w / (float) h;
    engine.animating = true;
    engine.delta = 0;
    LOGI ("display init!");

    // Initialize GL state.
    glClearColor (0.2f, 0.2f, 0.2f, 0.0f);
    glEnable (GL_CULL_FACE);
    glEnable (GL_DEPTH_TEST);

    engine.projectionMatrix = perspective (60.0f, ((float)w)/h, 0.1f, 2000.0f);
    engine.orthoMatrix = ortho (-engine.aspectRatio, +engine.aspectRatio,
                                      -1.0f, 1.0f, -1.0f, 1.0f);

    if (menu == nullptr) {
        menu = unique_ptr<Menu> (new Menu);
        hud = unique_ptr<Hud> (new Hud);
        text = unique_ptr<Text> (new Text);
        text->addText ("alpha", textUnit {vec2(0, -1),
                                          vec4(1, 1, 0, 0.1),
                                          1, A_CENTER, A_MINUS, "alpha version"});

        skybox = unique_ptr<Skybox>(new Skybox);
        envp = unique_ptr<Particles> (new Particles (vec3 (1,1,0.5), 256, w/20, 1/512.0));
        ast = unique_ptr<Asteroids> (new Asteroids);
        ship = unique_ptr<Ship> (new Ship);
    }
    return 0;
}

static void drawFrame () {
    if (engine.display == NULL) {
        // No display.
        return;
    }

    static float timeCounter = 0;
    static int frameCounter = 0;
    double start = now_ms ();

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    static float winTimer = 0;
    if ((engine.gameState == GAME_PLAYING) && (ast->getAsteroids ().size () == 0)) {
        winTimer += engine.delta;
        if (winTimer >= 5000) {
            engine.gameState = GAME_WIN_MENU;
            winTimer = 0;
        }
    }

    switch (engine.gameState) {
    case GAME_PLAYING:
    {
        auto shipOrientation = ship->getOrientation ();
        vec3 dv = shipOrientation * vec3 (0, 0, 1);
        vec3 up = shipOrientation * vec3 (0, 1, 0);
        vec3 rt = shipOrientation * vec3 (1, 0, 0);

        vec3 offset = engine.state.camRot[0] * rt - (0.5f + engine.state.camRot[1]) * up + 2.0f * dv;
        if (!hud->getRotating ()) engine.state.camRot *= 0.95f;

        engine.viewMatrix = lookAt (vec3 (0), offset, up);
        skybox->draw ();

        ship->update (ast->getAsteroids ());

        auto shipPos = ship->getPosition ();
        auto shipVel = ship->getVelocity ();

        char buffer[256];
        sprintf (buffer, "|v|=%.1fu/s", length (shipVel));
        text->addText ("speed", textUnit {vec2 (engine.aspectRatio, 1),
                                          mix (vec4 (0, 0.5, 1, 1), vec4 (1, 0.5, 0, 1), length (shipVel) / 100.f),
                                          0.8, A_PLUS, A_PLUS, buffer});

        if (timeCounter > 1000) {
            sprintf (buffer, "FPS: %d", frameCounter);
            text->addText ("fps", textUnit {vec2 (-engine.aspectRatio, 1),
                                            vec4 (0, 1, 1, 0.1),
                                            0.8, A_MINUS, A_PLUS, buffer});
            frameCounter = 0;
            timeCounter = 0;

            sprintf (buffer, "p: (%.1f, %.1f, %.1f) v: (%.1f, %.1f, %.1f)",
                     shipPos[0], shipPos[1], shipPos[2],
                    shipVel[0], shipVel[1], shipVel[2]);
            text->addText ("pos", textUnit {vec2 (0, 1),
                                            vec4 (1, 1, 1, 0.5),
                                            0.5, A_CENTER, A_PLUS, buffer});
        }

        sprintf (buffer, "asteroids: %d", ast->getAsteroids ().size ());
        text->addText ("ast", textUnit {vec2 (engine.aspectRatio, 1-0.1),
                                        vec4 (1, 1, 1, 0.5),
                                        0.8, A_PLUS, A_PLUS, buffer});

        engine.state.eyePos = shipPos - offset;
        vec3 center = shipPos + 3.f*dv + engine.state.camRot[0]*rt - engine.state.camRot[1] * up;
        engine.viewMatrix = lookAt (engine.state.eyePos, center, up);

        ast->draw ();

        ship->draw ();

        envp->addParticles (shipPos+ballRand (100.0f), vec3 (0), 1);
        envp->draw ();

        hud->draw ();
        text->draw ();

        timeCounter += engine.delta;
        frameCounter++;
        break;
    }
    default:
        menu->draw ();
        break;
    }
    eglSwapBuffers(engine.display, engine.surface);

    double end = now_ms ();
    engine.delta = end - start;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engineTermDisplay (Engine &engine) {
    if (engine.display != EGL_NO_DISPLAY) {
        eglMakeCurrent (engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine.context != EGL_NO_CONTEXT) {
            eglDestroyContext (engine.display, engine.context);
        }
        if (engine.surface != EGL_NO_SURFACE) {
            eglDestroySurface (engine.display, engine.surface);
        }
        eglTerminate (engine.display);
    }
    engine.animating = false;
    engine.display = EGL_NO_DISPLAY;
    engine.context = EGL_NO_CONTEXT;
    engine.surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engineHandleInput (struct android_app* app, AInputEvent* event) {
    Engine &engine = *(Engine *)app->userData;
    auto eventType = AInputEvent_getType (event);
    if (eventType == AINPUT_EVENT_TYPE_MOTION) {
        auto count = AMotionEvent_getPointerCount (event);
        auto action = AMotionEvent_getAction (event);
        auto actionMasked = action & AMOTION_EVENT_ACTION_MASK;
        auto actionIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

        if (action != AMOTION_EVENT_ACTION_CANCEL) {
            for (size_t i = 0; i < count; ++i) {
                float x = AMotionEvent_getX(event, i) * 2.0 / engine.width - 1.0;
                float y = -(AMotionEvent_getY(event, i) * 2.0 / engine.height - 1.0);
                if (engine.gameState == GAME_PLAYING) {
                    if (actionIndex == i && (actionMasked == AMOTION_EVENT_ACTION_UP
                                             || actionMasked == AMOTION_EVENT_ACTION_POINTER_UP)) {
                        ship->setThrottle (false);
                        ship->setFire (false);
                        hud->setRotating (false);
                    } else {
                        hud->handleTouch (x, y);
                    }
                } else {
                    menu->handleTouch (actionMasked, x, y);
                }
            }
        }

        return 1;
    } else if (eventType == AINPUT_EVENT_TYPE_KEY) {
        auto eventAction = AKeyEvent_getAction (event);
        if (eventAction == AKEY_EVENT_ACTION_DOWN) {
            auto eventKeyCode = AKeyEvent_getKeyCode (event);
            switch (engine.gameState) {
            case GAME_PLAYING:
                if ((eventKeyCode == AKEYCODE_BACK) || (eventKeyCode == AKEYCODE_MENU)) {
                    engine.gameState = GAME_PAUSE_MENU;
                    return 1;
                }
                break;
            case GAME_PAUSE_MENU:
                if (eventKeyCode == AKEYCODE_MENU) {
                    engine.gameState = GAME_PLAYING;
                    return 1;
                }
            case GAME_STATS_MENU:
                if (eventKeyCode == AKEYCODE_BACK) {
                    engine.gameState = GAME_START_MENU;
                    return 1;
                }
                break;
            default:
                break;
            }
        }
    }

    return 0;
}

/**
 * Process the next main command.
 */
static void engineHandleCmd (struct android_app* app, int32_t cmd) {
    Engine &engine = *(Engine *)app->userData;
    switch (cmd) {
    case APP_CMD_SAVE_STATE:
        LOGI ("Save state!");
        ship->saveState ();
        engine.app->savedState = malloc(sizeof(struct saved_state));
        *((struct saved_state*)engine.app->savedState) = engine.state;
        engine.app->savedStateSize = sizeof(struct saved_state);
        break;
    case APP_CMD_INIT_WINDOW:
        if (engine.app->window != NULL) {
            engineInitDisplay(engine);
        }
        break;
    case APP_CMD_TERM_WINDOW:
        engineTermDisplay(engine);
        break;
    case APP_CMD_GAINED_FOCUS:
        engine.token = rand ();
        break;
    case APP_CMD_LOST_FOCUS:
        engine.animating = false;
        if (engine.gameState == GAME_PLAYING) engine.gameState = GAME_PAUSE_MENU;
//        engineDrawFrame(engine);
        break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    LOGI ("entering android_main!");
    srand (time(NULL));

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engineHandleCmd;
    state->onInputEvent = engineHandleInput;
    engine.app = state;

    initAssetManager (state->activity->assetManager);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        LOGI ("Restore state!");
        engine.state = *(struct saved_state*)state->savedState;
    } else {
        engine.state.lightPos = vec4 (-1000, 0, 0, 1);
    }

    while (1) {
        int ident;
        int fdesc;
        int events;
        struct android_poll_source* source;

        while ((ident = ALooper_pollAll(0, &fdesc, &events, (void**)&source)) >= 0) {
            if (source) {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engineTermDisplay(engine);
                return;
            }
        }

        if (engine.exitFlag) {
            engine.animating = false;
            ANativeActivity_finish (state->activity);
            exit (0);
        }
        if (engine.animating) {
            drawFrame();
        }
    }
}

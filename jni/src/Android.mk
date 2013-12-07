LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-activity

PNG_PATH := ../libpng
GLM_PATH := ../glm

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(PNG_PATH) \
	$(LOCAL_PATH)/$(GLM_PATH)

LOCAL_SRC_FILES := main.cpp \
        hud.cpp \
        menu.cpp \
        util/shader.cpp \
        util/assets.cpp \
        util/text.cpp \
        objects/skybox.cpp \
        objects/sun.cpp \
        objects/ship.cpp \
        objects/particles.cpp \
        objects/asteroids.cpp \
        gui/element.cpp \
        gui/button.cpp \
        gui/layout.cpp

LOCAL_SHARED_LIBRARIES := png
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2

LOCAL_CPPFLAGS += -O2 -std=c++11

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)

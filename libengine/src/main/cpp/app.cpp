#include "app.hpp"

#include <AL/alc.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <cstdio>

#include <functional>
#include <string>

namespace {
    static void _onError(const std::string& msg) {
#if __EMSCRIPTEN__
        printf("ERR: %s\n", msg.c_str());
        __builtin_trap();
#else
        throw msg;
#endif
    }

    static void _init() {
        static bool needsInit = true;

        if (needsInit) {
            if (glfwInit() != GL_TRUE) {
                _onError("glfwInit failed!");
            }

            needsInit = false;
        }
    }

#ifdef __EMSCRIPTEN__
    application * _pApp;

    void _doFrameStatic() {
        _pApp->doFrame();
        
    }
#endif
}

application::application(int width, int height, const std::string& title) {
    _init();
    
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if ((_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)) == nullptr) {
        _onError("glfwCreateWindow failed!");
    }

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if ((_alcDevice = alcOpenDevice(nullptr)) == nullptr) {
        _onError("alcOpenDevice failed!");
    }

    if ((_alcContext = alcCreateContext(_alcDevice, nullptr)) == nullptr) {
        _onError("alcCreateContext failed!");
    }

    if (!alcMakeContextCurrent(_alcContext)) {
        _onError("alcMakeContextCurrent failed!");
    }
}

application::~application() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_alcContext);
    alcCloseDevice(_alcDevice);

    glfwDestroyWindow(_window);
    glfwTerminate();
}

void application::doFrame() const {
    _onFrame(userData.get());
    glfwSwapBuffers(_window);    
    glfwPollEvents();
}

void application::setOnFrame(const std::function<void(void *)>& fn) {
    _onFrame = fn;
}

void application::start(const std::function<void(void *)>& fn) {
    setOnFrame(fn);

#ifdef __EMSCRIPTEN__
    _pApp = this;
    emscripten_set_main_loop(_doFrameStatic, 60, 1);
#else
    while (!glfwWindowShouldClose(_window)) {
        doFrame();
    }
#endif
}
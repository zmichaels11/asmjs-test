#include "engine/application.hpp"

#include <AL/alc.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace {
    void _onError(const std::string& msg) {
        std::cerr << msg << std::endl;
        __builtin_trap();
    }

    void doFrame();

    struct native_resources {
        struct glfw_resources_T {
            GLFWwindow * pWindow;
        } glfw;

        struct openal_resources_T {
            ALCdevice * pDevice;
            ALCcontext * pContext;
        } oal;

        native_resources() {}

        native_resources(const std::string& title, unsigned int width, unsigned int height);        

        ~native_resources();

        void update();

        bool isValid() const;
    };    

    native_resources::native_resources(const std::string& title, unsigned int width, unsigned int height) {
        if (!glfwInit()) {
            _onError("glfwInit failed!");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        if ((glfw.pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)) == nullptr) {
            _onError("glfwCreateWindow failed!");
        }

        glfwMakeContextCurrent(glfw.pWindow);
        glfwSwapInterval(1);

        if ((oal.pDevice = alcOpenDevice(nullptr)) == nullptr) {
            _onError("alcOpenDevice failed!");
        }

        if ((oal.pContext = alcCreateContext(oal.pDevice, nullptr)) == nullptr) {
            _onError("alcCreateContext failed!");
        }

        if (!alcMakeContextCurrent(oal.pContext)) {
            _onError("alcMakeContextCurrent failed!");
        }
    }

    native_resources::~native_resources() {
        if (oal.pContext && oal.pDevice) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(oal.pContext);
            alcCloseDevice(oal.pDevice);            
        }

        if (glfw.pWindow) {
            glfwDestroyWindow(glfw.pWindow);
            glfwTerminate();
        }
    }

    bool native_resources::isValid() const {
        return glfw.pWindow && !glfwWindowShouldClose(glfw.pWindow);
    }

    void native_resources::update() {
        glfwSwapBuffers(glfw.pWindow);
        glfwPollEvents();
    }    
}

namespace {
    std::unique_ptr<native_resources> _pNativeResources;
    std::function<void(void*)> _onFrame;
    std::shared_ptr<void> _pUserData;
    double _time;    

    void doFrame() {        
        _onFrame(_pUserData.get());
        _pNativeResources->update();
        _time = glfwGetTime();
    }
}

namespace engine {
    namespace application {
        void setOnFrame(const std::function<void(void*)>& callback) {
            _onFrame = callback;
        }

        void init(const std::string& name, unsigned int width, unsigned int height) {
            _pNativeResources = std::make_unique<native_resources> (name, width, height);
        }

        double getTime() {
            return _time;
        }

        void start(const std::shared_ptr<void>& pUserData) {
            _pUserData = pUserData;

#ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(doFrame, 60, 1);
#else
            while (_pNativeResources->isValid()) {
                doFrame();
            }
#endif
        }
    }
}
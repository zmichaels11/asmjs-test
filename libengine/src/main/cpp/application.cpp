#include "engine/application.hpp"

#include <cstddef>
#include <cstring>
#include <cstdio>

#include <AL/alc.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "nuklear/nk_ctx.hpp"

namespace {    
    void _onError(const std::string& msg) {
        std::cerr << msg << std::endl;
        __builtin_trap();
    }    

    void doFrame();

    struct native_resources {
        struct glfw_resources_t {
            GLFWwindow * pWindow;
        } glfw;

        struct openal_resources_t {
            ALCdevice * pDevice;
            ALCcontext * pContext;
        } oal;

        std::unique_ptr<nk::nk_ctx> nuklear;     

        native_resources() {}

        native_resources(const std::string& title, unsigned int width, unsigned int height);        

        ~native_resources();    

        bool isValid() const;
    };
}

namespace {
    std::unique_ptr<native_resources> _pNativeResources;
    std::function<void(void*)> _onFrame(nullptr);
    std::function<void(void*)> _onUpdate(nullptr);
    std::shared_ptr<void> _pUserData;

    double _time;            
}

namespace nk {
    nk_ctx * getContext() {
        return _pNativeResources->nuklear.get();
    }
}

namespace engine {
    namespace application {
        void setOnUpdate(const std::function<void(void*)>& callback) {
            _onUpdate = callback;
        }

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

namespace {
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

        nuklear = std::make_unique<nk::nk_ctx> (glfw.pWindow);
    }

    native_resources::~native_resources() {
        if (nuklear) {
            nuklear = nullptr;
        }

        if (glfw.pWindow) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(oal.pContext);
            alcCloseDevice(oal.pDevice);            

            glfwDestroyWindow(glfw.pWindow);
            glfwTerminate();
        }
    }

    bool native_resources::isValid() const {
        return glfw.pWindow && !glfwWindowShouldClose(glfw.pWindow);
    }

    void doFrame() {        
        glfwPollEvents();
        _time = glfwGetTime();

        auto& nk = _pNativeResources->nuklear;

        if (_onUpdate) {
            nk->newFrame();
            _onUpdate(_pUserData.get());        
        }

        if (_onFrame) {
            _onFrame(_pUserData.get());                        
            nk->render();
        }        
        
        glfwSwapBuffers(_pNativeResources->glfw.pWindow);    
    }
}
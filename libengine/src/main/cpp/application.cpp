#include "engine/application.hpp"

#include <cstddef>
#include <cstring>
#include <cstdio>

#include <AL/alc.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifdef GLES20
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#elif GLES30
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#elif GL
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#error No GL version specified!
#endif

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "graphics.hpp"

#include "renderer/scene.hpp"
#include "nk/nk_ctx.hpp"

namespace {    
    void _onError(const std::string& msg) noexcept {
        std::cerr << msg << std::endl;
        __builtin_trap();
    }    

    void doFrame() noexcept;

    struct native_resources {
        struct glfw_resources_t {
            GLFWwindow * pWindow;
        } glfw;

        struct openal_resources_t {
            ALCdevice * pDevice;
            ALCcontext * pContext;
        } oal;
        
        std::unique_ptr<nk::nk_ctx> nuklear;     

        native_resources() noexcept {}

        native_resources(const std::string& title, unsigned int width, unsigned int height) noexcept;

        ~native_resources() noexcept;

        bool isValid() const noexcept;
    };
}

namespace {
    std::shared_ptr<renderer::scene> _scene(nullptr);
    std::unique_ptr<native_resources> _pNativeResources(nullptr);
    std::function<void(void*)> _onFrame(nullptr);
    std::function<void(void*)> _onUpdate(nullptr);
    std::shared_ptr<void> _pUserData;

    double _time(0.0);            
}

namespace nk {
    nk_ctx * getContext() {
        return _pNativeResources->nuklear.get();
    }
}

namespace engine {
    renderer::scene * application::getScene() noexcept {
        return _scene.get();
    }
    
    void application::setScene(const std::shared_ptr<renderer::scene>& scene) noexcept {
        _scene = scene;
    }

    void application::setScene(const renderer::scene_info& sceneInfo) noexcept {
        _scene = std::make_shared<renderer::scene> (sceneInfo);
    }

    void application::setOnUpdate(const std::function<void(void*)>& callback) noexcept {
        _onUpdate = callback;
    }

    void application::setOnFrame(const std::function<void(void*)>& callback) noexcept {
        _onFrame = callback;
    }

    void application::init(const std::string& name, unsigned int width, unsigned int height) noexcept {
        _pNativeResources = std::make_unique<native_resources> (name, width, height);            
    }

    double application::getTime() noexcept {
        return _time;
    }

    void application::start(const std::shared_ptr<void>& pUserData) noexcept {
        _pUserData = pUserData;

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(doFrame, 0, 1);
#else
        while (_pNativeResources->isValid()) {
            doFrame();
        }
#endif
    }            
}

namespace {
    native_resources::native_resources(const std::string& title, unsigned int width, unsigned int height) noexcept {
        if (!glfwInit()) {
            _onError("glfwInit failed!");
        }

        glfwDefaultWindowHints();

#ifdef GLES20
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#elif GLES30
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#elif GL        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
#error "No GL version specified!"
#endif


        if ((glfw.pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)) == nullptr) {
            _onError("glfwCreateWindow failed!");
        }

        glfwMakeContextCurrent(glfw.pWindow);

        graphics::init();

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

    native_resources::~native_resources() noexcept {
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

    bool native_resources::isValid() const noexcept {
        return glfw.pWindow && !glfwWindowShouldClose(glfw.pWindow);
    }

    void doFrame() noexcept {        
        glfwPollEvents();
        _time = glfwGetTime();

        auto& nk = _pNativeResources->nuklear;        

        if (_onUpdate) {            
            _onUpdate(_pUserData.get());        
        }

        if (_scene) {
            _scene->update();
        }

        if (_onFrame) {
            _onFrame(_pUserData.get());            
        }        

        if (_scene) {
            _scene->render();
        }
        
        glfwSwapBuffers(_pNativeResources->glfw.pWindow);    
    }
}
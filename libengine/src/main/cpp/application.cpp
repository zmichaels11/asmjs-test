#include "engine/application.hpp"

#include <cstddef>
#include <cstring>
#include <cstdio>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#if defined(GLES20)
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#elif defined(GLES30)
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#elif defined(GL)
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#error No GL version specified!
#endif

#include <cstdio>

#include <functional>
#include <memory>
#include <string>

#include "audio.hpp"
#include "graphics.hpp"

#include "renderer/scene.hpp"
#include "nk/nk_ctx.hpp"

namespace {    
    void _onGLFWError(int error, const char * desc);

    void doFrame() noexcept;

    struct native_resources {
        struct glfw_resources_t {
            GLFWwindow * pWindow;
        } glfw;
        
        std::unique_ptr<nk::nk_ctx> nuklear;     

        native_resources() noexcept {}

        native_resources(const std::string& title, unsigned int width, unsigned int height) noexcept;

        ~native_resources() noexcept;

        bool isValid() const noexcept;
    };
}

namespace {
    std::unique_ptr<renderer::scene> _scene(nullptr);
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

    void application::setScene(const renderer::scene_info& info) noexcept {
        _scene.reset(new renderer::scene(info));
    }

    std::unique_ptr<renderer::scene> application::releaseScene() noexcept {
        return std::unique_ptr<renderer::scene>(_scene.release());
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

#if defined(__EMSCRIPTEN__)
        emscripten_set_main_loop(doFrame, 0, 1);
#else
        while (_pNativeResources->isValid()) {
            doFrame();
        }
#endif
    }            
}

namespace {
    void _onGLFWError(int error, const char * description) {
        switch (error) {
            case GLFW_NOT_INITIALIZED:
                std::printf("[GLFW] Not initialized: %s\n", description);                
                break;
            case GLFW_NO_CURRENT_CONTEXT:
                std::printf("[GLFW] No current context: %s\n", description);
                break;
            case GLFW_INVALID_ENUM:
                std::printf("[GLFW] Invalid enum: %s\n", description);
                break;
            case GLFW_INVALID_VALUE:
                std::printf("[GLFW] Invalid value: %s\n", description);
                break;
            case GLFW_OUT_OF_MEMORY:
                std::printf("[GLFW] Out of memory: %s\n", description);
                break;
            case GLFW_API_UNAVAILABLE:
                std::printf("[GLFW] API unavailable: %s\n", description);
                break;
            case GLFW_VERSION_UNAVAILABLE:
                std::printf("[GLFW] Version unavailable: %s\n", description);
                break;
            case GLFW_PLATFORM_ERROR:
                std::printf("[GLFW] Platform error: %s\n", description);
                break;
            case GLFW_FORMAT_UNAVAILABLE:
                std::printf("[GLFW] Format unavailable: %s\n", description);
                break;
            case GLFW_NO_WINDOW_CONTEXT:
                std::printf("[GLFW] No window context: %s\n", description);
                break;
            default:
                std::printf("[GLFW] Unknown error: %s\n", description);
                break;
        }
    }

    native_resources::native_resources(const std::string& title, unsigned int width, unsigned int height) noexcept {
        glfwSetErrorCallback(_onGLFWError);

        if (glfwInit() != GLFW_TRUE) {
            std::printf("[GLFW] Unable to initialize!\n");
            __builtin_trap();
        }

        {
            int major = 0;
            int minor = 0;
            int revision = 0;

            glfwGetVersion(&major, &minor, &revision);

            std::printf("[GLFW] GLFW Version: %d.%d.%d\n", major, minor, revision);
        }

        glfwDefaultWindowHints();

#if defined(GLES20)
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#elif defined(GLES30)
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#elif defined(GL)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        std::printf("[GLFW] Configured for OpenGL!\n");
#else
#error "No GL version specified!"
#endif


        if ((glfw.pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)) == nullptr) {
            std::printf("[GLFW] Window creation failed!\n");
            __builtin_trap();
        }

        glfwMakeContextCurrent(glfw.pWindow);

        graphics::init();

        glfwSwapInterval(1);

        audio::init();

        nuklear = std::make_unique<nk::nk_ctx> (glfw.pWindow);        
    }

    native_resources::~native_resources() noexcept {
        if (nuklear) {
            nuklear = nullptr;
        }

        if (glfw.pWindow) {
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
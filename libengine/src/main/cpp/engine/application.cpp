#include "pch.h"
#include "engine/application.hpp"

#include "engine/application_hint.hpp"
#include "engine/application_info.hpp"
#include "engine/layers/scene.hpp"

namespace {    
    void _onError(const std::string& src, const std::string& err, const std::string& msg) noexcept;

    void _onGLFWError(int error, const char * desc) noexcept;

    void _doFrame() noexcept;

    void _updateFramebufferSize(GLFWwindow * pWindow, int width, int height) noexcept;

    void _handleKeyEvent(GLFWwindow * pWindow, unsigned int codepoint) noexcept;

    void _handleScrollEvent(GLFWwindow * pWindow, double sx, double sy) noexcept;

    void _handleMouseButtonEvent(GLFWwindow * pWindow, int button, int action, int mods) noexcept;

    struct native_resources {        
        GLFWwindow * pWindow;        

        native_resources() noexcept:
            pWindow(nullptr) {}

        native_resources(const engine::application_info& info) noexcept;

        native_resources(const native_resources&) = delete;

        native_resources& operator=(const native_resources&) = delete;

        inline native_resources(native_resources&& other) noexcept {
            pWindow = std::move(other.pWindow);
            other.pWindow = nullptr;
        }

        inline native_resources& operator=(native_resources&& other) {
            std::swap(pWindow, other.pWindow);

            return *this;
        }

        ~native_resources();

        bool isValid() const noexcept;
    };
}

namespace {
    engine::layers::scene _scene;
    native_resources _nativeResources;
    std::function<void(void*)> _onFrame(nullptr);
    std::function<void(void*)> _onUpdate(nullptr);
    std::function<void(void*)> _onInit(nullptr);
    std::shared_ptr<void> _pUserData;
    std::vector<std::function<bool(unsigned int)>> _charCallbacks;
    std::vector<std::function<bool(double, double)>> _scrollCallbacks;
    std::vector<std::function<bool(int, int, int)>> _mouseButtonCallbacks;

    graphics::scissor_state_info _scissorRect;
    graphics::viewport_state_info _viewport;

    double _time(0.0);  
}

namespace engine {
    engine::layers::scene * application::getScene() noexcept {
        return &_scene;
    }

    void application::step() noexcept {
        _doFrame();
    }

    void application::setScene(const engine::layers::scene_info& info) noexcept {        
        _scene = engine::layers::scene(info);

        if (_onInit) {
            _onInit(_pUserData.get());
        }
    }

    void application::releaseScene() noexcept {
        _charCallbacks.clear();
        _scrollCallbacks.clear();
        _mouseButtonCallbacks.clear();
        
        _scene = engine::layers::scene();
    }

    void application::setOnInit(const std::function<void(void*)>& callback) noexcept {
        _onInit = callback;
    }

    void application::setOnUpdate(const std::function<void(void*)>& callback) noexcept {
        _onUpdate = callback;
    }

    void application::setOnFrame(const std::function<void(void*)>& callback) noexcept {
        _onFrame = callback;
    }

    void application::init(const application_info& info) noexcept {
        _nativeResources = native_resources(info);
    }

    const void * application::getViewport() noexcept {
        return reinterpret_cast<const void * > (&_viewport);
    }

    const void * application::getScissorRect() noexcept {
        return reinterpret_cast<const void * > (&_scissorRect);
    }

    void * application::getContext() noexcept {
        return reinterpret_cast<void * > (_nativeResources.pWindow);
    }

    double application::getTime() noexcept {
        return _time;
    }

    void application::setUserData(const std::shared_ptr<void>& pUserData) noexcept {
        _pUserData = pUserData;
    }

    void application::start() noexcept {
#if defined(__EMSCRIPTEN__)
        emscripten_set_main_loop(_doFrame, 0, 1);
#else
        while (_nativeResources.isValid()) {
            _doFrame();
        }
#endif
    }

    void application::registerCharCallback(const std::function<bool(unsigned int)>& callback) noexcept {
        _charCallbacks.push_back(callback);
    }

    void application::registerMouseButtonCallback(const std::function<bool(int, int, int)>& callback) noexcept {
        _mouseButtonCallbacks.push_back(callback);
    }

    void application::registerScrollCallback(const std::function<bool(double, double)>& callback) noexcept {
        _scrollCallbacks.push_back(callback);
    }

    void application::setClipboardString(const std::string& content) noexcept {
        glfwSetClipboardString(_nativeResources.pWindow, content.c_str());
    }

    std::string application::getClipboardString() noexcept {
        return glfwGetClipboardString(_nativeResources.pWindow);
    }
}

namespace {
    void _handleKeyEvent(GLFWwindow * pWindow, unsigned int codepoint) noexcept {
        for (auto&& callback : _charCallbacks) {
            if (callback(codepoint)) {
                return;
            }
        }
    }

    void _handleScrollEvent(GLFWwindow * pWindow, double sx, double sy) noexcept {
        for (auto&& callback : _scrollCallbacks) {
            if (callback(sx, sy)) {
                return;
            }
        }
    }

    void _handleMouseButtonEvent(GLFWwindow * pWindow, int button, int action, int mods) noexcept {
        for (auto&& callback : _mouseButtonCallbacks) {
            if (callback(button, action, mods)) {
                return;
            }
        }
    }    

    void _onError(const std::string& src, const std::string& err, const std::string& msg) noexcept {
        std::cerr << "[" << src << "] " << err << ": " << msg << std::endl;
    }

    void _onGLFWError(int error, const char * description) noexcept { 
        auto __fmtErr = [](auto strErr, auto strDesc){
            _onError("GLFW",
                (strErr) ? strErr : "<null>",
                (strDesc) ? strDesc : "<null");
        };

        switch (error) {
            case GLFW_NOT_INITIALIZED:
                __fmtErr("Not initialized", description);
                break;
            case GLFW_NO_CURRENT_CONTEXT:
                __fmtErr("No current context", description);
                break;
            case GLFW_INVALID_ENUM:
                __fmtErr("Invalid enum", description);
                break;
            case GLFW_INVALID_VALUE:
                __fmtErr("Invalid value", description);
                break;
            case GLFW_OUT_OF_MEMORY:
                __fmtErr("Out of memory", description);
                break;
            case GLFW_API_UNAVAILABLE:
                __fmtErr("API unavailable", description);
                break;
            case GLFW_VERSION_UNAVAILABLE:
                __fmtErr("Version unavailable", description);
                break;
            case GLFW_PLATFORM_ERROR:
                __fmtErr("Platform error", description);
                break;
            case GLFW_FORMAT_UNAVAILABLE:
                __fmtErr("Format unavailable", description);
                break;
            case GLFW_NO_WINDOW_CONTEXT:
                __fmtErr("No window context", description);
                break;
            default:
                __fmtErr("Unknown error", description);
                break;
        }
    }

    native_resources::native_resources(const engine::application_info& info) noexcept {
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

            std::cout << "[GLFW] GLFW Version: " << major << "." << minor << "." << revision << std::endl;
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

        if (static_cast<unsigned int> (info.hints & engine::application_hint::DEBUG)) {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);        
        }        
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.apiVersion.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.apiVersion.minor);        
#else
#error "No GL version specified!"
#endif


        if ((pWindow = glfwCreateWindow(info.window.width, info.window.height, info.name.c_str(), nullptr, nullptr)) == nullptr) {
            std::cerr << "[GLFW] Window creation failed!" << std::endl;
            __builtin_trap();
        }

        _viewport = {0, 0, static_cast<unsigned int> (info.window.width), static_cast<unsigned int> (info.window.height)};

        glfwMakeContextCurrent(pWindow);

        graphics::init();
        
        if (static_cast<unsigned int> (info.hints & engine::application_hint::VSYNC)) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }

        glfwSetFramebufferSizeCallback(pWindow, _updateFramebufferSize);
        glfwSetCharCallback(pWindow, _handleKeyEvent);
        glfwSetMouseButtonCallback(pWindow, _handleMouseButtonEvent);
        glfwSetScrollCallback(pWindow, _handleScrollEvent);

        audio::init(); 
    }

    native_resources::~native_resources() {        
        _scene = engine::layers::scene();

        if (pWindow) {
            glfwDestroyWindow(pWindow);
            glfwTerminate();
        }
    }

    bool native_resources::isValid() const noexcept {
        return pWindow && !glfwWindowShouldClose(pWindow);
    }

    void _updateFramebufferSize(GLFWwindow *, int width, int height) noexcept {
        _viewport.width = width;
        _viewport.height = height;
        _scissorRect = {true, 0, 0, static_cast<unsigned int> (width), static_cast<unsigned int> (height)};
    }

    void _doFrame() noexcept {        
        glfwPollEvents();     

        if (_onUpdate && _scene) {
            _scene.beginWrite();     
            _onUpdate(_pUserData.get());
            _scene.endWrite();
        }

        if (_onFrame) {
            _onFrame(_pUserData.get());            
        }        

        if (_scene) {
            _scene.render();
        }
        
        glfwSwapBuffers(_nativeResources.pWindow);    
    }
}
#include "engine/application.hpp"

#include <AL/alc.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <iostream>
#include <functional>
#include <string>


namespace engine {
    namespace {        
        static void _onError(const std::string& msg) {
            std::cerr << "ERR: " << msg << std::endl;
            __builtin_trap();
        }

        struct native_resources : public resources {
            GLFWwindow * _window;
            ALCdevice * _alcDevice;
            ALCcontext * _alcContext;

            native_resources(int width, int height, const std::string& title) {
                if (!glfwInit()) {
                    _onError("glfwInit failed!");
                }

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

            ~native_resources() {
                alcMakeContextCurrent(nullptr);
                alcDestroyContext(_alcContext);
                alcCloseDevice(_alcDevice);

                glfwDestroyWindow(_window);
                glfwTerminate();
            }

            void update() const {
                glfwSwapBuffers(_window);
                glfwPollEvents();                
            }

            bool isValid() const {
                return !glfwWindowShouldClose(_window);
            }            

            double getTime() const {
                return glfwGetTime();
            }
        };

#ifdef __EMSCRIPTEN__
        application * _pApp;

        void _doFrameStatic() {
            _pApp->doFrame();
            
        }
#endif
    }

    application::application(int width, int height, const std::string& title) {        
        _resources = std::make_unique<native_resources> (width, height, title);
    }

    void application::doFrame() const {
        _onFrame(userData.get());
        _resources->update();
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
        while (_resources->isValid()) {
            doFrame();
        }
#endif
    }
}
#include "engine/application.hpp"

#include <AL/alc.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <cstddef>
#include <iostream>
#include <functional>
#include <memory>
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
            double _time;
            button_state _keyboardStates[GLFW_KEY_LAST];
            struct mouse_state_T {
                button_state axes[2];
                button_state buttons[3];
            } _mouseStates;
            struct joystick_state_T {
                std::unique_ptr<button_state[]> axes;
                std::size_t numAxes;
                std::unique_ptr<button_state[]> buttons;
                std::size_t numButtons;
                bool connected;
            } _gamepadStates[4];

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

            void update() {
                glfwSwapBuffers(_window);
                glfwPollEvents();
                _time = glfwGetTime();        
            }

            bool isValid() const {
                return !glfwWindowShouldClose(_window);
            }            

            double getTime() const {
                return _time;
            }

            bool isAvailable(input_type input) const {
                switch (input) {
                    case input_type::KEYBOARD:
                    case input_type::MOUSE:
                        return true;
                    case input_type::GAMEPAD0:
                        return _gamepadStates[0].connected;
                    case input_type::GAMEPAD1:
                        return _gamepadStates[1].connected;
                    case input_type::GAMEPAD2:
                        return _gamepadStates[2].connected;
                    case input_type::GAMEPAD3:
                        return _gamepadStates[3].connected;
                    default:
                        _onError("Invalid input type!");
                }
            }

            button_state _getGamepadButton(int gamepadId, int buttonId) const {
                const auto& gamepad = _gamepadStates[gamepadId];

                return gamepad.connected ? gamepad.buttons[buttonId] : button_state();
            }            

            button_state getButton(input_type input, int buttonId) const {
                switch (input) {
                    case input_type::MOUSE:                        
                        return _mouseStates.buttons[buttonId];
                    case input_type::KEYBOARD:
                        return _keyboardStates[buttonId];
                    case input_type::GAMEPAD0:
                        return _getGamepadButton(0, buttonId);
                    case input_type::GAMEPAD1:
                        return _getGamepadButton(1, buttonId);
                    case input_type::GAMEPAD2:
                        return _getGamepadButton(2, buttonId);
                    case input_type::GAMEPAD3:
                        return _getGamepadButton(3, buttonId);
                    default:
                        _onError("Invalid input type!");
                }
            }

            std::size_t _getGamepadButtonCount(int gamepadId) const {
                const auto& gamepad = _gamepadStates[gamepadId];

                return gamepad.connected ? gamepad.numButtons : 0;
            }            

            std::size_t getButtonCount(input_type input) const {
                switch (input) {
                    case input_type::MOUSE:
                        return 3;
                    case input_type::KEYBOARD:
                        return GLFW_KEY_LAST;
                    case input_type::GAMEPAD0:
                        return _getGamepadButtonCount(0);
                    case input_type::GAMEPAD1:  
                        return _getGamepadButtonCount(1);
                    case input_type::GAMEPAD2:
                        return _getGamepadButtonCount(2);
                    case input_type::GAMEPAD3:
                        return _getGamepadButtonCount(3);
                    default:
                        _onError("Invalid input type!");
                }
            }

            button_state _getGamepadAxis(int gamepadId, int axisId) const {
                const auto& gamepad = _gamepadStates[gamepadId];

                return gamepad.connected ? gamepad.axes[axisId] : button_state();
            }

            button_state getAxis(input_type input, int axisId) const {
                switch (input) {
                    case input_type::MOUSE:
                        return _mouseStates.axes[axisId];
                    case input_type::KEYBOARD:
                        return button_state();
                    case input_type::GAMEPAD0:
                        return _getGamepadAxis(0, axisId);
                    case input_type::GAMEPAD1:
                        return _getGamepadAxis(1, axisId);
                    case input_type::GAMEPAD2:
                        return _getGamepadAxis(2, axisId);
                    case input_type::GAMEPAD3:
                        return _getGamepadAxis(3, axisId);
                    default:
                        _onError("Invalid input type!");
                }
            }

            std::size_t _getGamepadAxesCount(int gamepadId) const {
                const auto& gamepad = _gamepadStates[gamepadId];

                return gamepad.connected ? gamepad.numAxes : 0;
            }

            std::size_t getAxesCount(input_type input) const {
                switch (input) {
                    case input_type::KEYBOARD:
                        return 0;
                    case input_type::MOUSE:
                        return 2;
                    case input_type::GAMEPAD0:
                        return _getGamepadAxesCount(0);
                    case input_type::GAMEPAD1:
                        return _getGamepadAxesCount(1);
                    case input_type::GAMEPAD2:
                        return _getGamepadAxesCount(2);
                    case input_type::GAMEPAD3:
                        return _getGamepadAxesCount(3);
                    default:
                        _onError("Invalid input type!");
                }
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

    void application::doFrame() {
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
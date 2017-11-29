#pragma once

#include <cstddef>

#include <functional>
#include <memory>
#include <string>

namespace engine {
    struct button_state {
        double value;
        double time;
    };

    enum class input_type {
        KEYBOARD,
        MOUSE,
        GAMEPAD0,
        GAMEPAD1,
        GAMEPAD2,
        GAMEPAD3
    };

    constexpr int MOUSE_LEFT = 0;
    constexpr int MOUSE_MIDDLE = 1;
    constexpr int MOUSE_RIGHT = 2;
    constexpr int MOUSE_X = 0;
    constexpr int MOUSE_Y = 1;

    struct resources {
        virtual ~resources() {}

        virtual void update() = 0;

        virtual bool isValid() const = 0;

        virtual double getTime() const = 0;

        virtual button_state getButton(input_type input, int buttonId) const = 0;

        virtual std::size_t getButtonCount(input_type input) const = 0;

        virtual button_state getAxis(input_type input, int axisId) const = 0;

        virtual std::size_t getAxesCount(input_type input) const = 0;

        virtual bool isAvailable(input_type input) const = 0;
    };

    class application {
        std::unique_ptr<resources> _resources;        
        std::function<void(application *)> _onFrame;

        application(const application&) = delete;

        application& operator=(application&&) = default;

    public:
        std::shared_ptr<void> userData;

        application() :
            _resources(nullptr),
            _onFrame(nullptr),
            userData(nullptr) {}

        application(int width, int height, const std::string& title);        

        application(application&&) = default;

        application& operator=(const application&) = delete;            

        void setOnFrame(const std::function<void(application *)>& fn);

        void doFrame();

        void start(const std::function<void(application *)>& fn);

        inline double getTime() const {
            return _resources->getTime();
        }

        inline button_state getButton(input_type input, int buttonId) const {
            return _resources->getButton(input, buttonId);
        }

        inline std::size_t getButtonCount(input_type input) const {
            return _resources->getButtonCount(input);
        }

        inline button_state getAxis(input_type input, int axisId) const {
            return _resources->getAxis(input, axisId);
        }

        inline std::size_t getAxesCount(input_type input) const {
            return _resources->getAxesCount(input);
        }

        inline bool isAvailable(input_type input) const {
            return _resources->isAvailable(input);
        }
    };
}
#pragma once

#include <cstddef>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "engine/gui/window.hpp"

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

    namespace gui {
        class component;
    }

    namespace application {
        void init(const std::string& name, unsigned int width, unsigned int height);

        void setOnFrame(const std::function<void(void*)>& callback);

        void start(const std::shared_ptr<void>& pUserData);

        double getTime();

        void add(const std::shared_ptr<engine::gui::window>& window);
        
        void addAll(const std::vector<std::shared_ptr<engine::gui::window>>& windows);
    }
}
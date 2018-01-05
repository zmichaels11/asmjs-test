#pragma once

#include <cstddef>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "renderer/scene.hpp"
#include "renderer/scene_info.hpp"

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
        void init(const std::string& name, unsigned int width, unsigned int height) noexcept;

        void setOnFrame(const std::function<void(void*)>& callback) noexcept;

        void setOnUpdate(const std::function<void(void*)>& callback) noexcept;

        void setScene(const std::shared_ptr<renderer::scene>& scene) noexcept;

        void setScene(const renderer::scene_info& sceneInfo) noexcept;

        renderer::scene * getScene() noexcept;

        void start(const std::shared_ptr<void>& pUserData) noexcept;

        double getTime() noexcept;
    }
}
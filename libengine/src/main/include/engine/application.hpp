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
        void init(const std::string& name, unsigned int width, unsigned int height);

        void setOnFrame(const std::function<void(void*)>& callback);

        void setOnUpdate(const std::function<void(void*)>& callback);

        void setScene(const std::shared_ptr<renderer::scene>& scene);

        void setScene(const renderer::scene_info& sceneInfo);

        renderer::scene * getScene();

        void start(const std::shared_ptr<void>& pUserData);

        double getTime();                 
    }
}
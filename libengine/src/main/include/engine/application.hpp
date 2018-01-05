#pragma once

#include <cstddef>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "renderer/scene.hpp"
#include "renderer/scene_info.hpp"

namespace engine {
    class application {
        application() = delete;
        
    public:
        static void init(const std::string& name, unsigned int width, unsigned int height) noexcept;

        static void setOnFrame(const std::function<void(void*)>& callback) noexcept;

        static void setOnUpdate(const std::function<void(void*)>& callback) noexcept;

        static void setScene(const std::shared_ptr<renderer::scene>& scene) noexcept;

        static void setScene(const renderer::scene_info& sceneInfo) noexcept;

        static renderer::scene * getScene() noexcept;

        static void start(const std::shared_ptr<void>& pUserData) noexcept;

        static double getTime() noexcept;
    };
}
#pragma once

#include <functional>
#include <memory>

#include "engine/application_info.hpp"

#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene.hpp"

namespace engine {
    class application {
        application() = delete;
        
    public:
        static void init(const application_info& info) noexcept;

        static void setOnInit(const std::function<void(void*)>& callback) noexcept;

        static void setOnFrame(const std::function<void(void*)>& callback) noexcept;

        static void setOnUpdate(const std::function<void(void*)>& callback) noexcept;

        static void setScene(const engine::layers::scene_info& info) noexcept;

        static std::unique_ptr<engine::layers::scene> releaseScene() noexcept;

        static engine::layers::scene * getScene() noexcept;

        static void setUserData(const std::shared_ptr<void>& pUserData) noexcept;

        static void start() noexcept;

        static double getTime() noexcept;

        static const void * getViewport() noexcept;

        static const void * getScissorRect() noexcept;
    };
}
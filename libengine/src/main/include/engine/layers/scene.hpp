#pragma once

#include <memory>

#include "engine/layers/base_resources.hpp"
#include "engine/layers/base_layer.hpp"
#include "engine/layers/scene_info.hpp"

namespace engine {
    namespace layers {        
        class scene {
            std::unique_ptr<base_resources> _pResources;

            scene(const scene&) = delete;

            scene& operator=(const scene&) = delete;

        public:
            scene(scene&&) = default;

            scene& operator=(scene&&) = default;

            scene(const scene_info& info) noexcept;

            void beginWrite() noexcept;

            void endWrite() noexcept;

            void render() noexcept;

            base_layer * getLayer(int layerId) noexcept;

            const base_layer * getLayer(int layerId) const noexcept;

            const scene_info& getInfo() const noexcept;

            context * getContext() noexcept;
        };
    }
}
#pragma once

#include <cstddef>

#include "renderer/scene_info.hpp"

namespace renderer {
    class scene {
    public:
        virtual ~scene() {}

        virtual void onFrameStart() = 0;

        virtual void onFrameEnd() = 0;

        virtual const renderer::scene_info& getInfo() const = 0;

        virtual renderer::layer * getLayer(int id) const = 0;

        virtual std::size_t getLayerCount() const = 0;

        virtual layer_type getLayerType(int id) const = 0;
    };
}
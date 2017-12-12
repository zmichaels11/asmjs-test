#pragma once

#include <cstddef>

#include <memory>
#include <vector>

#include "renderer/layer.hpp"
#include "renderer/scene_info.hpp"

namespace renderer {
    struct scene_res {
        virtual ~scene_res() {}        
    };

    class scene {
        scene_info _info;
        std::vector<std::shared_ptr<renderer::layer>> _layers;
        std::shared_ptr<scene_res> _resources;      

    public:        
        scene(const scene_info& info);

        virtual ~scene() {}

        void update(double timestep);

        void doFrame(double timestep);

        const renderer::scene_info& getInfo() const;

        const renderer::layer * getLayer(int id) const;

        std::size_t getLayerCount() const;

        layer_type getLayerType(int id) const;
    };
}
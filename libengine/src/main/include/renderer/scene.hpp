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

    struct layer_res {
        std::shared_ptr<renderer::layer> drawLayer;
        render_info renderInfo;
    };

    class scene {
        scene_info _info;
        std::vector<layer_res> _layers;
        std::shared_ptr<scene_res> _resources;      

    public:        
        scene(const scene_info& info);

        virtual ~scene() {}

        void update();

        void render();

        const renderer::scene_info& getInfo() const;

        renderer::layer * getLayer(int id) const;

        const renderer::render_info& getLayerRenderInfo(int id) const;

        std::size_t getLayerCount() const;

        layer_type getLayerType(int id) const;
    };
}
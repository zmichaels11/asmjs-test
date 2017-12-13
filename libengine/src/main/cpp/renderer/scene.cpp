#include "renderer/scene.hpp"

#include <cstddef>

#include <iostream>
#include <memory>

#include "graphics/blend_state_info.hpp"
#include "graphics/clear_state_info.hpp"

#include "renderer/image_layer.hpp"
#include "renderer/image_layer_info.hpp"
#include "renderer/gui_layer.hpp"
#include "renderer/scene_info.hpp"

namespace renderer {
    namespace {
        struct scene_res_impl : public virtual renderer::scene_res{
            virtual ~scene_res_impl() {}

            graphics::clear_state_info clearInfo;
        };

        void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }
    }

    scene::scene(const renderer::scene_info& info) {
        _info = info;
        
        auto resources = std::make_shared<scene_res_impl>();

        resources->clearInfo.buffers = graphics::clear_buffer::COLOR | graphics::clear_buffer::DEPTH;
        resources->clearInfo.color = {info.clear.color.red, info.clear.color.green, info.clear.color.blue, info.clear.color.alpha};
        resources->clearInfo.depth = info.clear.depth;

        _resources = resources;

        for (int i = 0; i < info.layerInfoCount; i++) {
            auto& layerInfo = info.pLayerInfos[i];

            switch (layerInfo.type) {
                case renderer::layer_type::GUI:
                    _layers.push_back(std::make_shared<renderer::gui_layer>());
                    break;
                case renderer::layer_type::IMAGE:
                {
                    auto imageLayerInfo = static_cast<renderer::image_layer_info*> (layerInfo.pInfo);                  

                    if (imageLayerInfo) {
                        _layers.push_back(std::make_shared<renderer::image_layer> (*imageLayerInfo));
                    } else {
                        _onError("image_layer_info cannot be null!");
                    }
                    break;
                }
                default:
                    _onError("Not implemented yet!");
            }
        }
    }

    void scene::update() {
        for (auto&& layer : _layers) {
            layer->update();
        }
    }

    void scene::doFrame() {
        auto res = dynamic_cast<scene_res_impl*> (_resources.get());

        graphics::apply(res->clearInfo);
        graphics::apply(graphics::blendStatePremultiplyAlpha());

        for (auto&& layer : _layers) {
            layer->doFrame();
        }
    }

    const renderer::scene_info& scene::getInfo() const {
        return _info;
    }

    const renderer::layer * scene::getLayer(int id) const {
        return _layers[id].get();
    }

    std::size_t scene::getLayerCount() const {
        return _layers.size();
    }

    renderer::layer_type scene::getLayerType(int id) const {
        return _info.pLayerInfos[id].type;
    }
}
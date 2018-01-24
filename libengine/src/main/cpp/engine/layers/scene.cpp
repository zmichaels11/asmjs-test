#include "engine/layers/scene.hpp"

#include <memory>
#include <string>
#include <vector>

#include "util.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct scene_resources : public base_resources {                
                scene_info _info;
                context _context;
                std::vector<scene_layer> _layers;

                scene_resources(const scene_info& info) noexcept;

                virtual ~scene_resources() {}
            };
        }

        context& scene::getContext() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_context;
        }

        const context& scene::getContext() const noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_context;
        }

        scene::scene(const scene_info& info) noexcept {
            _pResources = std::make_unique<scene_resources> (info);
        }

        void scene::beginWrite() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            auto& layers = res->_layers;
            
            res->_context.beginWrite();

            for (auto&& layer : layers) {
                layer.layer->beginWrite();
            }
        }

        void scene::endWrite() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            auto& layers = res->_layers;

            res->_context.endWrite();

            for (auto&& layer : layers) {
                layer.layer->endWrite();
            }
        }

        void scene::invalidate() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            auto& layers = res->_layers;

            for (auto&& layer : layers) {
                layer.layer->invalidate();
            }
        }

        void scene::render() const noexcept {
            auto res = dynamic_cast<const scene_resources * > (_pResources.get());
            auto& layers = res->_layers;

            res->_context.render();

            for (auto&& layer : layers) {
                layer.layer->render();
            }
        }

        base_layer * scene::getLayer(int layerId) noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_layers[layerId].layer.get();
        }

        const base_layer * scene::getLayer(int layerId) const noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_layers[layerId].layer.get();
        }

        const scene_info& scene::getInfo() const noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] scene error: %s\n", msg.c_str());
                __builtin_trap();
            }
            
            scene_resources::scene_resources(const scene_info& info) noexcept {
                _info = info;
                _context = context(info.contextInfo);

                for (auto it = info.pLayerInfos; it != (info.pLayerInfos + info.nLayerInfos); it++) {
                    switch (it->type) {
                        case layer_type::BACKGROUND_LAYER: {
                            _layers.push_back({
                                *it,
                                std::make_unique<background_layer> (_context, it->info.backgroundLayer)});                            
                        } break;
                        case layer_type::BASIC_IMAGE_LAYER: {                            
                            _layers.push_back({
                                *it, 
                                std::make_unique<basic_image_layer> (_context, it->info.basicImageLayer)});
                        } break;
                        default: 
                            _onError("Invalid layer_type!");                        
                    }
                }
            }
        }
    }
}
#include "pch.h"
#include "engine/layers/scene.hpp"

#include "engine/application.hpp"

#include "engine/layers/background_layer.hpp"
#include "engine/layers/basic_sprite_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/gui_layer.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer.hpp"
#include "engine/layers/sound_layer.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;            

            struct scene_resources : public base_resources {                
                scene_info _info;
                context _context;
                std::vector<scene_layer> _layers;
                std::vector<std::function<void()>> _beginWriteCommands;
                std::vector<std::function<void()>> _endWriteCommands;
                std::vector<std::function<void()>> _renderCommands;      

                scene_resources(const scene_info& info) noexcept;

                virtual ~scene_resources() {}
            };
        }

        context * scene::getContext() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return &res->_context;
        }        

        scene::scene(const scene_info& info) noexcept {
            _pResources = std::make_unique<scene_resources> (info);
        }

        void scene::beginWrite() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            
            for (auto&& cmd : res->_beginWriteCommands) {
                cmd();
            }
        }

        void scene::endWrite() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            
            for (auto&& cmd : res->_endWriteCommands) {
                cmd();
            }
        }

        void scene::render() noexcept {
            auto res = dynamic_cast<const scene_resources * > (_pResources.get());                                                  
            
            for (auto&& cmd : res->_renderCommands) {
                cmd();
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
                std::cerr << "[render_engine] scene error: " << msg << std::endl;
                __builtin_trap();
            }
            
            scene_resources::scene_resources(const scene_info& info) noexcept {
                _info = info;
                _context = context(info.contextInfo);

                // register all of the context commands.
                _beginWriteCommands.push_back(std::bind(&context::beginWrite, &_context));
                _endWriteCommands.push_back(std::bind(&context::endWrite, &_context));                               
                _renderCommands.push_back(std::bind(&context::render, &_context));

                _renderCommands.push_back([](){                    
                    // switch back to the default framebuffer in case we switched away from it.
                    auto pViewport = reinterpret_cast<const graphics::viewport_state_info * > (application::getViewport());
                    auto pScissor = reinterpret_cast<const graphics::scissor_state_info * > (application::getScissorRect());

                    graphics::framebuffer::getDefault().bind();                    
                    graphics::apply(*pViewport);                    
                    graphics::apply(*pScissor);
                    graphics::apply(graphics::blend_state_info::premultipliedAlpha());
                });

                _context.bind();

                for (auto it = info.pLayerInfos; it != (info.pLayerInfos + info.nLayerInfos); it++) {
                    if (static_cast<unsigned int> (it->ext.hints & scene_layer_hint::CLEAR)) {                        
                        auto buffers = static_cast<graphics::clear_buffer> (0);

                        if (static_cast<unsigned int> (it->ext.clear.type & clear_type::COLOR)) {
                            buffers |= graphics::clear_buffer::COLOR;
                        }

                        if (static_cast<unsigned int> (it->ext.clear.type & clear_type::DEPTH)) {
                            buffers |= graphics::clear_buffer::DEPTH;
                        }

                        if (static_cast<unsigned int> (it->ext.clear.type & clear_type::STENCIL)) {
                            buffers |= graphics::clear_buffer::STENCIL;
                        }

                        auto clearStateInfo = graphics::clear_state_info{
                            buffers,
                            {
                                util::normalize(it->ext.clear.clearColor.r),
                                util::normalize(it->ext.clear.clearColor.g),
                                util::normalize(it->ext.clear.clearColor.b),
                                util::normalize(it->ext.clear.clearColor.a)},
                            it->ext.clear.clearDepth,
                            it->ext.clear.clearStencil};                        

                        _renderCommands.push_back(std::bind(&graphics::apply<graphics::clear_state_info>, clearStateInfo));
                    }

                    if (static_cast<unsigned int> (it->ext.hints & scene_layer_hint::SCISSOR)) {
                        //NOTE: this is actually wrong, scissor rect is from bottom-left
                        auto scissorStateInfo = graphics::scissor_state_info{
                            true,
                            it->ext.scissor.x,
                            it->ext.scissor.y,
                            it->ext.scissor.width,
                            it->ext.scissor.height};

                        _renderCommands.push_back(std::bind(&graphics::apply<graphics::scissor_state_info>, scissorStateInfo));
                    }

                    if (static_cast<unsigned int> (it->ext.hints& scene_layer_hint::VIEWPORT)) {
                        auto viewportStateInfo = graphics::viewport_state_info{
                            it->ext.viewport.x,
                            it->ext.viewport.y,
                            it->ext.viewport.width,
                            it->ext.viewport.height
                        };

                        _renderCommands.push_back(std::bind(&graphics::apply<graphics::viewport_state_info>, viewportStateInfo));
                    }

                    switch (it->type) {
                        case layer_type::BACKGROUND_LAYER: {
                            auto ptr = std::make_unique<background_layer> (_context, it->info.backgroundLayer);

                            _renderCommands.push_back(std::bind(&background_layer::render, ptr.get()));
                            _layers.push_back({*it, std::move(ptr)});              
                        } break;
                        case layer_type::BASIC_SPRITE_LAYER: {
                            auto ptr = std::make_unique<basic_sprite_layer> (_context, it->info.basicSpriteLayer);

                            _renderCommands.push_back(std::bind(&basic_sprite_layer::render, ptr.get()));
                            _beginWriteCommands.push_back(std::bind(&basic_sprite_layer::beginWrite, ptr.get()));
                            _endWriteCommands.push_back(std::bind(&basic_sprite_layer::endWrite, ptr.get()));

                            _layers.push_back({*it, std::move(ptr)});
                        } break;
                        case layer_type::GUI_LAYER: {
                            auto ptr = std::make_unique<gui_layer> (_context, it->info.guiLayer);

                            _renderCommands.push_back(std::bind(&gui_layer::render, ptr.get()));
                            _beginWriteCommands.push_back(std::bind(&gui_layer::beginWrite, ptr.get()));
                            _endWriteCommands.push_back(std::bind(&gui_layer::endWrite, ptr.get()));

                            _layers.push_back({*it, std::move(ptr)});                            
                        } break;
                        case layer_type::SOUND_LAYER: {
                            auto ptr = std::make_unique<sound_layer> (_context, it->info.soundLayer);

                            _beginWriteCommands.push_back(std::bind(&sound_layer::beginWrite, ptr.get()));
                            _endWriteCommands.push_back(std::bind(&sound_layer::endWrite, ptr.get()));

                            _layers.push_back({*it, std::move(ptr)});                            
                        } break;

                        default: 
                            _onError("Invalid layer_type!");                        
                    }
                }
            }
        }
    }
}
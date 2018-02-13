#include "pch.h"
#include "engine/layers/context.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/bitmap_font.hpp"
#include "engine/layers/renderable.hpp"
#include "engine/layers/renderable_image.hpp"
#include "engine/layers/sprite_sheet.hpp"
#include "engine/layers/tiled_image.hpp"

#include "audio.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct context_resources : public base_resources {
                context_info _info;
                std::vector<sprite_sheet> _spriteSheets;
                std::vector<bitmap_font> _fonts;
                std::vector<std::unique_ptr<renderable>> _renderables;
                std::vector<sound_info> _soundResources;

                std::vector<std::function<void()>> _beginWriteCommands;
                std::vector<std::function<void()>> _endWriteCommands;
                std::vector<std::function<void()>> _renderCommands;

                context_resources(const context_info& info);

                virtual ~context_resources() {}
            };
        }

        void context::bind() noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            for (auto&& renderable : res->_renderables) {
                renderable->bind(this);
            }
        }

        void context::beginWrite() noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            for (auto&& cmd : res->_beginWriteCommands) {
                cmd();
            }
        }

        void context::endWrite() noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            for (auto&& cmd : res->_endWriteCommands) {
                cmd();
            }
        }

        void context::render() noexcept {
            auto res = dynamic_cast<const context_resources * > (_pResources.get());

            for (auto&& cmd : res->_renderCommands) {
                cmd();
            }
        }

        context::context(const context_info& info) noexcept {
            _pResources = std::make_unique<context_resources> (info);
        }

        const sprite_sheet * context::getSpriteSheet(int id) const noexcept {
            auto res = dynamic_cast<const context_resources * > (_pResources.get());            

            return res->_spriteSheets.data() + id;
        }

        const bitmap_font * context::getFont(int id) const noexcept {
            auto res = dynamic_cast<const context_resources * > (_pResources.get());
            
            return res->_fonts.data() + id;
        }

        const context_info& context::getInfo() const noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            return res->_info;
        }

        renderable * context::getRenderableImage(int id) noexcept {
            auto res = dynamic_cast<context_resources * > (_pResources.get());

            return res->_renderables[id].get();
        }

        const renderable * context::getRenderableImage(int id) const noexcept {
            auto res = dynamic_cast<const context_resources * > (_pResources.get());

            return res->_renderables[id].get();
        }

        std::unique_ptr<audio::sound_channel> context::openSoundChannel(int id) const noexcept {
            auto res = dynamic_cast<const context_resources * > (_pResources.get());
            auto& info = res->_soundResources[id];

            switch (info.type) {
                case sound_type::VORBIS:
                    return std::make_unique<audio::vorbis_channel> (info.data, info.len);
                case sound_type::WAVE:
                    return std::make_unique<audio::wave_memory_channel> (info.data, info.len);
                default:
                    _onError("Unknown sound type!");
            }
            
        }

        namespace {
            void _onError(const std::string& msg) noexcept {                
                std::cerr << "[render_engine] context error: " << msg << std::endl;
                __builtin_trap();
            }

            context_resources::context_resources(const context_info& info) {

                _info = info;
                _spriteSheets.reserve(info.nSpriteInfos);
                _fonts.reserve(info.nFontInfos);
                _renderables.reserve(info.nRenderableInfos);
                _soundResources.reserve(info.nSoundResources);

                for (decltype(info.nSoundResources) i = 0; i < info.nSoundResources; i++) {
                    if (info.pSoundResources[i].type == sound_type::WAVE) {
                        std::cerr << "Memory streaming of WAVE data is experimental!" << std::endl;
                    }

                    _soundResources.push_back(info.pSoundResources[i]);
                }

                for (decltype(info.nSpriteInfos) i = 0; i < info.nSpriteInfos; i++) {
                    auto spriteSheet = sprite_sheet(info.pSpriteInfos[i]);    

                    _spriteSheets.push_back(std::move(spriteSheet));   
                }                

                for (decltype(info.nFontInfos) i = 0; i < info.nFontInfos; i++) {
                    auto bmpFont = bitmap_font(info.pFontInfos[i]);

                    _fonts.push_back(std::move(bmpFont));
                }

                for (decltype(info.nRenderableInfos) i = 0; i < info.nRenderableInfos; i++) {
                    auto pInfo = info.pRenderableInfos + i;

                    switch (pInfo->type) {
                        case renderable_type::IMAGE:{
                            auto ptr = std::make_unique<renderable_image>(pInfo->info.imageInfo);

                            _endWriteCommands.push_back(std::bind(&renderable_image::endWrite, ptr.get()));

                            _renderables.push_back(std::move(ptr));
                        } break;
                        case renderable_type::TILED_IMAGE: {
                            auto ptr = std::make_unique<tiled_image> (pInfo->info.tiledImageInfo);

                            _beginWriteCommands.push_back(std::bind(&tiled_image::beginWrite, ptr.get()));
                            _endWriteCommands.push_back(std::bind(&tiled_image::endWrite, ptr.get()));
                            _renderCommands.push_back(std::bind(&tiled_image::render, ptr.get()));

                            _renderables.push_back(std::move(ptr));
                        } break;
                        default:
                            _onError("Unsupported renderable_type!");                            
                    }
                }
            }
        }
    }
}
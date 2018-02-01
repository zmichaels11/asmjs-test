#include "engine/layers/context.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "engine/layers/base_resources.hpp"
#include "engine/layers/bitmap_font.hpp"
#include "engine/layers/renderable.hpp"
#include "engine/layers/renderable_image.hpp"
#include "engine/layers/sprite_sheet.hpp"
#include "engine/layers/tiled_image.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct context_resources : public base_resources {
                context_info _info;
                std::vector<sprite_sheet> _spriteSheets;
                std::vector<bitmap_font> _fonts;
                std::vector<std::unique_ptr<renderable>> _renderables;
                std::vector<std::function<void()>> _beginWriteCommands;
                std::vector<std::function<void()>> _endWriteCommands;
                std::vector<std::function<void()>> _renderCommands;

                context_resources(
                    const context * pctx,
                    const context_info& info);

                virtual ~context_resources() {}
            };
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

        void context::render() const noexcept {
            auto res = dynamic_cast<const context_resources * > (_pResources.get());

            for (auto&& cmd : res->_renderCommands) {
                cmd();
            }
        }

        context::context(const context_info& info) noexcept {
            _pResources = std::make_unique<context_resources> (this, info);
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

        namespace {
            void _onError(const std::string& msg) noexcept {                
                std::cerr << "[render_engine] context error: " << msg << std::endl;
                __builtin_trap();
            }

            context_resources::context_resources(
                const context * pctx,
                const context_info& info) {

                _info = info;
                _spriteSheets.reserve(info.nSpriteInfos);
                _fonts.reserve(info.nFontInfos);
                _renderables.reserve(info.nRenderableInfos);

                for (decltype(info.nSpriteInfos) i = 0; i < info.nSpriteInfos; i++) {                    
                    _spriteSheets.push_back(sprite_sheet(info.pSpriteInfos[i]));
                }

                for (decltype(info.nFontInfos) i = 0; i < info.nFontInfos; i++) {
                    _fonts.push_back(bitmap_font(info.pFontInfos[i]));
                }

                for (decltype(info.nRenderableInfos) i = 0; i < info.nRenderableInfos; i++) {
                    auto pInfo = info.pRenderableInfos + i;

                    switch (pInfo->type) {
                        case renderable_type::IMAGE:{
                            auto ptr = std::make_unique<renderable_image>(pctx, pInfo->info.imageInfo);

                            _endWriteCommands.push_back(std::bind(&renderable_image::endWrite, ptr.get()));

                            _renderables.push_back(std::move(ptr));
                        } break;
                        case renderable_type::TILED_IMAGE:
                            _renderables.push_back(std::make_unique<tiled_image>(pctx, pInfo->info.tiledImageInfo));
                            break;
                        default:
                            _onError("Unsupported renderable_type!");                            
                    }
                }
            }
        }
    }
}
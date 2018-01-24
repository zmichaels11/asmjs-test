#include "engine/layers/sprite_sheet.hpp"

#include <cstdio>

#include <algorithm>
#include <memory>

#include "engine/image_cache.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/sprite_sheet_info.hpp"

#include "graphics/image.hpp"
#include "graphics/texture.hpp"

#include "util.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct sprite_sheet_resources : public engine::layers::base_resources {
                engine::layers::sprite_sheet_info _info;
                std::unique_ptr<image_view[]> _sprites;
                
                graphics::texture _texture;

                sprite_sheet_resources(const sprite_sheet_info& info) noexcept;

                virtual ~sprite_sheet_resources() {}

                void buildLayeredSpritesheet() noexcept;

                void buildSpritesheet() noexcept;
            };
        }

        sprite_sheet::sprite_sheet(const engine::layers::sprite_sheet_info& info) noexcept {
            _pResources = std::make_unique<sprite_sheet_resources> (info);
        }

        const engine::layers::image_view& sprite_sheet::getSprite(int spriteID) const noexcept {
            auto res = dynamic_cast<sprite_sheet_resources * > (_pResources.get());
             
            return res->_sprites[spriteID];
        }

        const engine::layers::sprite_sheet_info& sprite_sheet::getInfo() const noexcept {
            auto res = dynamic_cast<sprite_sheet_resources * > (_pResources.get());

            return res->_info;
        }

        const void * sprite_sheet::getTexture() const noexcept {
            auto res = dynamic_cast<sprite_sheet_resources * > (_pResources.get());

            return &res->_texture;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] sprite_sheet error: %s\n", msg.c_str());
                __builtin_trap();
            }

            void sprite_sheet_resources::buildSpritesheet() noexcept {
                //TODO: support non-layered sprites
                buildLayeredSpritesheet();
            }

            void sprite_sheet_resources::buildLayeredSpritesheet() noexcept {
                unsigned int width = 0;
                unsigned int height = 0;                                

                for (decltype(_info.imageCount) i = 0; i < _info.imageCount; i++) {
                    auto pImage = _info.ppImages[i];

                    width = std::max(width, pImage->getWidth());
                    height = std::max(height, pImage->getHeight());
                }

                width = util::bestFitPowerOf2(width);
                height = util::bestFitPowerOf2(height);

                auto pixelsPerImage = width * height;
                auto totalPixels = pixelsPerImage * _info.imageCount;
                auto totalBytes = totalPixels * 4;
                auto samplerInfo = graphics::sampler_info::defaults();

                if ((_info.hints & sprite_sheet_hint::HORIZONTAL_REPEAT) == static_cast<sprite_sheet_hint> (0)) {
                    samplerInfo.addressing.wrapS = graphics::address_mode::CLAMP_TO_EDGE;
                }

                if ((_info.hints & sprite_sheet_hint::VERTICAL_REPEAT) == static_cast<sprite_sheet_hint> (0)) {
                    samplerInfo.addressing.wrapT = graphics::address_mode::CLAMP_TO_EDGE;
                }

                unsigned int levelCount = 1;

                if ((_info.hints & sprite_sheet_hint::GENERATE_MIPMAP) == static_cast<sprite_sheet_hint> (0)) {
                    samplerInfo.filters.mag = graphics::mag_filter::NEAREST;
                    samplerInfo.filters.min = graphics::min_filter::NEAREST;
                } else {
                    samplerInfo.filters.mag = graphics::mag_filter::NEAREST;
                    samplerInfo.filters.min = graphics::min_filter::NEAREST_MIPMAP_NEAREST;
                    levelCount = util::optimalMipmapCount<unsigned int>(width, height, 1);
                }
                
                _sprites = std::make_unique<image_view[]> (_info.imageCount);
                _texture = graphics::texture(graphics::texture_info{
                    {width, height, 1},
                    _info.imageCount,
                    levelCount,
                    samplerInfo,
                    graphics::internal_format::RGBA8
                });

                unsigned int currentLayer = 0;

                for (decltype(_info.imageCount) i = 0; i < _info.imageCount; i++) {
                    auto pImage = _info.ppImages[i];
                    auto pixelInfo = graphics::pixel_info{
                        graphics::pixel_type::UNSIGNED_BYTE,
                        graphics::pixel_format::RGBA,
                        const_cast<void*> (pImage->getData())};
                    auto w = pImage->getWidth();
                    auto h = pImage->getHeight();
                    auto sw = static_cast<float> (w) / static_cast<float> (width);
                    auto sh = static_cast<float> (h) / static_cast<float> (height);

                    _sprites[currentLayer] = {static_cast<float> (currentLayer), util::unorm<std::uint16_t> (sw), util::unorm<std::uint16_t> (sh)};                    
                    _texture.subImage(0, 0, 0, currentLayer, w, h, 1, pixelInfo);
                    currentLayer++;
                }
            }

            sprite_sheet_resources::sprite_sheet_resources(const sprite_sheet_info& info) noexcept {
                _info = info;

                if ((info.hints & sprite_sheet_hint::LAYERED) == sprite_sheet_hint::LAYERED) {
                    buildLayeredSpritesheet();
                } else {
                    buildSpritesheet();
                }
            }
        }
    }
}
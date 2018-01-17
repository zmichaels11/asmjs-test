#include "engine/layers/sprite_sheet.hpp"

#include <cstdio>

#include <algorithm>
#include <memory>
#include <map>
#include <string>

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
                std::map<std::string, engine::layers::image_view> _sprites;
                
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

        const engine::layers::image_view& sprite_sheet::getSprite(const std::string& lookup) const noexcept {
            auto res = dynamic_cast<sprite_sheet_resources * > (_pResources.get());

            return res->_sprites[lookup];
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
                buildLayeredSpritesheet();
            }

            void sprite_sheet_resources::buildLayeredSpritesheet() noexcept {
                unsigned int width = 0;
                unsigned int height = 0;
                auto layerCount = _info.requiredImages.size();
                auto images = std::map<std::string, const graphics::image*>();

                for (auto&& imgRef : _info.requiredImages) {
                    auto image = engine::image_cache::getImage(imgRef);

                    images.emplace(imgRef, image);
                    width = std::max(width, image->getWidth());
                    height = std::max(height, image->getHeight());                                      
                }

                width = util::bestFitPowerOf2(width);
                height = util::bestFitPowerOf2(height);

                auto pixelsPerImage = width * height;
                auto totalPixels = pixelsPerImage * layerCount;
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
                    levelCount = std::max(util::optimalMipmapCount(width), util::optimalMipmapCount(height));
                }
                
                _texture = graphics::texture(graphics::texture_info{
                    {width, height, 1},
                    layerCount,
                    levelCount,
                    samplerInfo,
                    graphics::internal_format::RGBA8
                });

                unsigned int currentLayer = 0;

                for (auto&& pair : images) {                    
                    auto& image = *pair.second;
                    auto pixelInfo = graphics::pixel_info{
                        graphics::pixel_type::UNSIGNED_BYTE,
                        graphics::pixel_format::RGBA,
                        const_cast<void*> (image.getData())
                    };

                    _texture.subImage(0, 0, 0, currentLayer, image.getWidth(), image.getHeight(), 1, pixelInfo);
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
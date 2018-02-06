#include "engine/layers/renderable_image.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "graphics/image.hpp"
#include "graphics/texture.hpp"

#include "util.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_filter_type.hpp"
#include "engine/layers/image_scroll_type.hpp"
#include "engine/layers/renderable_image_info.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            graphics::mag_filter _magFilterType(image_filter_type type) noexcept;

            graphics::min_filter _minFilterType(image_filter_type type) noexcept;

            graphics::address_mode _scrollType(image_scroll_type type) noexcept;

            struct renderable_image_resources : public base_resources {
                renderable_image_info _info;
                const graphics::image * _pImage;
                bool _dirty;                

                graphics::texture _texture;

                renderable_image_resources(const renderable_image_info& info) noexcept;

                virtual ~renderable_image_resources() {}
            };
        }

        void renderable_image::bind(const context * pCtx) noexcept {

        }

        renderable_image::renderable_image(const renderable_image_info& info) noexcept {

            _pResources = std::make_unique<renderable_image_resources> (info);
        }

        void renderable_image::invalidate() noexcept {
            auto res = dynamic_cast<renderable_image_resources * > (_pResources.get());

            res->_dirty = true;
        }

        void renderable_image::beginWrite() noexcept {}

        void renderable_image::endWrite() noexcept {
            auto res = dynamic_cast<renderable_image_resources * > (_pResources.get());

            if (res->_dirty && res->_pImage) {
                auto textureInfo = graphics::texture_info::ofImage(res->_pImage);

                textureInfo.samplerInfo = {
                    {_magFilterType(res->_info.filterType), _minFilterType(res->_info.filterType)},
                    {_scrollType(res->_info.scroll.horizontal), _scrollType(res->_info.scroll.vertical), graphics::address_mode::CLAMP_TO_EDGE},
                    {-1000.0F, 1000.0F}};                

                textureInfo.levels = res->_info.filterType == image_filter_type::TRILINEAR
                    ? util::optimalMipmapCount(res->_pImage->getWidth(), res->_pImage->getHeight(), 1u)
                    : 1;

                auto newTexture = graphics::texture(textureInfo);

                std::swap(res->_texture, newTexture);                
                
                res->_texture.subImage(0, 0, 0, 0, res->_pImage);

                if (textureInfo.levels > 1) {
                    res->_texture.generateMipmaps();
                }

                res->_dirty = false;
            }
        }

        void renderable_image::setImage(const graphics::image * pImage) noexcept {
            auto res = dynamic_cast<renderable_image_resources * > (_pResources.get());

            res->_dirty = true;
            res->_pImage = pImage;
        }

        void renderable_image::render() const noexcept {}

        const void * renderable_image::getTexture() const noexcept {
            auto res = dynamic_cast<const renderable_image_resources * > (_pResources.get());

            if (res->_texture) {
                return reinterpret_cast<const void * > (&res->_texture);
            } else {
                return nullptr;
            }
        }

        const renderable_image_info& renderable_image::getInfo() const noexcept {
            auto res = dynamic_cast<renderable_image_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::cerr << "[render_engine] renderable_image error: " << msg << std::endl;
                __builtin_trap();
            }

            graphics::mag_filter _magFilterType(image_filter_type type) noexcept {
                switch (type) {
                    case image_filter_type::NEAREST:
                        return graphics::mag_filter::NEAREST;
                    default:
                        return graphics::mag_filter::LINEAR;
                }
            }

            graphics::min_filter _minFilterType(image_filter_type type) noexcept {
                switch (type) {
                    case image_filter_type::NEAREST:
                        return graphics::min_filter::NEAREST;
                    case image_filter_type::BILINEAR:
                        return graphics::min_filter::LINEAR;
                    case image_filter_type::TRILINEAR:
                        return graphics::min_filter::LINEAR_MIPMAP_LINEAR;
                    default:
                        _onError("Unsupported image_filter_type!");
                }
            }

            graphics::address_mode _scrollType(image_scroll_type type) noexcept {
                switch (type) {
                    case image_scroll_type::REPEAT:
                        return graphics::address_mode::REPEAT;
                    case image_scroll_type::STATIC:
                        return graphics::address_mode::CLAMP_TO_EDGE;
                    default:
                        _onError("Unsupported image_filter_type!");
                }
            }

            renderable_image_resources::renderable_image_resources(const renderable_image_info& info) noexcept {

                _info = info;
                _dirty = true;
                _pImage = info.initialImage;                                
            }
        }
    }
}
#include "engine/layers/bitmap_font.hpp"

#include <memory>

#include "graphics/image.hpp"
#include "graphics/texture.hpp"

#include "engine/layers/base_resources.hpp"

namespace engine {
    namespace layers {
        namespace {
            struct bitmap_font_resources : public base_resources {
                graphics::font_image _image;
                graphics::texture _texture;

                bitmap_font_resources(const graphics::font_info& info);

                virtual ~bitmap_font_resources() {}
            };
        }

        const graphics::font_image& bitmap_font::getImage() const noexcept {
            auto res = dynamic_cast<bitmap_font_resources * > (_pResources.get());

            return res->_image;
        }

        const void * bitmap_font::getTexture() const noexcept {
            auto res = dynamic_cast<bitmap_font_resources * > (_pResources.get());
            auto pTexture = &res->_texture;

            return reinterpret_cast<const void * > (pTexture);
        }

        bitmap_font::bitmap_font(const graphics::font_info& info) noexcept {
            _pResources = std::make_unique<bitmap_font_resources> (info);
        }

        namespace {
            bitmap_font_resources::bitmap_font_resources(const graphics::font_info& info) {
                _image = graphics::font_image(info);
                _texture = graphics::texture({
                    {_image.getWidth(), _image.getHeight(), 1},
                    1, 1,
                    {
                        {graphics::mag_filter::NEAREST, graphics::min_filter::NEAREST},
                        {graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE},
                        {-1000.0F, 1000.0F}
                    },
                    graphics::internal_format::R8});

                _texture.subImage(0, 0, 0, 0, _image.getWidth(), _image.getHeight(), 1, {
                    graphics::pixel_type::UNSIGNED_BYTE,
                    graphics::pixel_format::RED,
                    const_cast<void *> (_image.getData())});
            }
        }
    }
}
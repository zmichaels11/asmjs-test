#include "renderer/image_layer.hpp"

#include <cmath>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "graphics/address_mode.hpp"
#include "graphics/blend_state_info.hpp"
#include "graphics/buffer.hpp"
#include "graphics/draw.hpp"
#include "graphics/draw_mode.hpp"
#include "graphics/mag_filter.hpp"
#include "graphics/min_filter.hpp"
#include "graphics/pixel_format.hpp"
#include "graphics/pixel_type.hpp"
#include "graphics/program.hpp"
#include "graphics/texture.hpp"
#include "graphics/uniform.hpp"
#include "graphics/vertex_array.hpp"

#include "renderer/color_transform.hpp"
#include "renderer/image_scroll_type.hpp"
#include "renderer/image_layer_info.hpp"


namespace renderer {
    namespace {
        constexpr float _TO_RADIANS = 3.14159265358F / 180.0F;
        struct image_layer_res_impl : public virtual image_layer_res {
            virtual ~image_layer_res_impl() {}

            struct uniform_t {
                int image;
                int mask;
                int data;
            } uniforms;

            graphics::blend_state_info * pBlendState;
            graphics::program * pProgram;
            graphics::texture image, mask;
            graphics::vertex_array model;
            bool supportsMask;
            float uniformData[5 * 4];
        };

        graphics::address_mode _addressMode(image_scroll_type scrollType);

        void _onError(const std::string& msg);

        graphics::texture _newTexture(const image_layer_info::sublayer_info_t& info);

        graphics::mag_filter _magFilter(image_filter filter);

        graphics::min_filter _minFilter(image_filter filter);
    }

    image_layer::image_layer(const image_layer_info& info) {
        _info = info;
        
        auto pResources = std::make_shared<image_layer_res_impl> ();

        if (info.image.image.data == nullptr) {
            _onError("image_layer must define an image!");
        }

        pResources->image = _newTexture(info.image);

        if (info.mask.image.data) {
            pResources->mask = _newTexture(info.mask);
            pResources->supportsMask = true;
        }        

        {
            auto newModel = graphics::vertex_array({nullptr, 0, nullptr, 0, nullptr});

            std::swap(pResources->model, newModel);
        }

        _pResources = pResources;
    }

    void image_layer::setColorTransform(const renderer::color_transform& ct) {
        auto res = dynamic_cast<image_layer_res_impl*> (_pResources.get());

        res->uniformData[8] = std::cos(_TO_RADIANS * ct.hsv.hue);
        res->uniformData[9] = std::sin(_TO_RADIANS * ct.hsv.hue);
        res->uniformData[10] = ct.hsv.saturation;
        res->uniformData[11] = ct.hsv.value;
        res->uniformData[12] = ct.rgbaOffset.red;
        res->uniformData[13] = ct.rgbaOffset.green;
        res->uniformData[14] = ct.rgbaOffset.blue;
        res->uniformData[15] = ct.rgbaOffset.alpha;
        res->uniformData[16] = ct.rgbaScale.red;
        res->uniformData[17] = ct.rgbaScale.green;
        res->uniformData[18] = ct.rgbaScale.blue;
        res->uniformData[19] = ct.rgbaScale.alpha;
    }

    void image_layer::scrollImage(float s0, float t0, float s1, float t1) {
        auto res = dynamic_cast<image_layer_res_impl*> (_pResources.get());

        res->uniformData[0] = s0;
        res->uniformData[1] = t0;
        res->uniformData[2] = s1;
        res->uniformData[3] = t1;
    }

    void image_layer::scrollMask(float s0, float t0, float s1, float t1) {
        auto res = dynamic_cast<image_layer_res_impl*> (_pResources.get());

        res->uniformData[0] = s0;
        res->uniformData[1] = t0;
        res->uniformData[2] = s1;
        res->uniformData[3] = t1;
    }

    const image_layer_info& image_layer::getInfo() const {
        return _info;
    }

    void image_layer::setProjection(const float * proj) {
        _onError("image_layer does not support projection matrix!");
    }

    void image_layer::update() {}

    void image_layer::doFrame() {
        auto res = dynamic_cast<image_layer_res_impl*> (_pResources.get());

        res->pProgram->use();

        graphics::uniform::setUniform4(res->uniforms.data, 5, res->uniformData);
        graphics::uniform::setUniform1(res->uniforms.image, 0);

        res->image.bind(0);

        if (res->supportsMask) {
            graphics::uniform::setUniform1(res->uniforms.mask, 1);
            res->mask.bind(1);
        }

        res->model.bind();

        graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
    }

    namespace {
        void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }

        graphics::mag_filter _magFilter(image_filter filter)  {
            switch (filter) {
                case image_filter::NEAREST:
                    return graphics::mag_filter::NEAREST;
                case image_filter::LINEAR:
                    return graphics::mag_filter::LINEAR;
                default:
                    _onError("Unsupported image filter!");
                    return graphics::mag_filter::LINEAR;
            }
        }

        graphics::min_filter _minFilter(image_filter filter) {
            switch (filter) {
                case image_filter::NEAREST:
                    return graphics::min_filter::NEAREST;
                case image_filter::LINEAR:
                    return graphics::min_filter::LINEAR;
                default:
                    _onError("Unsupported image filter!");
                    return graphics::min_filter::LINEAR;
            }
        }

        graphics::address_mode _addressMode(image_scroll_type scrollType) {
            switch (scrollType) {
                case image_scroll_type::STATIC:
                    return graphics::address_mode::CLAMP_TO_EDGE;
                case image_scroll_type::REPEAT:
                    return graphics::address_mode::REPEAT;
                default:
                    _onError("Unsupported scroll type!");
                    return graphics::address_mode::REPEAT;
            }
        }

        graphics::texture _newTexture(const image_layer_info::sublayer_info_t& info) {
            auto out = graphics::texture({
                {info.image.width, info.image.height, 1},
                1, 1,
                {
                    {_magFilter(info.magFilter), _minFilter(info.minFilter)},
                    {_addressMode(info.hScroll), _addressMode(info.vScroll), graphics::address_mode::CLAMP_TO_EDGE},
                    {-1000.0F, 1000.0F}
                },
                graphics::internal_format::RGBA8
            });

            out.subImage(0, 0, 0, 0, info.image.width, info.image.height, 1, {
                graphics::pixel_type::UNSIGNED_BYTE,
                graphics::pixel_format::RGBA,
                info.image.data
            });

            if (info.pColorTransform) {
                //TODO: apply color transform now
            }            

            return out;
        }
    }
}
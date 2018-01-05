#include "renderer/image_layer.hpp"

#include <cmath>
#include <cstdio>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <GLES3/gl3.h>

#include "graphics/address_mode.hpp"
#include "graphics/clear_state_info.hpp"
#include "graphics/draw.hpp"
#include "graphics/draw_mode.hpp"
#include "graphics/mag_filter.hpp"
#include "graphics/min_filter.hpp"
#include "graphics/pixel_format.hpp"
#include "graphics/pixel_type.hpp"
#include "graphics/program.hpp"
#include "graphics/program_info.hpp"
#include "graphics/scissor_state_info.hpp"
#include "graphics/shader.hpp"
#include "graphics/shader_info.hpp"
#include "graphics/texture.hpp"
#include "graphics/texture_info.hpp"
#include "graphics/uniform.hpp"
#include "graphics/vertex_array.hpp"

#include "renderer/color_transform.hpp"
#include "renderer/image_scroll_type.hpp"
#include "renderer/image_layer_info.hpp"

#include "util.hpp"


namespace renderer {
    namespace {
        constexpr float _TO_RADIANS = 3.14159265358F / 180.0F;
        
        struct image_layer_res_impl : public virtual image_layer_res {
            virtual ~image_layer_res_impl() {}

            graphics::texture image;
            graphics::vertex_array model;
            float uniformData[5 * 4];
        };

        graphics::program _newProgram(const std::string& vsh, const std::string& fsh);

        graphics::address_mode _addressMode(image_scroll_type scrollType);

        void _onError(const std::string& msg);

        graphics::mag_filter _magFilter(image_filter filter);

        graphics::min_filter _minFilter(image_filter filter);
    }

    image_layer::image_layer(const image_layer_info& info) {
        _info = info;   
        _scissor = renderer::scissor_rect{false};
        
        auto pResources = new image_layer_res_impl();

        if (info.image.pData == nullptr) {
            _onError("image_layer must define an image!");
        }

        {
            auto newImage = graphics::texture({
                    {info.image.width, info.image.height, 1},
                    1, 1,
                    {
                        {_magFilter(info.filter.mag), _minFilter(info.filter.min)},
                        {_addressMode(info.scroll.horizontal), _addressMode(info.scroll.vertical), graphics::address_mode::CLAMP_TO_EDGE},
                        {-1000.0F, 1000.0F}
                    },
                    graphics::internal_format::RGBA8
                });                        

            newImage.subImage(0, 0, 0, 0, info.image.width, info.image.height, 1, {
                graphics::pixel_type::UNSIGNED_BYTE,
                graphics::pixel_format::RGBA,
                const_cast<void*> (info.image.pData)
            });                  

            std::swap(pResources->image, newImage);
        }
            
        {
            auto newModel = graphics::vertex_array({nullptr, 0, nullptr, 0, nullptr});

            std::swap(pResources->model, newModel);
        }

        pResources->uniformData[0] = 0.0F;
        pResources->uniformData[1] = 0.0F;
        pResources->uniformData[2] = 1.0F;
        pResources->uniformData[3] = 1.0F;
        pResources->uniformData[4] = 0.0F;
        pResources->uniformData[5] = 0.0F;
        pResources->uniformData[6] = 1.0F;
        pResources->uniformData[7] = 1.0F;

        _pResources.reset(pResources);
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

    void image_layer::setScissor(const scissor_rect& scissor) {
        _scissor = scissor;
    }

    const scissor_rect& image_layer::getScissor() const {
        return _scissor;
    }

    void image_layer::setProjection(const float * proj) {
        _onError("image_layer does not support projection matrix!");
    }

    void image_layer::update() {}  

    void image_layer::render(const render_info& info) {
        auto res = dynamic_cast<image_layer_res_impl*> (_pResources.get());        

        switch (info.target) {
            case render_target::STENCIL: {
                static graphics::program STENCIL_PROGRAM;
                static int uImage, uData;

                if (STENCIL_PROGRAM == 0) {
                    auto newProgram = _newProgram("data/shaders/image_layer/stencil_300_ES.vert", "data/shaders/image_layer/stencil_300_ES.frag");

                    uImage = newProgram.getUniformLocation("uImage");
                    uData = newProgram.getUniformLocation("uData");

                    std::swap(STENCIL_PROGRAM, newProgram);
                }

                STENCIL_PROGRAM.use();

                graphics::uniform::setUniform1(uImage, 0);
                graphics::uniform::setUniform4(uData, 1, res->uniformData);
            }
            break;
            default: {
                static graphics::program COLOR_PROGRAM;
                static int uImage, uData;            

                if (COLOR_PROGRAM == 0) {
                    auto newProgram = _newProgram("data/shaders/image_layer/300_ES.vert", "data/shaders/image_layer/300_ES.frag");

                    uImage = newProgram.getUniformLocation("uImage");
                    uData = newProgram.getUniformLocation("uData");

                    std::swap(COLOR_PROGRAM, newProgram);                
                }

                COLOR_PROGRAM.use();

                graphics::uniform::setUniform1(uImage, 0);
                graphics::uniform::setUniform4(uData, 1, res->uniformData);
            }
            break;
        }
        
        res->model.bind();
        res->image.bind(0);

        if (_scissor.enabled) {
            auto left = static_cast<int> (_scissor.x);
            auto bottom = static_cast<int> (_scissor.y);
            auto width = static_cast<int> (_scissor.width);
            auto height = static_cast<int> (_scissor.height);
            auto scissorInfo = graphics::scissor_state_info{true, left, bottom, width, height};

            graphics::apply(scissorInfo);

            graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);        

            graphics::apply(graphics::scissor_state_info{false});
        } else {
            graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
        }
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

        graphics::program _newProgram(const std::string& v, const std::string& f) {
            auto vsrc = util::stringReadAll(v);
            auto fsrc = util::stringReadAll(f);
                    
            auto vsh = graphics::shader({graphics::shader_type::VERTEX, vsrc});
            auto fsh = graphics::shader({graphics::shader_type::FRAGMENT, fsrc});

            decltype(&vsh) shaders[] = {&vsh, &fsh};            

            return graphics::program({shaders, 2, nullptr, 0});
        }
    }
}
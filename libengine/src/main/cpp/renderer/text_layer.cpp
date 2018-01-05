#include "renderer/text_layer.hpp"

#include <cstdint>

#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>


#include "graphics/buffer.hpp"
#include "graphics/draw.hpp"
#include "graphics/draw_mode.hpp"
#include "graphics/font_image.hpp"
#include "graphics/program.hpp"
#include "graphics/scissor_state_info.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/uniform.hpp"
#include "graphics/vertex_array.hpp"

#include "util.hpp"

namespace renderer {
    namespace {
        constexpr float PIXELS_PER_INCH = 96.0F;
        constexpr float POINTS_PER_INCH = 72.0F;
        constexpr float PIXELS_PER_POINT = PIXELS_PER_INCH / POINTS_PER_INCH;
        constexpr unsigned int BYTES_PER_VERTEX = 16;
        constexpr unsigned int VERTICES_PER_CHARACTER = 6;
        constexpr unsigned int BYTES_PER_CHARACTER = BYTES_PER_VERTEX * VERTICES_PER_CHARACTER;

        graphics::program _newProgram(const std::string& v, const std::string& f) {
            auto vsrc = util::stringReadAll(v);
            auto fsrc = util::stringReadAll(f);
            auto vsh = graphics::shader({graphics::shader_type::VERTEX, vsrc});
            auto fsh = graphics::shader({graphics::shader_type::FRAGMENT, fsrc});

            decltype(&vsh) shaders[] = {&vsh, &fsh};

            graphics::attribute_state_info attribs[] = {
                {"vPosition", 0},
                {"vTexCoord", 1},
                {"vColor", 2}
            };

            return graphics::program({shaders, 2, attribs, 3});
        }

        void _onError(const std::string& msg) {
            std::cout << msg << std::endl;
            __builtin_trap();
        }

        std::uint16_t _tc(float tc) {
            return static_cast<std::uint16_t> (tc * 65535u);
        }

        struct vertex {
            float x, y;
            std::uint16_t s, t;
            std::uint8_t r, g, b, a;
        };

        struct text_layer_res_impl : public virtual text_layer_res {
            virtual ~text_layer_res_impl() {}

            float projection[16];            

            std::vector<vertex> vertices;

            unsigned int bufferSize;
            graphics::texture texture;
            graphics::buffer vtext;
            graphics::vertex_array model;
            graphics::font_image font;
        };
    }

    text_layer::text_layer(const text_layer_info& info) {
        _info = info;
        _scissor = {false};
                
        auto bufferSize = info.maxCharacters * BYTES_PER_CHARACTER;
        auto vtext = graphics::buffer({graphics::buffer_target::ARRAY, graphics::buffer_usage::STREAM_DRAW, {nullptr, bufferSize}});

        auto binding = graphics::vertex_binding_description {0, 16, 0, &vtext};
        auto aPosition = graphics::vertex_attribute_description {0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0};
        auto aTexCoord = graphics::vertex_attribute_description {1, graphics::vertex_format::X16Y16_UNORM, 8, 0};
        auto aColor = graphics::vertex_attribute_description {2, graphics::vertex_format::X8Y8Z8W8_UNORM, 12, 0};
        decltype(&aPosition) attribs[] = {&aPosition, &aTexCoord, &aColor};
        decltype(&binding) bindings[] = {&binding};

        auto model = graphics::vertex_array({attribs, 3, bindings, 1, nullptr});
        
        auto font = graphics::font_image({info.firstChar, info.charCount, info.fontFile, info.pointSize * PIXELS_PER_POINT});            

        auto texture = graphics::texture({
            {font.getWidth(), font.getHeight(), 1},
            1, 1,
            {
                {graphics::mag_filter::NEAREST, graphics::min_filter::NEAREST},
                {graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE},
                {-1000.0F, 1000.0F}
            },
            graphics::internal_format::R8
        });

        texture.subImage(0, 0, 0, 0, font.getWidth(), font.getHeight(), 1, {
            graphics::pixel_type::UNSIGNED_BYTE,
            graphics::pixel_format::RED,
            const_cast<void*> (font.getData())
        });        
        
        auto pResources = std::make_shared<text_layer_res_impl>();

        pResources->bufferSize = bufferSize;
        std::swap(pResources->vtext, vtext);
        std::swap(pResources->model, model);
        std::swap(pResources->font, font);
        std::swap(pResources->texture, texture);  

        _pResources = pResources;              
    }

    const text_layer_info& text_layer::getInfo() const {
        return _info;
    }

    void text_layer::update() {}

    void text_layer::render(const render_info& info) {
        static graphics::program PROGRAM;
        static int uFont, uProjection;

        if (PROGRAM == 0) {
            auto newProgram = _newProgram("data/shaders/text_layer/300_ES.vert", "data/shaders/text_layer/300_ES.frag");
            
            std::swap(PROGRAM, newProgram);

            uFont = PROGRAM.getUniformLocation("uFont");
            uProjection = PROGRAM.getUniformLocation("uProjection");
        }

        auto res = dynamic_cast<text_layer_res_impl*> (_pResources.get());

        if (res->vertices.size() == 0) {
            return;
        }

        auto drawLimit = res->vertices.size();

        res->model.bind();
        res->vtext.invalidate();        
        res->vtext.subData(0, res->vertices.data(), drawLimit * BYTES_PER_VERTEX);

        PROGRAM.use();

        graphics::uniform::setUniformMatrix4(uProjection, 1, res->projection);
        graphics::uniform::setUniform1(uFont, 0);

        res->texture.bind(0);        
        
        if (_scissor.enabled) {
            //TODO: bottom = (size.height - (y + height))
            auto left = static_cast<int> (_scissor.x);
            auto bottom = static_cast<int> (_scissor.y);
            auto width = static_cast<int> (_scissor.width);
            auto height = static_cast<int> (_scissor.height);
            auto scissorInfo = graphics::scissor_state_info{true, left, bottom, width, height};

            graphics::apply(scissorInfo);
            graphics::draw::arrays(graphics::draw_mode::TRIANGLES, 0, drawLimit);
            graphics::apply(graphics::scissor_state_info{false});
        } else {
            graphics::draw::arrays(graphics::draw_mode::TRIANGLES, 0, drawLimit);
        }        

        res->vertices.clear();
    }

    void text_layer::submit(const text_info * pTexts, std::size_t count) {
        auto res = dynamic_cast<text_layer_res_impl*> (_pResources.get());
        auto& vertices = res->vertices;

        for (auto it = pTexts; it != (pTexts + count); it++) {
            auto glyphs = res->font.encode(it->x, it->y, it->text);        
            auto r = static_cast<std::uint8_t> (it->color.red * 255.0F);
            auto g = static_cast<std::uint8_t> (it->color.green * 255.0F);
            auto b = static_cast<std::uint8_t> (it->color.blue * 255.0F);
            auto a = static_cast<std::uint8_t> (it->color.alpha * 255.0F);            

            for (auto&& glyph : glyphs) {            
                vertices.push_back({glyph.vertex.x0, glyph.vertex.y0, _tc(glyph.texCoord.s0), _tc(glyph.texCoord.t0), r, g, b, a});
                vertices.push_back({glyph.vertex.x1, glyph.vertex.y0, _tc(glyph.texCoord.s1), _tc(glyph.texCoord.t0), r, g, b, a});
                vertices.push_back({glyph.vertex.x0, glyph.vertex.y1, _tc(glyph.texCoord.s0), _tc(glyph.texCoord.t1), r, g, b, a});
                vertices.push_back({glyph.vertex.x1, glyph.vertex.y0, _tc(glyph.texCoord.s1), _tc(glyph.texCoord.t0), r, g, b, a});
                vertices.push_back({glyph.vertex.x0, glyph.vertex.y1, _tc(glyph.texCoord.s0), _tc(glyph.texCoord.t1), r, g, b, a});
                vertices.push_back({glyph.vertex.x1, glyph.vertex.y1, _tc(glyph.texCoord.s1), _tc(glyph.texCoord.t1), r, g, b, a});
            }
        }        
    }

    void text_layer::setScissor(const scissor_rect& scissor) {
        _scissor = scissor;
    }

    const scissor_rect& text_layer::getScissor() const {
        return _scissor;
    }

    void text_layer::setProjection(const float * projection) {
        auto res = dynamic_cast<text_layer_res_impl*> (_pResources.get());

        for (int i = 0; i < 16; i++) {
            res->projection[i] = projection[i];
        }
    }

    float text_layer::getAscent() const {
        auto res = dynamic_cast<text_layer_res_impl*> (_pResources.get());

        return res->font.getAscent();
    }

    float text_layer::getDescent() const {
        auto res = dynamic_cast<text_layer_res_impl*> (_pResources.get());

        return res->font.getDescent();
    }

    float text_layer::getLineGap() const {
        auto res = dynamic_cast<text_layer_res_impl*> (_pResources.get());

        return res->font.getLineGap();
    }
}
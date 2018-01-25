#include "engine/layers/text_layer.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <memory>
#include <string>
#include <vector>

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "math/mat4.hpp"

#include "util.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/bitmap_font.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/text_layer_info.hpp"

namespace engine {
    namespace layers {
        namespace {
            constexpr unsigned int BYTES_PER_VERTEX = 16;
            constexpr unsigned int VERTICES_PER_CHARACTER = 6;
            constexpr unsigned int BYTES_PER_CHARACTER = BYTES_PER_VERTEX * VERTICES_PER_CHARACTER;

            struct vertex {
                float x, y;
                std::uint16_t s, t;
                std::uint8_t r, g, b, a;
            };

            struct text_layer_resources : public base_resources {
                text_layer_info _info;

                graphics::buffer _vbo;
                graphics::vertex_array _vao;
                const bitmap_font * _pFont;
                float _projection[16];

                std::vector<vertex> _vertices;
                unsigned int _bufferSize;

                text_layer_resources(
                    const context& ctx,
                    const text_layer_info& info) noexcept;

                virtual ~text_layer_resources() {}
            };

            graphics::program _program;
            int _uFont;
            int _uProjection;

            const std::string BASE_SHADER_PATH = "data/shaders/text_layer/";
#if defined(GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "330_core.frag";
#elif defined(GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined(GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "100_es.frag";
#else
            const std::string VERTEX_SHADER_PATH = "";
            const std::string FRAGMENT_SHADER_PATH = "";
#error "No GL defined!"
#endif
        }

        text_layer::text_layer(
            const context& ctx,
            const text_layer_info& info) noexcept {

            _pResources = std::make_unique<text_layer_resources> (ctx, info);
        }

        void text_layer::beginWrite() noexcept {

        }

        void text_layer::endWrite() noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());

            res->_vbo.invalidate();
            res->_vbo.subData(0, res->_vertices.data(), res->_vertices.size() * BYTES_PER_VERTEX);
        }

        void text_layer::render() const noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());
            auto drawLimit = res->_vertices.size();

            if (drawLimit == 0) {
                return;
            }

            res->_vertices.clear();

            res->_vao.bind();

            _program.use();
            
            graphics::uniform::setUniformMatrix4(_uProjection, 1, res->_projection);
            graphics::uniform::setUniform1(_uFont, 0);

            auto pTexture = reinterpret_cast<const graphics::texture * > (res->_pFont->getTexture());

            pTexture->bind(0);

            graphics::draw::arrays(graphics::draw_mode::TRIANGLES, 0, drawLimit);
        }

        void text_layer::invalidate() noexcept {

        }

        void text_layer::setProjection(const math::mat4& projection) noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());

            projection.data(res->_projection);
        }

        void text_layer::setProjection(const float * projection) noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());

            std::memcpy(res->_projection, projection, sizeof(float) * 16);
        }

        void text_layer::submit(
            const text_info * pTexts,
            std::size_t count) noexcept {

            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());
            auto& vertices = res->_vertices;
            auto& font = res->_pFont->getImage();

            for (auto it = pTexts; it != (pTexts + count); it++) {
                auto glyphs = font.encode(it->x, it->y, it->text);
                auto r = it->color.r;
                auto g = it->color.g;
                auto b = it->color.b;
                auto a = it->color.a;

                for (auto&& glyph : glyphs) {
                    auto s0 = util::unorm<std::uint16_t> (glyph.texCoord.s0);
                    auto t0 = util::unorm<std::uint16_t> (glyph.texCoord.t0);
                    auto s1 = util::unorm<std::uint16_t> (glyph.texCoord.s1);
                    auto t1 = util::unorm<std::uint16_t> (glyph.texCoord.t1);
                    auto x0 = glyph.vertex.x0;
                    auto y0 = glyph.vertex.y0;
                    auto x1 = glyph.vertex.x1;
                    auto y1 = glyph.vertex.y1;

                    vertices.push_back({x0, y0, s0, t0, r, g, b, a});
                    vertices.push_back({x1, y0, s1, t0, r, g, b, a});
                    vertices.push_back({x0, y1, s0, t1, r, g, b, a});
                    vertices.push_back({x1, y0, s1, t0, r, g, b, a});
                    vertices.push_back({x0, y1, s0, t1, r, g, b, a});
                    vertices.push_back({x1, y1, s1, t1, r, g, b, a});
                }
            }
        }

        float text_layer::getAscent() const noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());

            return res->_pFont->getImage().getAscent();
        }

        float text_layer::getDescent() const noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());

            return res->_pFont->getImage().getDescent();
        }

        float text_layer::getLineGap() const noexcept {
            auto res = dynamic_cast<text_layer_resources * > (_pResources.get());

            return res->_pFont->getImage().getLineGap();
        }

        namespace {
            text_layer_resources::text_layer_resources(
                const context& ctx,
                const text_layer_info& info) noexcept {

                _info = info;
                _bufferSize = info.maxCharacters * BYTES_PER_CHARACTER;
                _vbo = graphics::buffer({
                        graphics::buffer_target::ARRAY,
                        graphics::buffer_usage::STREAM_DRAW,
                        {nullptr, _bufferSize}});                            
                
                graphics::vertex_attribute_description attribs[] = {
                    {0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0},
                    {1, graphics::vertex_format::X16Y16_UNORM, 8, 0},
                    {2, graphics::vertex_format::X8Y8Z8W8_UNORM, 12, 0}};

                graphics::vertex_binding_description bindings[] = {
                    {0, 16, 0, &_vbo}};

                _vao = graphics::vertex_array({attribs, 3, bindings, 1, nullptr});
                _pFont = ctx.getFont(info.fontID);

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    graphics::attribute_state_info attribs[] = {
                        {"vPosition", 0},
                        {"vTexCoord", 1},
                        {"vColor", 2}};

                    _program = graphics::program({shaders, 2, attribs, 3});
                    _uFont = _program.getUniformLocation("uFont");
                    _uProjection = _program.getUniformLocation("uProjection");
                }
            }
        }
    }
}
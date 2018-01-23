#include "engine/layers/basic_image_layer.hpp"

#include <cstdio>
#include <cstring>

#include <memory>
#include <string>
#include <utility>

#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/basic_image_layer_info.hpp"
#include "engine/layers/context.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            graphics::address_mode _scrollType(image_scroll_type type) noexcept;

            struct basic_image_layer_resources : public base_resources {
                basic_image_layer_info _info;

                float _scroll[4];
                float _projection[16];

                graphics::texture _texture;
                graphics::vertex_array _vao;

                basic_image_layer_resources(
                    const context& ctx,
                    const basic_image_layer_info& info) noexcept;

                virtual ~basic_image_layer_resources() {}
            };

            graphics::program _program;
            int _uImage;
            int _uImageView;
            int _uProjection;

            const std::string BASE_SHADER_PATH = "data/shaders/basic_image_renderer/";
#if defined(GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH  + "330_core.frag";
#elif defined (GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined (GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "100_es.frag";
#else
#error "No GL defined!"
            const std::string VERTEX_SHADER_PATH = "";
            const std::string FRAGMENT_SHADER_PATH = "";
#endif
        }

        basic_image_layer::basic_image_layer(
            const context& ctx,
            const basic_image_layer_info& info) noexcept {

            _pResources = std::make_unique<basic_image_layer_resources> (ctx, info);
        }

        void basic_image_layer::beginWrite() noexcept {

        }

        void basic_image_layer::endWrite() noexcept {

        }

        void basic_image_layer::render() const noexcept {
            auto res = dynamic_cast<const basic_image_layer_resources * > (_pResources.get());

            _program.use();            

            graphics::uniform::setUniform1(_uImage, 0);
            graphics::uniform::setUniform4(_uImageView, 1, res->_scroll);
            graphics::uniform::setUniformMatrix4(_uProjection, 1, res->_projection);

            res->_texture.bind(0);
            res->_vao.bind();

            graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
        }

        void basic_image_layer::invalidate() noexcept {

        }

        void basic_image_layer::setProjection(const math::mat4& projection) noexcept {
            auto res = dynamic_cast<basic_image_layer_resources * > (_pResources.get());

            projection.data(res->_projection);
        }

        void basic_image_layer::setProjection(const float * projection) noexcept {
            auto res = dynamic_cast<basic_image_layer_resources * > (_pResources.get());

            std::memcpy(res->_projection, projection, 16 * sizeof(float));
        }

        void basic_image_layer::scroll(float s0, float t0, float s1, float t1) noexcept {
            auto res = dynamic_cast<basic_image_layer_resources * > (_pResources.get());

            res->_scroll[0] = s0;
            res->_scroll[1] = t0;
            res->_scroll[2] = s1;
            res->_scroll[3] = t1;
        }

        const basic_image_layer_info& basic_image_layer::getInfo() const noexcept {
            auto res = dynamic_cast<basic_image_layer_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] basic_image_layer error: %s\n\0", msg.c_str());
                __builtin_trap();
            }

            graphics::address_mode _scrollType(image_scroll_type type) noexcept {
                switch (type) {
                    case image_scroll_type::REPEAT:
                        return graphics::address_mode::REPEAT;
                    case image_scroll_type::STATIC:
                        return graphics::address_mode::CLAMP_TO_EDGE;
                    default:
                        _onError("Unsupported image_scroll_type!");                        
                }
            }

            basic_image_layer_resources::basic_image_layer_resources(
                const context & ctx,
                const basic_image_layer_info& info) noexcept {

                _info = info;
                
                float defaultScroll[4] = {0.0F, 0.0F, 1.0F, 1.0F};

                std::memcpy(_scroll, defaultScroll, 4 * sizeof(float));

                float defaultProjection[16] = {
                    2.0F, 0.0F, 0.0F, 0.0F,
                    0.0F, -2.0F, 0.0F, 0.0F,
                    0.0F, 0.0F, 1.0F, 0.0F,
                    -1.0F, 1.0F, 0.0F, 1.0F};

                std::memcpy(_projection, defaultProjection, 16 * sizeof(float));

                auto newTex = graphics::texture({
                    {info.pImage->getWidth(), info.pImage->getHeight(), 1},
                    1, 1,
                    {
                        {graphics::mag_filter::LINEAR, graphics::min_filter::LINEAR},
                        {_scrollType(info.scroll.horizontal), _scrollType(info.scroll.vertical), graphics::address_mode::CLAMP_TO_EDGE},
                        {-1000.0F, 1000.0F}
                    },
                    graphics::internal_format::RGBA8});

                std::swap(_texture, newTex);                

                _texture.subImage(0, 0, 0, 0, info.pImage->getWidth(), info.pImage->getHeight(), 1, {
                    graphics::pixel_type::UNSIGNED_BYTE,
                    graphics::pixel_format::RGBA,
                    const_cast<void *> (info.pImage->getData())});

                auto newVao = graphics::vertex_array({nullptr});
                
                std::swap(_vao, newVao);

                if (!_program) {                    
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    auto newProgram = graphics::program({shaders, 2, nullptr, 0});

                    std::swap(_program, newProgram);

                    _uImage = _program.getUniformLocation("uImage");
                    _uImageView = _program.getUniformLocation("uImageView");
                    _uProjection = _program.getUniformLocation("uProjection");
                }
            }
        }
    }
}
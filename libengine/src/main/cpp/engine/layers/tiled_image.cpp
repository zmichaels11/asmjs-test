#include "engine/layers/tiled_image.hpp"

#include <cstdio>

#include <memory>
#include <string>
#include <utility>

#include "graphics/buffer.hpp"
#include "graphics/framebuffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/tiled_image_info.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct tiled_image_resources : public base_resources {
                const context * _pctx;
                tiled_image_info _info;
                bool _dirty;
                bool _redraw;

                struct vbo_t {
                    graphics::buffer position;
                    graphics::buffer offset;
                    graphics::buffer imageView;
                } _vbos;

                graphics::framebuffer _fb;
                graphics::texture _texture;
                graphics::vertex_array _vao;                

                tiled_image_resources(
                    const context * pctx,
                    const tiled_image_info& info) noexcept;

                virtual ~tiled_image_resources() {}
            };

            graphics::program _program;
            int _uImage;

            const std::string BASE_SHADER_PATH = "data/shaders/tiled_image_renderer/";
#if defined (GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "330_core.frag";
#elif defined (GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined (GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "100_es.frag";
#else
            const std::string VERTEX_SHADER_PATH = "";
            const std::string FRAGMENT_SHADER_PATH = "";
#error "No GL defined!"
#endif
        }

        tiled_image::tiled_image(
            const context * pctx,
            const tiled_image_info& info) noexcept {

            _pResources = std::make_unique<tiled_image_resources> (pctx, info);
        }

        void tiled_image::beginWrite() noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            // always clera the redraw flag. It gets set only in endWrite.
            res->_redraw = false;
        }

        void tiled_image::endWrite() noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            if (!res->_dirty) {
                return;
            }

            res->_dirty = false;
        }

        void tiled_image::invalidate() noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());
            
            res->_dirty = true;
        }

        void tiled_image::render() const noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            if (!res->_redraw) {
                return;
            }

            _program.use();
        }

        const void * tiled_image::getTexture() const noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            if (static_cast<int> (res->_texture)) {
                return reinterpret_cast<const void * > (&res->_texture);
            } else {
                return nullptr;
            }
        }

        void tiled_image::setTile(int col, int row, const image_view& view) noexcept {

        }

        void tiled_image::setOffset(int rowOrCol, float offset) noexcept {

        }

        const tiled_image_info& tiled_image::getInfo() const noexcept {
            auto res = dynamic_cast<const tiled_image_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] tiled_image error: %s\n", msg.c_str());
                __builtin_trap();
            }

            tiled_image_resources::tiled_image_resources(
                    const context * pctx,
                    const tiled_image_info& info) noexcept {

                _pctx = pctx;
                _info = info;

                auto textureWidth = info.dim.columns * info.tileSize.width;
                auto textureHeight = info.dim.rows * info.tileSize.width;

                auto textureInfo = graphics::texture_info();

                textureInfo.extent = {textureWidth, textureHeight, 1};
                textureInfo.layers = 1;
                textureInfo.levels = 1;
                textureInfo.samplerInfo = graphics::sampler_info::defaults();
                textureInfo.format = graphics::internal_format::RGBA8;

                auto newTexture = graphics::texture(textureInfo);

                std::swap(_texture, newTexture);

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeVertex(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    graphics::attribute_state_info attribs[] = {
                        {"vPosition", 0},
                        {"vOffset", 1},
                        {"vImageView", 2}};

                    auto newProgram = graphics::program({shaders, 2, attribs, 3});

                    std::swap(_program, newProgram);

                    _uImage = _program.getUniformLocation("uImage");
                }
            }
        }
    }
}
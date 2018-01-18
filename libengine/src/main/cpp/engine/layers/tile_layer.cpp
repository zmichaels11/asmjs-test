#include "engine/layers/tile_layer.hpp"

#include <cstdio>
#include <cstring>

#include <memory>
#include <string>

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/texture.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/sprite_sheet.hpp"
#include "engine/layers/tile_layer_info.hpp"
#include "engine/layers/tile_slot.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct tile_layer_resources : public engine::layers::base_resources {
                const sprite_sheet * _pSpriteSheet;
                tile_layer_info _info;
                std::unique_ptr<tile_slot[]> _tileSlots;
                tile_slot * _tileSlotAccessor;

                float _projection[16];

                bool _cacheValid;
                graphics::buffer _vertices;
                graphics::buffer _tiles;
                graphics::vertex_array _vao;

                tile_layer_resources(
                    const context& ctx,
                    const tile_layer_info& info) noexcept;

                virtual ~tile_layer_resources() {}
            };

            const std::string BASE_SHADER_PATH = "data/shaders/tile_renderer/";
#if defined(GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "330_core.frag";
#elif defined(GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined(GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_pATH + "100_es.frag";
#else
            const std::string VERTEX_SHADER_PATH = "";
            const std::string FRAGMENT_SHADER_PATH = "";
#error "No GL defined!"
#endif

            graphics::program _program;
            int _uProjection;
            int _uImage;
            int _uTileSize;
        }

        tile_layer::tile_layer(
            const context& ctx,
            const tile_layer_info& info) noexcept {

            _pResources = std::make_unique<tile_layer_resources> (ctx, info);
        }

        const tile_layer_info& tile_layer::getInfo() const noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());

            return res->_info;
        }

        void tile_layer::beginWrite() noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());

            res->_tileSlotAccessor = res->_tileSlots.get();
        }

        void tile_layer::endWrite() noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());
            auto tileDataSize = res->_info.rows * res->_info.columns * sizeof(tile_slot);

            res->_tiles.invalidate();
            res->_tiles.subData(0, res->_tileSlots.get(), tileDataSize);
        }

        void tile_layer::invalidate() noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());

            res->_cacheValid = false;
        }

        void tile_layer::render() const noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());          
            
            if (res->_cacheValid) {
                //TODO: optionally use cached version
            } else {
                _program.use();
                graphics::uniform::setUniform1(_uImage, 0);
                graphics::uniform::setUniform1(_uTileSize, res->_info.tileSize);
                graphics::uniform::setUniformMatrix4(_uProjection, 1, res->_projection);

                auto pTexture = reinterpret_cast<const graphics::texture * > (res->_pSpriteSheet->getTexture());

                pTexture->bind(0);

                res->_vao.bind();

                auto tileCount = res->_info.columns * res->_info.rows;

                graphics::draw::arraysInstanced(graphics::draw_mode::TRIANGLE_STRIP, 0, 4, tileCount);
            }            
        }

        void tile_layer::setProjection(const math::mat4& projection) noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());

            projection.data(res->_projection);
        }

        void tile_layer::setProjection(const float * projection) noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());

            std::memcpy(res->_projection, projection, sizeof(float) * 16);
        }

        engine::layers::tile_slot ** tile_layer::fetchTileSlots() const noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());

            return &res->_tileSlotAccessor;
        }

        const engine::layers::image_view& tile_layer::getImageView(int tileID) const noexcept {
            auto res = dynamic_cast<tile_layer_resources * > (_pResources.get());
            
            return res->_pSpriteSheet->getSprite(tileID);
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] tile_layer error: %s\n", msg.c_str());
                __builtin_trap();
            }

            tile_layer_resources::tile_layer_resources(
                const context& ctx,                
                const tile_layer_info& info) noexcept {

                _info = info;
                _pSpriteSheet = ctx.getSpriteSheet(info.tileSheetID);
                
                auto tileCount = info.rows * info.columns;
                _tileSlots = std::make_unique<tile_slot[]> (tileCount);

                float vertexData[] = {
                    0.0F, 0.0F,
                    0.0F, 1.0F,
                    1.0F, 0.0F,
                    1.0F, 1.0F};

                _vertices = graphics::buffer(graphics::buffer_info{
                    graphics::buffer_target::ARRAY,
                    graphics::buffer_usage::STATIC_DRAW,
                    {vertexData, sizeof(float) * 8}});

                _tiles = graphics::buffer(graphics::buffer_info{
                    graphics::buffer_target::ARRAY,
                    graphics::buffer_usage::STREAM_DRAW,
                    {nullptr, tileCount * sizeof(tile_slot)}});

                auto verticesBinding = graphics::vertex_binding_description{0, sizeof(float) * 8, 0, &_vertices, 0};
                auto tileBinding = graphics::vertex_binding_description{1, sizeof(tile_slot), 1, &_tiles, 0};

                auto aPosition = graphics::vertex_attribute_description{0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0};
                auto aFrameIndex = graphics::vertex_attribute_description{1, graphics::vertex_format::X32_SFLOAT, 0, 1};
                auto aFrameSize = graphics::vertex_attribute_description{2, graphics::vertex_format::X16Y16_UNORM, 4, 1};

                decltype(&verticesBinding) pBindings[] = {&verticesBinding, &tileBinding};
                decltype(&aPosition) pAttributes[] = {&aPosition, &aFrameIndex, &aFrameSize};

                _vao = graphics::vertex_array(graphics::vertex_array_info{
                    pAttributes, 3,
                    pBindings, 2,
                    nullptr});

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    graphics::attribute_state_info attributes[] = {
                        {"vPosition", 0},
                        {"vFrameIndex", 1},
                        {"vFrameSize", 2}};

                    _program = graphics::program(graphics::program_info{
                        shaders, 2,
                        attributes, 3});

                    _uProjection = _program.getUniformLocation("uProjection");
                    _uImage = _program.getUniformLocation("uImage");
                    _uTileSize = _program.getUniformLocation("uTileSize");
                }
            }
        }
    }
}
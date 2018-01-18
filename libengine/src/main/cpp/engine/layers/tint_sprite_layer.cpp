#include "engine/layers/tint_sprite_layer.hpp"

#include <cstring>

#include <memory>
#include <string>

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/tint_sprite_layer_info.hpp"
#include "engine/layers/tint_sprite_slot.hpp"
#include "engine/layers/sprite_sheet.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct tint_sprite_layer_resources : public base_resources {
                const sprite_sheet * _pSpriteSheet;
                tint_sprite_layer_info _info;
                std::unique_ptr<tint_sprite_slot[]> _spriteSlots;
                tint_sprite_slot * _spriteSlotAccessor;

                float _projection[16];

                graphics::buffer _vbo;
                graphics::vertex_array _vao;

                tint_sprite_layer_resources(
                    const context& ctx,
                    const tint_sprite_layer_info& info) noexcept;

                virtual ~tint_sprite_layer_resources() {}
            };

            const std::string BASE_SHADER_PATH = "data/shaders/tint_sprite_renderer/";
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
        }

        tint_sprite_layer::tint_sprite_layer(
            const context& ctx,
            const tint_sprite_layer_info& info) noexcept {

            _pResources = std::make_unique<tint_sprite_layer_resources> (ctx, info);
        }

        void tint_sprite_layer::invalidate() noexcept {
            
        }

        void tint_sprite_layer::beginWrite() noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());
            
            //NOTE: this could be the result of a buffer map
            res->_spriteSlotAccessor = res->_spriteSlots.get();
        }

        void tint_sprite_layer::endWrite() noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());
            auto vertexDataSize = res->_info.maxSprites * sizeof(tint_sprite_slot);

            res->_vbo.invalidate();
            res->_vbo.subData(0, res->_spriteSlots.get(), vertexDataSize);
        }

        void tint_sprite_layer::render() const noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());

            _program.use();
            graphics::uniform::setUniform1(_uImage, 0);
            graphics::uniform::setUniformMatrix4(_uProjection, 1, res->_projection);

            auto pTexture = reinterpret_cast<const graphics::texture * > (res->_pSpriteSheet->getTexture());

            pTexture->bind(0);

            res->_vao.bind();

            graphics::draw::arraysInstanced(graphics::draw_mode::TRIANGLE_STRIP, 0, 4, res->_info.maxSprites);
        }

        void tint_sprite_layer::setProjection(const math::mat4& projection) noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());

            projection.data(res->_projection);
        }

        void tint_sprite_layer::setProjection(const float * projection) noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());

            std::memcpy(res->_projection, projection, sizeof(float) * 16);
        }

        tint_sprite_slot ** tint_sprite_layer::fetchSpriteSlots() const noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());

            return &res->_spriteSlotAccessor;
        }

        const image_view& tint_sprite_layer::getImageView(int spriteID) const noexcept {
            auto res = dynamic_cast<tint_sprite_layer_resources * > (_pResources.get());

            return res->_pSpriteSheet->getSprite(spriteID);
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] tint_sprite_layer error: %s\n", msg.c_str());
                __builtin_trap();
            }

            tint_sprite_layer_resources::tint_sprite_layer_resources(
                const context& ctx,
                const tint_sprite_layer_info& info) noexcept {

                _info = info;
                _pSpriteSheet = ctx.getSpriteSheet(info.spriteID);
                _spriteSlots = std::make_unique<tint_sprite_slot[]> (info.maxSprites);

                graphics::buffer_usage usage;

                switch (info.writeHint) {
                    case engine::layers::write_hint::ONCE:
                        usage = graphics::buffer_usage::STATIC_DRAW;
                        break;
                    case engine::layers::write_hint::SOMETIMES:
                        usage = graphics::buffer_usage::DYNAMIC_DRAW;
                        break;
                    case engine::layers::write_hint::OFTEN:
                        usage = graphics::buffer_usage::STREAM_DRAW;
                        break;
                    default:
                        _onError("Invalid write hint!");
                        break;
                }

                auto vertexDataSize = info.maxSprites * sizeof(tint_sprite_slot);

                _vbo = graphics::buffer(graphics::buffer_info{
                    graphics::buffer_target::ARRAY,
                    usage,
                    {nullptr, vertexDataSize}});

                auto vboBinding = graphics::vertex_binding_description{0, sizeof(tint_sprite_slot), 1, &_vbo, 0};
                
                auto aUpperLeft = graphics::vertex_attribute_description{0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0};
                auto aUpperRight = graphics::vertex_attribute_description{1, graphics::vertex_format::X32Y32_SFLOAT, 8, 0};
                auto aLowerLeft = graphics::vertex_attribute_description{2, graphics::vertex_format::X32Y32_SFLOAT, 16, 0};
                auto aFrameIndex = graphics::vertex_attribute_description{3, graphics::vertex_format::X32_SFLOAT, 24, 0};
                auto aFrameSize = graphics::vertex_attribute_description{4, graphics::vertex_format::X16Y16_UNORM, 28, 0};
                auto aTint = graphics::vertex_attribute_description{5, graphics::vertex_format::X32Y32Z32W32_SFLOAT, 32, 0};

                decltype(&vboBinding) pBindings[] = {&vboBinding};
                decltype(&aUpperLeft) pAttributes[] = {&aUpperLeft, &aUpperRight, &aLowerLeft, &aFrameIndex, &aFrameSize, &aTint};

                _vao = graphics::vertex_array(graphics::vertex_array_info{
                    pAttributes, 6,
                    pBindings, 1,
                    nullptr});

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    graphics::attribute_state_info attributes[] = {
                        {"vUpperLeft", 0},
                        {"vUpperRight", 1},
                        {"vLowerLeft", 2},
                        {"vFrameIndex", 3},
                        {"vFrameSize", 4},
                        {"vTint", 5}};

                    _program = graphics::program(graphics::program_info{
                        shaders, 2,
                        attributes, 6});

                    _uProjection = _program.getUniformLocation("uProjection");
                    _uImage = _program.getUniformLocation("uImage");
                }
            }
        }
    }
}
#include "engine/layers/basic_sprite_layer.hpp"

#include <cstring>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "math/mat4.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/basic_sprite_layer_info.hpp"
#include "engine/layers/basic_sprite_slot.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/sprite_sheet.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct basic_sprite_layer_resources : public engine::layers::base_resources {
                const sprite_sheet * _pSpriteSheet;
                basic_sprite_layer_info _info;                
                basic_sprite_slot * _spriteSlotAccessor;

#if defined(__EMSCRIPTEN__)
                std::unique_ptr<basic_sprite_slot[]> _spriteSlots;
#endif

                unsigned int _vboSize;

                float _projection[16];

                graphics::buffer _select;
                graphics::buffer _vbo;
                graphics::vertex_array _vao;

                basic_sprite_layer_resources(
                    const context& ctx, 
                    const basic_sprite_layer_info& info) noexcept;

                virtual ~basic_sprite_layer_resources() {}
            };

            const std::string BASE_SHADER_PATH = "data/shaders/basic_sprite_renderer/";
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

        basic_sprite_layer::basic_sprite_layer(
            const context& ctx, 
            const basic_sprite_layer_info& info) noexcept {

            _pResources = std::make_unique<basic_sprite_layer_resources>(ctx, info);
        }

        void basic_sprite_layer::invalidate() noexcept {
            
        }

        const basic_sprite_layer_info& basic_sprite_layer::getInfo() const noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());

            return res->_info;
        }

        void basic_sprite_layer::beginWrite() noexcept {
            constexpr static auto MAP_ACCESS = graphics::buffer_access::WRITE | graphics::buffer_access::INVALIDATE_BUFFER;
            
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());                        
            
#if defined(__EMSCRIPTEN__)
            res->_spriteSlotAccessor = res->_spriteSlots.get();
#else
            auto mappedData = res->_vbo.map(0, res->_vboSize, MAP_ACCESS);            
            
            res->_spriteSlotAccessor = reinterpret_cast<basic_sprite_slot * > (mappedData);
#endif
        }

        void basic_sprite_layer::endWrite() noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());            

            res->_spriteSlotAccessor = nullptr;

#if defined(__EMSCRIPTEN__)
            res->_vbo.invalidate();
            res->_vbo.subData(0, res->_spriteSlots.get(), res->_vboSize);
#else
            res->_vbo.unmap();
#endif
        }

        void basic_sprite_layer::render() const noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());            

            _program.use();
            graphics::uniform::setUniform1(_uImage, 0);
            graphics::uniform::setUniformMatrix4(_uProjection, 1, res->_projection);

            auto pTexture = reinterpret_cast<const graphics::texture *> (res->_pSpriteSheet->getTexture());
            
            pTexture->bind(0);

            res->_vao.bind();            

            graphics::draw::arraysInstanced(graphics::draw_mode::TRIANGLE_STRIP, 0, 4, res->_info.maxSprites);
        }

        void basic_sprite_layer::setProjection(const math::mat4& projection) noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());

            projection.data(res->_projection);
        }

        void basic_sprite_layer::setProjection(const float * projection) noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());

            std::memcpy(res->_projection, projection, sizeof(float) * 16);
        }

        engine::layers::basic_sprite_slot ** basic_sprite_layer::fetchSpriteSlots() const noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());

            return &res->_spriteSlotAccessor;
        }

        const engine::layers::image_view& basic_sprite_layer::getImageView(int spriteID) const noexcept {
            auto res = dynamic_cast<basic_sprite_layer_resources * > (_pResources.get());

            return res->_pSpriteSheet->getSprite(spriteID);
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] basic_sprite_layer error: %s\n", msg.c_str());
                __builtin_trap();
            }

            basic_sprite_layer_resources::basic_sprite_layer_resources(
                const context& ctx,
                const basic_sprite_layer_info& info) noexcept {

                _info = info;
                _pSpriteSheet = ctx.getSpriteSheet(info.spriteSheetID);
                _vboSize = info.maxSprites * sizeof(basic_sprite_slot);

#if defined(__EMSCRIPTEN__)
                _spriteSlots = std::make_unique<basic_sprite_slot[]> (info.maxSprites);
#endif

                {
                    float select[] = {
                        0.0F, 0.0F,
                        0.0F, 1.0F,
                        1.0F, 0.0F,
                        1.0F, 1.0F};

                    auto newSelect = graphics::buffer({
                        graphics::buffer_target::ARRAY,
                        graphics::buffer_usage::STATIC_DRAW,
                        {select, sizeof(float) * 8}});

                    std::swap(_select, newSelect);
                }

                {
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

                    auto newVbo = graphics::buffer(graphics::buffer_info{
                        graphics::buffer_target::ARRAY,
                        usage,
                        {nullptr, _vboSize}});

                    std::swap(_vbo, newVbo);
                }                                

                {
                    auto attributes = std::vector<graphics::vertex_attribute_description>();

                    attributes.push_back({0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0});
                    attributes.push_back({1, graphics::vertex_format::X32Y32_SFLOAT, 0, 1});
                    attributes.push_back({2, graphics::vertex_format::X32Y32_SFLOAT, 8, 1});
                    attributes.push_back({3, graphics::vertex_format::X32Y32_SFLOAT, 16, 1});
                    attributes.push_back({4, graphics::vertex_format::X32_SFLOAT, 24, 1});
                    attributes.push_back({5, graphics::vertex_format::X16Y16_UNORM, 28, 1});

                    auto bindings = std::vector<graphics::vertex_binding_description>();

                    bindings.push_back({0, 0, 0, &_select, 0});
                    bindings.push_back({1, sizeof(basic_sprite_slot), 1, &_vbo, 0});

                    auto newVao = graphics::vertex_array(graphics::vertex_array_info{
                        attributes.data(), attributes.size(),
                        bindings.data(), bindings.size(),
                        nullptr});

                    std::swap(_vao, newVao);
                }

                if (!_program) {                    
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    auto attributes = std::vector<graphics::attribute_state_info>();

                    attributes.push_back({"vSelect", 0});
                    attributes.push_back({"vUpperLeft", 1});
                    attributes.push_back({"vUpperRight", 2});
                    attributes.push_back({"vLowerLeft", 3});
                    attributes.push_back({"vFrameIndex", 4});
                    attributes.push_back({"vFrameSize", 5});

                    auto newProgram = graphics::program(graphics::program_info{
                        shaders, 2,
                        attributes.data(), attributes.size()});

                    std::swap(_program, newProgram);

                    _uProjection = _program.getUniformLocation("uProjection");
                    _uImage = _program.getUniformLocation("uImage");
                }
            }
        }
    }
}
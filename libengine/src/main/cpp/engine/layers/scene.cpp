#include "engine/layers/scene.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "graphics/framebuffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/renderbuffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/state.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "util.hpp"

#include "engine/application.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct scene_resources : public base_resources {                
                scene_info _info;
                context _context;
                std::vector<scene_layer> _layers;

                graphics::viewport_state_info _viewport;
                graphics::framebuffer _fb;
                graphics::renderbuffer _depthStencil;
                graphics::texture _color;                
                graphics::vertex_array _vao;

                scene_resources(const scene_info& info) noexcept;

                virtual ~scene_resources() {}
            };

            graphics::program _program;
            int _uImage;

            const std::string BASE_SHADER_PATH = "data/shaders/fullscreen_quad/";
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

        context& scene::getContext() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_context;
        }

        const context& scene::getContext() const noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_context;
        }

        scene::scene(const scene_info& info) noexcept {
            _pResources = std::make_unique<scene_resources> (info);
        }

        void scene::beginWrite() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            auto& layers = res->_layers;
            
            res->_context.beginWrite();

            for (auto&& layer : layers) {
                layer.layer->beginWrite();
            }
        }

        void scene::endWrite() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            auto& layers = res->_layers;

            res->_context.endWrite();

            for (auto&& layer : layers) {
                layer.layer->endWrite();
            }
        }

        void scene::invalidate() noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());
            auto& layers = res->_layers;

            for (auto&& layer : layers) {
                layer.layer->invalidate();
            }
        }

        void scene::render() const noexcept {
            auto res = dynamic_cast<const scene_resources * > (_pResources.get());
            auto& layers = res->_layers;

            // render any context objects; these *may* write to other render targets
            res->_context.render();            
            
            // render to the indirect surface
            res->_fb.bind();

            graphics::apply(res->_viewport);

            graphics::draw_buffer drawBuffers[] = {graphics::draw_buffer::COLOR_ATTACHMENT0};

            graphics::framebuffer::drawBuffers(drawBuffers, 1);

            for (auto&& layer : layers) {
                layer.layer->render();
            }

            graphics::framebuffer::getDefault().bind();

            auto pDefaultViewport = reinterpret_cast<const graphics::viewport_state_info * > (application::getViewport());

            graphics::apply(*pDefaultViewport);

            _program.use();

            graphics::uniform::setUniform1(_uImage, 0);

            res->_color.bind(0);

            res->_vao.bind();

            graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
        }

        base_layer * scene::getLayer(int layerId) noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_layers[layerId].layer.get();
        }

        const base_layer * scene::getLayer(int layerId) const noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_layers[layerId].layer.get();
        }

        const scene_info& scene::getInfo() const noexcept {
            auto res = dynamic_cast<scene_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::printf("[render_engine] scene error: %s\n", msg.c_str());
                __builtin_trap();
            }
            
            scene_resources::scene_resources(const scene_info& info) noexcept {
                _info = info;
                _context = context(info.contextInfo);

                for (auto it = info.pLayerInfos; it != (info.pLayerInfos + info.nLayerInfos); it++) {
                    switch (it->type) {
                        case layer_type::BACKGROUND_LAYER: {
                            _layers.push_back({
                                *it,
                                std::make_unique<background_layer> (_context, it->info.backgroundLayer)});                            
                        } break;
                        case layer_type::BASIC_IMAGE_LAYER: {                            
                            _layers.push_back({
                                *it, 
                                std::make_unique<basic_image_layer> (_context, it->info.basicImageLayer)});
                        } break;
                        default: 
                            _onError("Invalid layer_type!");                        
                    }
                }

                _viewport = graphics::viewport_state_info{0, 0, static_cast<int> (info.surface.width), static_cast<int> (info.surface.height)};                

                auto newColor = graphics::texture({
                    {info.surface.width, info.surface.height, 1},
                    1, 1,
                    {
                        {graphics::mag_filter::NEAREST, graphics::min_filter::LINEAR},
                        {graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE},
                        {-1000.0F, 10000.F}
                    },
                    graphics::internal_format::RGB8});

                std::swap(_color, newColor);

                auto newDepthStencil = graphics::renderbuffer({
                    graphics::internal_format::DEPTH24_STENCIL8,
                    info.surface.width, info.surface.height});

                std::swap(_depthStencil, newDepthStencil);

                graphics::attachment_info attachments[] = {
                    {0, nullptr, &_color},
                    {1, &_depthStencil, nullptr}};

                auto newFb = graphics::framebuffer({attachments, 1});

                std::swap(_fb, newFb);

                auto newVao = graphics::vertex_array({nullptr});

                std::swap(_vao, newVao);

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    auto newProgram = graphics::program({shaders, 2});

                    std::swap(_program, newProgram);

                    _uImage = _program.getUniformLocation("uImage");
                }
            }
        }
    }
}
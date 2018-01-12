#include "engine/gfx/quad_renderer.hpp"

#include <memory>
#include <string>

#include "engine/gfx/base_resources.hpp"
#include "engine/gfx/quad_renderer_info.hpp"

#include "graphics/buffer.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

namespace engine {
    namespace gfx {
        namespace {
            struct quad_renderer_resources : public virtual base_resources {
                quad_renderer_info _info;

                quad_renderer_resources(const quad_renderer_info& info) noexcept;

                virtual ~quad_renderer_resources();
            };

            graphics::program program;
            int uProjection;
            int uImage;

            void _checkProgram() noexcept;

            const std::string BASE_SHADER_PATH = "data/shaders/quad_renderer/";
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

        quad_renderer::quad_renderer(const quad_renderer_info& info) noexcept {
            _pResources = std::make_unique<quad_renderer_resources>(info);

            _checkProgram();
        }

        namespace {
            quad_renderer_resources::quad_renderer_resources(const quad_renderer_info& info) noexcept {
                _info = info;
            }

            quad_renderer_resources::~quad_renderer_resources() {

            }

            void _checkProgram() noexcept {
                if (program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    graphics::attribute_state_info attribs[] = {
                        {"vPosition", 0},
                        {"vTexCoord", 1}
                    };

                    decltype(&vsh) shaders[] = {&vsh, &fsh};

                    program = graphics::program(graphics::program_info{shaders, 2, attribs, 2});

                    uProjection = program.getUniformLocation("uProjection");
                    uImage = program.getUniformLocation("uImage");
                }
            }
        }
    }
}
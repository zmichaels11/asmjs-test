#include "engine/gfx/fullscreen_quad_renderer.hpp"

#include <cstring>

#include <memory>
#include <string>

#include "engine/gfx/base_resources.hpp"

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

namespace engine {
    namespace gfx {
        namespace {
            struct fullscreen_quad_resources : public virtual base_resources {
                graphics::buffer index;
                graphics::vertex_array model;

                float imageView[4];                

                fullscreen_quad_resources();

                virtual ~fullscreen_quad_resources() {}
            };
            
            graphics::program program;
            int uImage;
            int uImageView;

            const std::string BASE_SHADER_PATH = "data/shaders/fullscreen_quad/";

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

        fullscreen_quad_resources::fullscreen_quad_resources() {
            float indexData[] = {
                0.0F, 0.0F, 
                0.0F, 1.0F, 
                1.0F, 0.0F, 
                1.0F, 1.0F};

            index = graphics::buffer(graphics::buffer_info{graphics::buffer_target::ARRAY, graphics::buffer_usage::STATIC_DRAW, {indexData, sizeof(indexData)}});

            auto indexBinding = graphics::vertex_binding_description{0, 0, 0, &index, 0};
            auto indexAttrib = graphics::vertex_attribute_description{0, graphics::vertex_format::X32Y32_SFLOAT, 0};
            
            decltype(&indexAttrib) attribs[] = {&indexAttrib};
            decltype(&indexBinding) bindings[] = {&indexBinding};

            model = graphics::vertex_array(graphics::vertex_array_info{attribs, 1, bindings, 1});
        }

        fullscreen_quad_renderer::fullscreen_quad_renderer() noexcept {
            _pResources = std::make_unique<fullscreen_quad_resources>();
        }

        fullscreen_quad_renderer::~fullscreen_quad_renderer() {}

        void fullscreen_quad_renderer::reset() noexcept {
            auto res = dynamic_cast<fullscreen_quad_resources *> (_pResources.get());

            std::memset(res->imageView, 0, sizeof(float) * 4);
        }

        void fullscreen_quad_renderer::pushData(const void * pData) noexcept {
            auto res = dynamic_cast<fullscreen_quad_resources*> (_pResources.get());

            if (pData) {
                auto data = reinterpret_cast<const float *> (pData);

                std::memcpy(res->imageView, data, sizeof(float) * 4);
            } else {
                std::memset(res->imageView, 0, sizeof(float) * 4);
            }
        }

        void fullscreen_quad_renderer::render() const noexcept {
            if (!program) {
                auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                decltype(&vsh) pShaders[] = {&vsh, &fsh};                

                program = graphics::program(graphics::program_info{pShaders, 2, nullptr, 0});

                uImageView = program.getUniformLocation("uImageView");
                uImage = program.getUniformLocation("uImage");
            }

            auto res = dynamic_cast<fullscreen_quad_resources*> (_pResources.get());

            program.use();            

            graphics::uniform::setUniform1(uImage, 0);
            graphics::uniform::setUniform4(uImageView, 1, res->imageView);

            res->model.bind();

            graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
        }
    }
}
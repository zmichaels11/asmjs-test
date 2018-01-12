#include "engine/gfx/fullscreen_quad_renderer.hpp"

#include <memory>

#include "engine/gfx/base_resources.hpp"

#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

namespace engine {
    namespace gfx {
        namespace {
            struct fullscreen_quad_resources : public virtual base_resources {
                graphics::vertex_array model;

                float imageView[4];                

                fullscreen_quad_resources();

                virtual ~fullscreen_quad_resources() {}
            };

            graphics::program PROGRAM;
            int uImageView;
        }

        fullscreen_quad_resources::fullscreen_quad_resources() {
            model = graphics::vertex_array(graphics::vertex_array_info());
        }

        fullscreen_quad_renderer::fullscreen_quad_renderer() noexcept {
            _pResources = std::make_unique<fullscreen_quad_resources>();
        }

        fullscreen_quad_renderer::~fullscreen_quad_renderer() {}

        void fullscreen_quad_renderer::reset() noexcept {
            auto res = dynamic_cast<fullscreen_quad_resources *> (_pResources.get());

            res->imageView[0] = 0.0F;
            res->imageView[1] = 0.0F;
            res->imageView[2] = 0.0F;
            res->imageView[3] = 0.0F;
        }

        void fullscreen_quad_renderer::pushData(const void * pData) noexcept {
            auto res = dynamic_cast<fullscreen_quad_resources*> (_pResources.get());

            if (pData) {
                auto data = reinterpret_cast<const float *> (pData);

                res->imageView[0] = data[0];
                res->imageView[1] = data[1];
                res->imageView[2] = data[2];
                res->imageView[3] = data[3];
            } else {
                res->imageView[0] = 0.0F;
                res->imageView[1] = 0.0F;
                res->imageView[2] = 0.0F;
                res->imageView[3] = 0.0F;
            }
        }

        void fullscreen_quad_renderer::render() const noexcept {
            if (!PROGRAM) {
#if defined(GL)
                auto vsh = graphics::shader::makeVertex("data/shaders/fullscreen_quad_renderer/330_core.vert");
                auto fsh = graphics::shader::makeFragment("data/shaders/fullscreen_quad_renderer/330_core.frag");
#elif defined(GLES30)
                auto vsh = graphics::shader::makeVertex("data/shaders/fullscreen_quad_renderer/300_es.vert");
                auto fsh = graphics::shader::makeFragment("data/shaders/fullscreen_quad_renderer/300_es.frag");
#else
                graphics::shader vsh, fsh;                
#error "Only GL and GLES 3.0 are supported!"
#endif                

                decltype(&vsh) pShaders[] = {&vsh, &fsh};


                PROGRAM = graphics::program(graphics::program_info{pShaders, 2, nullptr, 0});

                uImageView = PROGRAM.getUniformLocation("uImageView");
            }

            auto res = dynamic_cast<fullscreen_quad_resources*> (_pResources.get());

            PROGRAM.use();

            graphics::uniform::setUniform4(uImageView, 1, res->imageView);

            graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
        }
    }
}
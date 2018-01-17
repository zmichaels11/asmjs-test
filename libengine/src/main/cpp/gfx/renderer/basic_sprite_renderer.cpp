#include "engine/gfx/renderer/basic_sprite_renderer.hpp"

#include "engine/gfx/renderer/base_renderer.hpp"

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            namespace {
                struct basic_sprite_renderer_resources : public base_renderer_resources {
                    graphics::buffer vbo;
                    basic_sprite_renderer_resources(const basic_sprite_renderer_info& info);                    
                };
            }
        }
    }
}